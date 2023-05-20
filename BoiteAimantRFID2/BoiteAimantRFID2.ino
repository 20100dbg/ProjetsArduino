#include <SoftwareSerial.h>
SoftwareSerial rfid = SoftwareSerial(0,1);
String code = "370030E2D336";
bool open = false;

void setup()
{
  Serial.begin(9600);

  pinMode(2, OUTPUT);

  rfid.begin(9600);
  rfid.listen();
}

void loop()
{

    String myInput = String("");
    
    while(rfid.available() > 0)
    {
      char c = rfid.read();
      if (c > -1) myInput.concat(c);
    }
    
    //Serial.println(myInput + " - " + myInput.indexOf(code));

    if (myInput.indexOf(code) > -1)
    {
      open = true;
    }

    digitalWrite(2, open);    
    delay(200);

}
