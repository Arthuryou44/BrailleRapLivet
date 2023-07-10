void DetectionFeuille() {
  bool PresenceFeuille = LOW;
  PresenceFeuille = digitalRead(pinCapteurFeuille);
  unsigned long TempsPasse = 0;
  unsigned long Temps = millis();
  while ( PresenceFeuille == LOW && TempsPasse < 5000) {
    TransFeuille(1, 255, 1, 0);
    PresenceFeuille = digitalRead(pinCapteurFeuille); //Lecture du phototransistor
    TempsPasse = abs(millis() - Temps); //Calcul du temps passé dans la boucle
    Serial.println(TempsPasse);
  }
  if (TempsPasse >= 4999) {//Si plus de 5s sont écoulées
    Erreurs(Feuille);//Indication qu'aucune feuille n'est détecté depuis trop longtemps
  }
  else {
    TempsPasse = 0;
    TransFeuille(0, 0, 1, 0);//Arret du moteur PaP
  }
}

//Le programme inclus des traces de changements et des projets qui n'ont pas encore été développés
void TransFeuille(bool Marche, byte Vitesse, bool Sens, byte Lignes) {
  if (Marche == HIGH) {
    digitalWrite(pinEnableFeuille, LOW); //Mise en marche du driver
    digitalWrite(pinSensFeuille, Sens); //Definition du sens de rotation
    if (Lignes != 0) {
      for (unsigned int i = 0; i < (PasParLigne * Lignes); i++)
      {
        digitalWrite(pinStepFeuille, HIGH);
        delay(1);
        digitalWrite(pinStepFeuille, LOW);
        delay(1);
      }
    }
    else {
      digitalWrite(pinStepFeuille, HIGH);
      delay(5);
      digitalWrite(pinStepFeuille, LOW);
      delay(5);
    }
  }
  else {
    digitalWrite(pinEnableFeuille, HIGH);
  }
}
