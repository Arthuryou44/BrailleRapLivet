void TranslationTete()
{
  digitalWrite(pinCCSens, HIGH);
  analogWrite(pinCCVitesse, 127);
  for (nombreImpulsions = 0; abs(nombreImpulsions) < nombreImpulsionsCible;)
    {
      // Faire avancer l'engrenage
      digitalWrite(pinCCSens, HIGH);
      analogWrite(pinCCVitesse, 127);
      Serial.println(nombreImpulsions);
    }
  analogWrite(pinCCVitesse, 0);
  CaractereLigne++;

}

void RetourOrigineTete() {
  EtatFinCourse = digitalRead(pinFinCourse);
  while (EtatFinCourse == 1)
  {
    digitalWrite(pinCCSens, LOW);
    analogWrite(pinCCVitesse, 255);
    EtatFinCourse = digitalRead(pinFinCourse);
  }
  analogWrite(pinCCVitesse, 0);
  delay(10);
  CaractereLigne = 0;
}

void compterImpulsion() {
  if (digitalRead(pinEncodeurB) != digitalRead(pinEncodeurA)) {
    nombreImpulsions += sensRotation;
  }
  else {
    nombreImpulsions -= sensRotation;
  }
}
