void Pos_ini1() {
  servo1.write(servo1repos); // servomoteur en position initiale
  delay(Tempo);

}
void Pointeau1() {
  servo1.write(p11); // position de descente pointeau 1
  delay(Tempo);
  Pos_ini1();
}

void Pointeau2() {
  servo1.write(p12); // position de descente pointeau 2
  delay(Tempo);
  Pos_ini1();
}

void Pos_ini2() {
  servo2.write(servo2repos); // servomoteur en position initiale
  delay(Tempo);
}

void Pointeau3() {
  servo2.write(p21); // position de descente pointeau 3
  delay(Tempo);
  Pos_ini2();
}

void Pointeau4() {
  servo2.write(p22); // position de descente pointeau 4
  delay(Tempo);
  Pos_ini2();
}

void Pos_ini3() {
  servo3.write(servo3repos); // servomoteur en position initiale
  delay(Tempo);
}

void Pointeau5() {
  servo3.write(p31); // position de descente pointeau 5
  delay(Tempo);
  Pos_ini3();
}

void Pointeau6() {
  servo3.write(p32); // position de descente pointeau 6
  delay(Tempo);
  Pos_ini3();
}
