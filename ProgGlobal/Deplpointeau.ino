void ASCIITable()
{
  if ((CaractereRecu <= 90) && (CaractereRecu >= 65)) {
    Serial.println("majuscule");
    EstChiffre == false;
    if (EstCapital == false) {
      EstCapital = true;
      Servoligne1(ASCIISpecial[0][16]);
      Servoligne2(ASCIISpecial[1][16]);
      Servoligne3(ASCIISpecial[2][16]);
      TranslationTete();
    }
    CaractereRecu = CaractereRecu - 65;
    Servoligne1(ASCIIAlpha[0][CaractereRecu]);
    Servoligne2(ASCIIAlpha[1][CaractereRecu]);
    Servoligne3(ASCIIAlpha[2][CaractereRecu]);
  }
  else {
    Serial.println("minuscule");
    EstCapital = false;
    if ((CaractereRecu <= 122) && (CaractereRecu >= 97)) {
      CaractereRecu = CaractereRecu - 97;
      Servoligne1(ASCIIAlpha[0][CaractereRecu]);
      Servoligne2(ASCIIAlpha[1][CaractereRecu]);
      Servoligne3(ASCIIAlpha[2][CaractereRecu]);
    }
    else {
      if ((CaractereRecu <= 57) && (CaractereRecu >= 48)) {
        Serial.println("Chiffre");
        if (EstChiffre == false) {
          EstChiffre = true;
          Servoligne1(ASCIISpecial[0][3]);
          Servoligne2(ASCIISpecial[1][3]);
          Servoligne3(ASCIISpecial[2][3]);
          TranslationTete();
        }
        CaractereRecu = CaractereRecu - 48;
        Servoligne1(ASCIINum[0][CaractereRecu]);
        Servoligne2(ASCIINum[1][CaractereRecu]);
        Servoligne3(ASCIINum[2][CaractereRecu]);
      }
      else {
        EstChiffre = false;
        Serial.println("Speciaux");
        if ((CaractereRecu >= 32) && (CaractereRecu <= 47)) {
          CaractereRecu = CaractereRecu - 32;
          Servoligne1(ASCIISpecial[0][CaractereRecu]);
          Servoligne2(ASCIISpecial[1][CaractereRecu]);
          Servoligne3(ASCIISpecial[2][CaractereRecu]);
        }
      }
    }
  }
}
