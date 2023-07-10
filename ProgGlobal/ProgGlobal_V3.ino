#include <ArduinoBLE.h> //Librairie Bluetooth
#include <Servo.h> //Librairie Servomoteurs
#include <BasicStepperDriver.h> //Librairie Pas à Pas
#include <Wire.h> //Librairie I2C
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Adafruit_SSD1306.h> //Librairies écran I2C
#include <splash.h>

//définitions des identifiants uniques (Universally Unique IDentifier) des 2 services
#define SERVICE_UUID "0000FFE0-0000-1000-8000-00805F9B34FB"
#define CHARACTERISTIC_UUID "0000FFE1-0000-1000-8000-00805F9B34FB"

//Définition des connecteurs
#define pinCCSens 5
#define pinEncodeurB 8
#define pinEncodeurA 9
#define pinCCVitesse 6
#define pinFinCourse 12

#define pinCapteurFeuille 7
#define pinSensFeuille 3
#define pinStepFeuille 2
#define pinEnableFeuille 4

#define pinServo1 17
#define pinServo2 16
#define pinServo3 15

//Définition du moteur Pas à Pas
#define MOTOR_STEPS 400
#define RPM 70
#define MICROSTEPS 1 //Mode Pas Complets
#define PasParLigne 101 //101 est le nombre de pas calculés pour déplacer d'une ligne

//Déclaration du moteur Pas à Pas
BasicStepperDriver MoteurFeuille(MOTOR_STEPS, pinSensFeuille, pinStepFeuille, pinEnableFeuille);

//Definition de la taille de l'écran, des icones bitmap
#define L_OLED 128
#define H_OLED 64
#define L_Bitmap 24
#define H_Bitmap 24

//Codes Erreures
#define Bluetooth 0
#define Feuille 1
#define MoteurCC 2
#define ERREUR 3
#define LIVET 4
#define INFOcircle 5

volatile int nombreImpulsions = 0;
const int nombreImpulsionsCible = 175;
int sensRotation = 1;
int duree;
int Tempo = 200;
bool EtatEncodeur;
bool EtatFinCourse;

byte servo1repos = 93;
byte servo2repos = 90;
byte servo3repos = 79;
float Action = 38;

float p11 = servo1repos + Action;
float p12 = servo1repos - Action;
float p21 = servo2repos + Action;
float p22 = servo2repos - Action;
float p31 = servo3repos + Action;
float p32 = servo3repos - Action;

//Traduction des caractères ASCII en instructions Servomoteurs ( 0 = Repos, 1 = Coté 'droit', 2 = Coté 'Gauche', 3 = Deux cotés )
byte ASCIIAlpha[3][26] = {
  //A B  C  D    E  F  G  H    I  J  K  L    M  N  O  P    Q  R  S  T    U  V  W  X    Y  Z
  {1, 1, 3, 3,   1, 3, 3, 1,   2, 2, 1, 1,   3, 3, 1, 3,   3, 1, 2, 2,   1, 1, 2, 3,   3, 1},
  {0, 1, 0, 2,   2, 1, 3, 3,   1, 3, 0, 1,   0, 2, 2, 1,   3, 3, 1, 3,   0, 1, 3, 0,   2, 2},
  {0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 1, 1,   1, 1, 1, 1,   1, 1, 1, 1,   3, 3, 2, 3,   3, 3}
};
byte ASCIINum[3][10] = {
  //0 1  2  3    4  5  6  7    8  9
  {1, 1, 1, 3,   3, 1, 3, 3,   1, 2},
  {3, 0, 1, 0,   2, 2, 1, 3,   3, 1},
  {0, 0, 0, 0,   0, 0, 0, 0,   0, 0},
};
byte ASCIISpecial[3] [17] = {
  //  !  "  #    $  %  &  '   (  )  *  +    ,  -  .  / Maj
  {0, 0, 0, 2,   0, 0, 0, 0,  0, 0, 0, 0,   0, 0, 0, 2, 2},
  {0, 3, 3, 2,   0, 0, 0, 0,  1, 2, 2, 3,   1, 0, 3, 0, 0},
  {0, 1, 3, 3,   0, 0, 0, 1,  3, 3, 1, 1,   0, 3, 2, 1, 2}
};

bool EstChiffre = LOW;
bool EstCapital = LOW;

String valeurAtransmettre = "";
char CaractereRecu = 0;
byte CaractereLigne = 0;
byte CaractereMax = 10;
byte CaractereEcran = 0;
unsigned int LongueurTexte = 0;

//Initialisation des servomoteurs
Servo servo1;
Servo servo2;
Servo servo3;

// BLE création d'un service à consommer, avec son UUID propre (cf l.6)
BLEService serviceGestionDesServos(SERVICE_UUID);
// BLE définition des caractéristiques du service (cf l.7)
BLEStringCharacteristic caracteristiqueService(CHARACTERISTIC_UUID, BLERead | BLEWriteWithoutResponse, 200);
BLEDevice central;

//Déclaration de l'écran I2C
Adafruit_SSD1306 ecranAffiche(L_OLED, H_OLED, &Wire, -1);
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

