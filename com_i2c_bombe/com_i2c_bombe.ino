#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#include <Wire.h>
#include <EEPROM.h>

char message[16] = "";
unsigned long duree = 500;
unsigned long tempsEcoule = 0;
unsigned long delaiAvantDebut = 0;
bool start = false;
bool buzzed = false;
long secondesRestantes;

bool etatInterrupteur = HIGH;
char code[8] = "7355608";
char myInput[8] = "";
int idxInput = 0;

bool codeValide = false;
bool animationDone = false;

int lastSteadyState = LOW;       // the previous steady state from the input pin
int lastFlickerableState = LOW;  // the previous flickerable state from the input pin
int currentState;                // the current reading from the input pin
unsigned long lastDebounceTime = 0;

//bool moduleError = false;
bool connectOk = false;

byte customChar[8] = { 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111 };

void setup()
{
  EEPROM.get(0, tempsEcoule);

  pinMode(6, OUTPUT); //led rouge
  pinMode(7, OUTPUT); //led verte
  pinMode(10, OUTPUT); //buzzer
  pinMode(8, INPUT); //interrupteur levier
  digitalWrite(8, etatInterrupteur);
  pinMode(13, INPUT_PULLUP); //bouton reset

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

void(* resetFunc) (void) = 0;

void loop()
{
  etatInterrupteur = digitalRead(8);
  currentState = digitalRead(13);

  if (currentState != lastFlickerableState) {
    lastDebounceTime = millis();
    lastFlickerableState = currentState;
  }

  if ((millis() - lastDebounceTime) > 50)
  {
    if (lastSteadyState == HIGH && currentState == LOW)
    {
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);

      tempsEcoule = 0;
      EEPROM.put(0, tempsEcoule);
      tone (10, 400, 50);
      delay(100);
      resetFunc();
    }
    lastSteadyState = currentState;
  }

  lcd.clear();

  //le compte a rebours commence quand le levier est baissé
  if (!start)
  {
    if (etatInterrupteur == LOW) 
    {
      start = true;
      delaiAvantDebut = millis() / 1000;
    }
    return;
  }

  //tant que le code n'est pas valide, le compteur tourne, sinon la LED est verte
  if (!codeValide)
  {
    secondesRestantes = duree - (tempsEcoule + (millis() / 1000) - delaiAvantDebut);
    EEPROM.put(0, tempsEcoule + (millis() / 1000) - delaiAvantDebut);
  }
  else
  {
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
  }
  
  if (secondesRestantes > 0)
  {
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
      sprintf(message, "%02" PRId32 ":%02d", secondesRestantes / 60, secondesRestantes - ((secondesRestantes / 60) * 60));
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
