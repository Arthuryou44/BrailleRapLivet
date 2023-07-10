void Servoligne1(byte x)
{
  if (x == 1)
    Pointeau1();
  if (x == 2)
    Pointeau2();
  if (x == 3)
  {
    Pointeau1(); 
    Pointeau2();
  }
}

void Servoligne2(byte x)
{
  if (x == 1)
    Pointeau3();
  if (x == 2)
    Pointeau4();
  if (x == 3)
  {
    Pointeau3();
    Pointeau4();
  }
}

void Servoligne3(byte x)
{
  if (x == 1)
    Pointeau5();
  if (x == 2)
    Pointeau6();
  if (x == 3)
  {
    Pointeau5();
    Pointeau6();
  }
}