void setup() {
  Serial.begin(9600);
  delay(10000);
  //while (!Serial);

  Serial.println("start");

  //Test initialisation Bluetooth
  if (!BLE.begin()) {
    Serial.println("starting BLE device failed!");
    while (1);
  }

  // définition du nom de la carte
  BLE.setLocalName("Nano_33_BLE");
  Serial.println("Nom de la carte : Nano_33_BLE");
  // on 'greffe' le service à consommer au service carte
  BLE.setAdvertisedService(serviceGestionDesServos);
  // ajout de caracteristiques au service (cf l.36)
  serviceGestionDesServos.addCharacteristic(caracteristiqueService);
  // ajout du service 'serviceGestionDesServos' parmi ceux disponibles
  BLE.addService(serviceGestionDesServos);
  // valeur initiale de caracteristique:
  caracteristiqueService.writeValue("");
  // début diffusion du service BLE
  BLE.advertise();

  Serial.println("BLE Nano_33 active, en attente de connection");
  Serial.print("BLE: ");
  Serial.println(BLE.address());

  //Connection des servomoteurs
  servo1.attach(pinServo1);
  servo2.attach(pinServo2);
  servo3.attach(pinServo3);
  servo1.write(servo1repos);
  servo2.write(servo2repos);
  servo3.write(servo3repos);

  //Association du programme à l'interruption
  attachInterrupt(digitalPinToInterrupt(pinEncodeurA), compterImpulsion, CHANGE);

  //Initialisation du  Moteur pas à pas
  MoteurFeuille.begin(RPM, MICROSTEPS);
  MoteurFeuille.setEnableActiveState(LOW); //Etat du pin Enable quand actif : NL0  ( Selon si le pin utilisé est ENABLE ou SLEEP )

  //Definition des entrées/sorties
  pinMode(pinCCVitesse, OUTPUT);
  pinMode(pinCCSens, OUTPUT);
  pinMode(pinEncodeurA, INPUT);
  pinMode(pinEncodeurB, INPUT);
  pinMode(pinFinCourse, INPUT_PULLUP);
  pinMode(pinCapteurFeuille, INPUT);

  EtatFinCourse = HIGH;
  duree = millis();

  Adafruit_SSD1306 setTextWrap(1); //Le texte revient automatiquement à gauche lorsqu'il atteint le bord de l'écran

  if (!ecranAffiche.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    //Verification de l'initialisation de l'écran
    Serial.println(F("SSD1306 allocation failed"));
    Serial.print("Panne d'écran");
    while (1) {}
  }
  ecranAffiche.cp437(true); //Utilisation du codage ASCII
  //ecranAffiche.setFont(&FreeMono12pt7b); //Utilisation d'une police particulière

  Serial.print("1");
  ecranAffiche.display(); // the library initializes this with an Adafruit splash screen.
  Serial.println("2");
  delay(500); // Pause for 0.5 seconds
  ecranAffiche.clearDisplay();   // Vide l'affichage
  Serial.println("3");
  INIT_LIVET(); //Animations de démarrage
  Serial.println("4");
  Init();
  Serial.print("BLE: ");
  ecranAffiche.println("BLE: ");
  Serial.println(BLE.address());
  ecranAffiche.println(BLE.address());
  ecranAffiche.display();

}

void loop()
{
  central = BLE.central();
  // si un appareil est connecté au périphérique:
  if (central)
  {
    Connection_BT();
    delay(1000);
    while (central.connected()) {
      if (caracteristiqueService.written()) { //Si reception d'une chaine de caractères
        Reception_BT(); //Reception de la chaîne de caractères par Bluetooth
        //Reception_Serie(); //Reception de la chaîne de caractères par Port Série
        OLED_Background();
        ecranAffiche.setCursor(6, 6);
        for (unsigned int i = 0; i < LongueurTexte - 1; i++) {
          CaractereRecu = int(valeurAtransmettre[i]); //Conversion du charactère numéro "i" du string reçu en variable int
          //DetectionFeuille(); //Verification qu'une feuille est prète à être embossée
          OLEDchar(CaractereRecu); //Affichage du caractère sur l'écran
          ASCIITable(); //Traduction du caractère en ordres servomoteur
          if (CaractereLigne <= CaractereMax /*&& CaractereRecu != 10 && CaractereRecu != 13*/)
            //Si le chariot est arrivé en bout de ligne, ou si CR LF
          {
            TranslationTete();
          }
          else
          {
            RetourOrigineTete();
            TransFeuille(1, 0, 1, 1);
          }
        }
        Serial.flush(); // vide la mémoire
      }
    }
    // déconnection de l'appareil:
    Serial.print("Déconnection: ");
    OLEDbit(Bluetooth);
    ecranAffiche.println("Deconnection");
    ecranAffiche.print("Bluetooth");
    ecranAffiche.display();
  }
}

void Init()
{
  analogWrite(pinCCVitesse, 0);
  TransFeuille(0, 0, 0, 0);
  Pos_ini1();
  Pos_ini2();
  Pos_ini3();
  RetourOrigineTete();
}
