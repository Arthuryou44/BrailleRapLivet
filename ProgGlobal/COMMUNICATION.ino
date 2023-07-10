void Connection_BT() {
  OLED_Background();
  OLEDbit(Bluetooth);
  Serial.print("Connecté à l'appareil: ");
  ecranAffiche.println("Adresse Telephone");
  // donner l'adresse MAC de la carte:
  Serial.println(central.address());
  ecranAffiche.println(central.address());
  ecranAffiche.display();
}

void Reception_BT()
{
  Serial.print("Texte reçu: ");
  valeurAtransmettre = caracteristiqueService.value();
  Serial.println(valeurAtransmettre); //vérifie que la valeur reçue est correcte
  LongueurTexte = valeurAtransmettre.length();//Obtenir le nombre de charactère du texte reçu
  Serial.println(LongueurTexte);
}

void Reception_Serie() {
  while (Serial.available() == 0) {}
  CaractereRecu = Serial.read();
  Serial.print (CaractereRecu, DEC);
  Serial.print ("  ");
  OLEDchar(CaractereRecu);
}

void Deconnection_BT() {

}


void Erreurs(byte x) {
  ecranAffiche.clearDisplay(); //Ecran effacé
  OLED_Background(); //Affichage du fond
  OLEDbit(ERREUR); //Dessin de l'icone correspondant à l'erreur
  switch (x) { //Ecriture en fonction de l'erreur
    case Feuille:
      ecranAffiche.print("ERREUR :");
      ecranAffiche.println("Feuille Introuvable");
      break;

    case MoteurCC:
      ecranAffiche.print("ERREUR :");
      ecranAffiche.println("MoteurCC");
      break;

    case Bluetooth:
      ecranAffiche.print("ERREUR :");
      ecranAffiche.println("Bluetooth");
      break;
  }
  ecranAffiche.display(); //Affihage

}

void OLEDchar(char x) //Programme d'ecriture de caractères
{
  byte Yecran;
  byte Xecran;
  Yecran = ecranAffiche.getCursorY(); //Obtenir la position en Y du curseur
  Xecran = ecranAffiche.getCursorX(); //Obtenir la position en X du curseur

  if (Xecran == 0) { //Si le caractères est revenu à gauche
    ecranAffiche.setCursor(6, Yecran); //Il est décalé afin de ne pas être derrière le fond d'écran
  }
  if (Yecran >= 56) //Si le caractère sors de l'écran
  {
    ecranAffiche.clearDisplay(); //Ecran effacé
    OLED_Background();    //Fond d'écran remis
    ecranAffiche.setCursor(6, 6); //Curseur repositionné
  }
  ecranAffiche.print(x); //Ecrire le caractère reçu
  ecranAffiche.display();
}

void OLEDbit(unsigned char x) { //Programme d'affichage d'icones bitmap
  switch (x) { //Choix de l'image bitmap selon la situation
    case Bluetooth:
      ecranAffiche.drawBitmap((ecranAffiche.width()  - L_Bitmap ) / 2, 8, Logo_Bluetooth, L_Bitmap, H_Bitmap, 1);
      break;
    case ERREUR:
      ecranAffiche.drawBitmap((ecranAffiche.width()  - L_Bitmap ) / 2, 8, alertcircle, L_Bitmap, H_Bitmap, 1);
      break;
    case LIVET:
      ecranAffiche.drawBitmap((ecranAffiche.width()  - 50 ) / 2, 8, LIVETLOGO, 50, 50, 1);
      break;
    case INFOcircle:
      ecranAffiche.drawBitmap((ecranAffiche.width()  - L_Bitmap ) / 2, 8, info, L_Bitmap, H_Bitmap, 1);
      break;
  }
  ecranAffiche.setCursor(6, 8 + H_Bitmap + 3); //Positionnement du curseur
  ecranAffiche.display();
}

void INIT_LIVET() { //Animation de démarrage
  ecranAffiche.clearDisplay(); //Effacer ecran
  ecranAffiche.setTextSize(1.5); //Définir taille de texte
  ecranAffiche.setTextColor(SSD1306_WHITE); //Couleur de texte ( notre écran est monochrome )
  OLEDbit(Bluetooth); //Affichage du logo bluetooth
  delay(1000);
  ecranAffiche.setCursor(6, 6); //Positionnement du curseur
  ecranAffiche.clearDisplay(); //Effacer ecran
  OLEDbit(LIVET); //Affichage du logo de Livet
  delay(1000);
  ecranAffiche.invertDisplay(true); //Inversion des couleurs ( Noir devient Blanc et vice versa )
  delay(500);
  ecranAffiche.invertDisplay(false);//Arrêt Inversion des couleurs ( Noir devient Blanc et vice versa )
  delay(500);
  ecranAffiche.clearDisplay(); //Effacer Ecran
  OLED_Background(); //Affichage fond d'écran

}
void OLED_Background() { //Fond d'écran, un cadre rectangulaire aux coins arrondis
  ecranAffiche.clearDisplay();
  ecranAffiche.drawRoundRect(0, 0, ecranAffiche.width(), ecranAffiche.height(), 8, SSD1306_WHITE); //X,Y,Largeur,Hauteur,Rayon Congés,Couleur
  ecranAffiche.display();
}
