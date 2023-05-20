#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Wire.h>
SoftwareSerial rfid = SoftwareSerial(0,1);

const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[rows] = {2,3,4,5}; //connect to the row pinouts of the keypad
byte colPins[cols] = {6,7,8}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

bool badgeOk = false;
bool needReset = false;
String code = "370030E2D336";
char codeReset[7] = "197355";
char inputReset[7] = "";
int idxInputReset = 0;


int lastSteadyState = LOW;       // the previous steady state from the input pin
int lastFlickerableState = LOW;  // the previous flickerable state from the input pin
int currentState;                // the current reading from the input pin
unsigned long lastDebounceTime = 0;

void setup()
{
  pinMode(10, OUTPUT); //led verte
  pinMode(11, OUTPUT); //led rouge
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);

  pinMode(13, INPUT_PULLUP); //bouton reset

  Wire.begin();
  rfid.begin(9600);
  rfid.listen();
}

//jaune : 370030E2D336
//rouge : 38006553727C
//bleu : 380018479EF9

void(* resetFunc) (void) = 0;

void loop()
{
  Wire.requestFrom(8, 1);
  receiveEvent(10);

  char key = keypad.getKey();

  //bouton reset
  currentState = digitalRead(13);

  if (currentState != lastFlickerableState) {
    lastDebounceTime = millis();
    lastFlickerableState = currentState;
  }

  if ((millis() - lastDebounceTime) > 50) {
    if (lastSteadyState == HIGH && currentState == LOW) resetFunc();
    lastSteadyState = currentState;
  }
  //

  if (needReset)
  {
    digitalWrite(11, HIGH);
    
    if (key != NO_KEY)
    {
      if (key == '#')
      {
        bool flag = true;
        for (int i = 0; i < 6; i++) if (codeReset[i] != inputReset[i]) flag = false;
    
        if (flag)
        {
          badgeOk = false;
          needReset = false;

          while (rfid.available() > 0) rfid.read();
          while (Wire.available() > 0) Wire.read();
          digitalWrite(11, LOW);
        }
        ViderInput();
      }
      else if (key == '*') ViderInput();
      else
      {
        if (idxInputReset >= 6) ViderInput();
        inputReset[idxInputReset++] = key;
      }
    }
    return;
  }
  
  if (!needReset && !badgeOk) //listening RFID
  {
    String myInput = String("");
    while(rfid.available() > 0)
    {
      char c = rfid.read();
      if (c > -1) myInput.concat(c);
    }
    
    if (code.length() > 3)
    {
      if (myInput.indexOf(code) > -1)
      {
        badgeOk = true;
        digitalWrite(10, HIGH);
        delay(500);
        digitalWrite(10, LOW);
      }
    }
    else needReset = true;

    delay(200);
  }
  
  if (key != NO_KEY)
  {
    if (badgeOk)
    {
      if (key == '*') 
      {
        ResetWire();
        digitalWrite(10, HIGH);
        delay(50);
        digitalWrite(10, LOW);
      }
      
      Wire.beginTransmission(8);
      Wire.write(key);
      byte e = Wire.endTransmission();
      
      digitalWrite(11, HIGH);
      delay(50);
      digitalWrite(11, LOW);
    }
    else needReset = true;
  }
}

void receiveEvent(int numBytes)
{
  while (Wire.available() > 0)
  {
    char c = Wire.read();
    if (!badgeOk) needReset = true;
  }
}

void ResetWire()
{
  Wire.end();
  delay(100);
  Wire.begin();
  delay(100);
}

void ViderInput()
{
  while (Wire.available() > 0) Wire.read();
  memset(inputReset, 0, sizeof inputReset);
  idxInputReset = 0;
}
