bool bVertWasUp = true;
bool bRougeWasUp = true;
bool bBleuWasUp = true;
bool bJauneWasUp = true;

#define LEDVERT 6
#define LEDROUGE 7
#define LEDBLEU 8
#define LEDJAUNE 9

#define NBMAX 8
#define DUREEMAX 3000
int tabMystere[NBMAX];
int tabJoue[NBMAX];

bool showLED = true;
int idxCurrent = 0;
int played = 0;
int nbPlayed = 0;
bool playing = true;
unsigned long lastTouche = 0;

void setup()
{
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(LEDVERT, OUTPUT);
  pinMode(LEDROUGE, OUTPUT);
  pinMode(LEDBLEU, OUTPUT);
  pinMode(LEDJAUNE, OUTPUT);
  pinMode(10, OUTPUT); //buzzer
  
  digitalWrite(LEDVERT, LOW);
  digitalWrite(LEDROUGE, LOW);
  digitalWrite(LEDBLEU, LOW);
  digitalWrite(LEDJAUNE, LOW);

  randomSeed(analogRead(A1));

  for (int i = 0; i < NBMAX; i++) tabMystere[i] = random(6, 10);
  
}

void LightVert()
{
  digitalWrite(LEDVERT, HIGH);
  tone (10, 100, 300);
  delay(300);
  digitalWrite(LEDVERT, LOW);
}

void LightRouge()
{
  digitalWrite(LEDROUGE, HIGH);
  tone (10, 300, 300);
  delay(300);
  digitalWrite(LEDROUGE, LOW);
}

void LightBleu()
{
  digitalWrite(LEDBLEU, HIGH);
  tone (10, 500, 300);
  delay(300);
  digitalWrite(LEDBLEU, LOW);
}

void LightJaune()
{
  digitalWrite(LEDJAUNE, HIGH);
  tone (10, 700, 300);
  delay(300);
  digitalWrite(LEDJAUNE, LOW);
}

void win()
{
  playing = false;
  
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(LEDVERT, HIGH);
    digitalWrite(LEDROUGE, HIGH);
    digitalWrite(LEDBLEU, HIGH);
    digitalWrite(LEDJAUNE, HIGH);
    delay(1000);
    digitalWrite(LEDVERT, LOW);
    digitalWrite(LEDROUGE, LOW);
    digitalWrite(LEDBLEU, LOW);
    digitalWrite(LEDJAUNE, LOW);
    delay(1000);
  }

}

void lose()
{
  playing = false;
  digitalWrite(LEDROUGE, HIGH);
  
  for (int i = 0; i < 500; i++)
  {
    tone(10, 500 - i, 50);
    delay(5);
  }
  
}

void loop()
{
  if (!playing) return;

  if (showLED)
  {
    for (int i = 0; i < nbPlayed + 1; i++)
    {
      if (tabMystere[i] == LEDVERT) LightVert();
      else if (tabMystere[i] == LEDROUGE) LightRouge();
      else if (tabMystere[i] == LEDBLEU) LightBleu();
      else if (tabMystere[i] == LEDJAUNE) LightJaune();
      delay(500);
    }
    lastTouche = millis();
    
    nbPlayed = 0;
    played = 0;
    showLED = false;
  }
  
  if (lastTouche + DUREEMAX < millis())
  {
    lose();    
  }


  bool bVertIsUp = digitalRead(2);
  if (bVertWasUp && !bVertIsUp) {
    delay(10);
    bVertIsUp = digitalRead(2);
  }
  bVertWasUp = bVertIsUp;

  bool bRougeIsUp = digitalRead(3);
  if (bRougeWasUp && !bRougeIsUp) {
    delay(10);
    bRougeIsUp = digitalRead(3);
  }
  bRougeWasUp = bRougeIsUp;

  bool bBleuIsUp = digitalRead(4);
  if (bBleuWasUp && !bBleuIsUp) {
    delay(10);
    bBleuIsUp = digitalRead(4);
  }
  bBleuWasUp = bBleuIsUp;

  bool bJauneIsUp = digitalRead(5);
  if (bJauneWasUp && !bJauneIsUp) {
    delay(10);
    bJauneIsUp = digitalRead(5);
  }
  bJauneWasUp = bJauneIsUp;


  if (!bVertIsUp)
  {
    LightVert();
    played = LEDVERT;
  }  
  else if (!bRougeIsUp)
  {
    LightRouge();
    played = LEDROUGE;
  }
  else if (!bBleuIsUp)
  {
    LightBleu();
    played = LEDBLEU;
  }
  else if (!bJauneIsUp)
  {
    LightJaune();
    played = LEDJAUNE;
  }

  //on remplie le tableau, il faut vérifier chaque frappe
  if (!bVertIsUp || !bRougeIsUp || !bBleuIsUp || !bJauneIsUp)
  {
    tabJoue[nbPlayed] = played;
    nbPlayed += 1;
    lastTouche = millis();

    for (int i = 0; i < nbPlayed; i++)
    {
      if (tabMystere[i] != tabJoue[i]) lose();
    }
  }

  //suite de frappe terminée
  if (nbPlayed == idxCurrent + 1)
  {
    showLED = true;
    
    //est ce que c'est fini ?
    if (tabMystere[idxCurrent++] == played)
    {
      if (idxCurrent == NBMAX) win();
    }

    delay(400);
  }

}
