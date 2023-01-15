#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#include <Wire.h>

char message[16] = "";
unsigned long duree = 500;
bool buzzed = false;
long secondesRestantes;
int etatInterrupteur = HIGH;

char code[8] = "7355608";
char myInput[8] = "";
int idxInput = 0;
bool codeValide = false;
bool animationDone = false;

//bool moduleError = false;
bool connectOk = false;

byte customChar[8] = { 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111 };

void setup()
{
  pinMode(6, OUTPUT); //led rouge
  pinMode(7, OUTPUT); //led verte
  pinMode(10, OUTPUT); //buzzer
  pinMode(8, INPUT); //interrupteur levier
  digitalWrite(8, etatInterrupteur);

  Wire.begin(8); // join I2C bus as the master
  Wire.onRequest(requestEvent); 
  Wire.onReceive(receiveEvent);
  
  lcd.begin(16, 2);
  lcd.createChar(0, customChar);
  lcd.display();
}

bool handleKey(char key)
{
  bool flag = false;
  if (key == '#')
  {
    flag = true;
    for (int i = 0; i < 7; i++) if (code[i] != myInput[i]) flag = false;
    if (!flag) ViderInput();
  }
  else if (key == '*') ViderInput();
  else
  {
    if (idxInput >= 7) ViderInput();
    myInput[idxInput++] = key;
  }
  return flag;
}

void ViderInput()
{
  memset(myInput, 0, sizeof myInput);
  idxInput = 0;
}

void ShowAnimation()
{
  for (int i = 0; i < 16; i++)
  {
    lcd.setCursor(i, 0);
    lcd.write((byte)0);
    delay(50);
  }
  animationDone = true;
}

void loop()
{
  etatInterrupteur = digitalRead(8);
  lcd.clear();

  if (!codeValide) secondesRestantes = duree - (millis() / 1000);
  else
  {
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
  }
  
  if (secondesRestantes > 0)
  {
    long minutes = secondesRestantes / 60;
    long secondes = secondesRestantes - (minutes * 60);
    
    if (etatInterrupteur == HIGH)
    {
      if (codeValide)
      {
        if (animationDone) sprintf(message, "Disamorced");
        else ShowAnimation();
      }
      else
      {
        digitalWrite(6, HIGH);
        sprintf(message, "Code : %s", myInput);
      }
    }
    else
    {
      digitalWrite(6, LOW);
      sprintf(message, "%02" PRId32 ":%02d",minutes,secondes);
    }
  }
  else
  {
    if (!buzzed)
    {
      tone (10, 400, 1000);
      buzzed = true;
    }
    digitalWrite(6, LOW);
    sprintf(message, "Trop tard...");
  }

  lcd.print(message);
  delay(100);
}

void receiveEvent(int numBytes)
{
  while (Wire.available() > 0)
  {
    char c = Wire.read();
    if (etatInterrupteur == HIGH) codeValide = handleKey(c);
  }
}
 
void requestEvent()
{
  Wire.write('X');
}
