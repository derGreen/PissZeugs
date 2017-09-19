/**********************************************************
  Some nice Intro text that lets you want to piss the hell out of your bladder.
**********************************************************/
#include "LiquidCrystal.h"
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// which pin to use for reading the sensor? can use any pin!
#define FLOWSENSORPIN 7

// count how many pulses!
volatile uint16_t pulses = 0;
// track the state of the pulse pin
volatile uint8_t lastflowpinstate;
// you can try to keep time of how long it is between pulses
volatile uint32_t lastflowratetimer = 100;
// and use that to calculate a flow rate
volatile float timer;

// Declare global variables
bool session = false; // describes if a session is activ or not
String fullName;

int highScore[10] = {99, 90, 80, 70, 60, 50, 40, 30, 20, 10}; //stores the highscore
String highPlayer[10] = {"Tom", "Hans", "Dieter", "Yannick", "Clausi", "Patrick", "Johann", "Paul", "Steffen", "Karl"}; //stores the highscore

// Calculates miliLiters from the pulses
int mliters = 0;

// Counts the highscore position
int count = 0;

//display stuff
const int numRows = 4; //2;
const int numCols = 20;//12;
/*
byte icon[8] = {
  B00110,
  B00110,
  B00001,
  B00011,
  B00101,
  B11111,
  B00010,
  B00101
};
*/
byte icon[8] = {
  B10001,
B11001,
B10001,
B11101,
B10001,
B11001,
B10001,
B11111
};
// Interrupt is called once a millisecond, looks for any pulses from the sensor!
SIGNAL(TIMER0_COMPA_vect)
{
  uint8_t x = digitalRead(FLOWSENSORPIN);

  if (x == lastflowpinstate)
  {
    //lastflowratetimer++;
    return; // nothing changed!
  }

  if (x == HIGH)
  {
    //low to high transition!
    pulses++;
  }
  lastflowpinstate = x;
  lastflowratetimer = 0;
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
  }
}

//*************************************************
//*********** Setup
//*************************************************
void setup()
{
  //Serial.begin(9600);
  // Serial.print("Flow sensor test!");
  lcd.begin(16, 2);
  // declare pin 9 to be an output for the contrast:
  pinMode(9, OUTPUT);
  analogWrite(9, 50);

  pinMode(FLOWSENSORPIN, INPUT);
  digitalWrite(FLOWSENSORPIN, HIGH);
  lastflowpinstate = digitalRead(FLOWSENSORPIN);
  useInterrupt(true);
  randomSeed(analogRead(0));
  lcd.createChar(1, icon);

  //lcd.begin(numCols, numRows);
/*
  lcd.createChar(1, icon);
  lcd.setCursor(19, 2);
  lcd.write(byte(1));*/
};

//*************************************************
//*********** Mainloop
//*************************************************
void loop()             // run over and over again

{


  //counts the time since the last signal
  timer = (lastflowratetimer / 10); // in seconds
  lastflowratetimer++;

  if (timer == 0) //((timer >= 0) && (timer < 3))              //Start a session
  {
    gameStart();
  }
  else if (timer == 3)           // End a session
  {
    // save the score to the highscore array
    setHighscore(fullName, mliters);  // Name des Spielers und Punkte übergeben an Higscore funktion
    session = false;
    pulses = 0;

  }
  else if (timer > 3)            // Starts the Game else stays in Idle mode
  {
    IdleMode();
  }
  /*
  lcd.setCursor(7, 4);
  lcd.print(timer);
  lcd.setCursor(18, 4);
  lcd.print(lastflowpinstate);
  lcd.setCursor(0, 4);
  lcd.print(lastflowratetimer);*/
  delay(100);
};


/********************************************************************/
void IdleMode()
{
  showHigh();
  lcd.setCursor(0, 0);
  lcd.print("IdleTime: "); lcd.print( timer ); lcd.print("      ");
};

/********************************************************************/
void showHigh()
{
  if (count == 100)
  {
    count = 0;
  };
  lcd.setCursor(0, 1);
  lcd.print("#" + String((count / 10) + 1)+ highPlayer[(count / 10)] + highScore[(count / 10)] + "ml                    ");
  count++;
};

/********************************************************************/
void gameStart()
{
  mliters = int(pulses * 2.556);
  if (session == false)
  {
    fullName = rndName();
    session = true;
  };
  printLCD(mliters, fullName);
};

/********************************************************************/
void printLCD(int result, String fullName)  // Show actual points and name on LCD
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(fullName);
  lcd.setCursor(10, 0);
  lcd.print(result);
  lcd.print("ml ");
  lcd.setCursor(0, 1);
  PissingBar(result,highScore[0]);
  delay(100);
};

/********************************************************************/
void PissingBar(int actual, int max)  // Show actual points and name on LCD
{
  int x = (actual/max)*16;
  //int x = random(16);
  lcd.setCursor(0,1);
  lcd.print(x);
  lcd.write(byte(1));
};

/********************************************************************/
void setHighscore(String player, int points)  // compares score with top 10 and add to position
{
  if (session == true)
  {
    int i = 9;
    // Search for the Rank

    while ((i >= 0) && (points > highScore[i]))
    {
      highScore[i+1]  = highScore[i];
      highPlayer[i+1] = highPlayer[i];
      i--;
    };
    if (i != 9)
    {
      // Set the Highscore
      highScore[i+1] = points;
      highPlayer[i+1] = player;
    };
  };
};

/********************************************************************/
// function to determine a random and, not yet implemented, unique name
String rndName() {
  String vorName[] = {
    "Wurst",
    "Piss",
    "Bier",
    "Teddy",
    "Ochs'",
    "Arsch",
    "Sauf",
    "Haus",
    "Kack",
    "Klein",
    "Dumm",
    "Geil",
    "Blau"
  };
  String nachName[] = {
    "meister",
    "knecht",
    "kopf",
    "wurst",
    "bauer",
    "heini",
    "hans",
    "depp",
    "typ",
    "lulli",
    "baer",
    "sack",
    "peter"
  };
  int array1 = random(13);
  int array2 = random(13);
  String fullName = vorName[array1] + nachName[array2];
  return (fullName);
};
