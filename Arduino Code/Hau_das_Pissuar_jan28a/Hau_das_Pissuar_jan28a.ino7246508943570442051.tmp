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
volatile uint32_t lastflowratetimer = 10;
// and use that to calculate a flow rate
volatile float timer;

// Declare global variables
bool session; // describes if a session is activ or not
String vollname;

int highScore[10] = {}; //stores the highscore
String highPlayer[10] = {}; //stores the highscore

// Calculates miliLiters from the pulses
float mliters = 0;

// Interrupt is called once a millisecond, looks for any pulses from the sensor!
SIGNAL(TIMER0_COMPA_vect) {
  uint8_t x = digitalRead(FLOWSENSORPIN);
  
  if (x == lastflowpinstate) {
    //lastflowratetimer++;
    return; // nothing changed!
  }
  
  if (x == HIGH) {
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
};

void loop()             // run over and over again
{  
  //counts the time since the last signal
  lastflowratetimer++;
  timer = (lastflowratetimer/10);  // in seconds

   if (timer == 0)                //Start a session
   {
    gameStart();
   } 
   else if (timer == 5)           // End a session
   {    
      // save the score to the highscore array
      setHighscore(vollname, mliters);
      session = false;
      pulses = 0;
   } 
   else if (timer > 5)            // Starts the Game else stays in Idle mode
   {
    IdleMode();
   }
  
   delay(100);
};

void IdleMode(){
  showHigh();
  lcd.setCursor(0,0);
  lcd.print("Idle Time: "); lcd.print( timer ); lcd.print("      ");
};

void showHigh() 
{
  int i = 0;
  lcd.setCursor(0,1);
  lcd.print("#"+(i+1)+highPlayer[i]+" - "+highScore[i]);
  //i++;
  return;
};

void gameStart()
{
  mliters = pulses *2.556;
  if (session == false)
  {
    vollname = name();
    session = true;
  };
  printLCD(mliters,vollname);
  return;
};

void printLCD(int ergebnis, String vollname)
{ 
  lcd.setCursor(0,0);
  lcd.print(vollname);
  lcd.setCursor(10,0);
  lcd.print(ergebnis); 
  lcd.print(" ml  ");
  delay(100);
  return;
};

void setHighscore(String player, int points)
{
  // http://forum.arduino.cc/index.php?topic=72823.0
  // http://playground.arduino.cc/Code/QueueList
  if (points > highScore[0])
  {
    highScore[0] = points;
    highPlayer[0] = player;
  }
  else if (points > highScore[1])
  {
    highScore[1] = points;
    highPlayer[1] = player;
  }
  else if (points > highScore[2])
  {
    highScore[2] = points;
    highPlayer[2] = player;
  };
}

  // function to determine a random and, not yet implemented, unique name
String name() {
  String vorName[] ={
        "Wurst",
        "Piss",
        "Bier",
        "Teddy",
        "Ochs'",
        "Arsch",
        "Sauf",
        "Haus",
        "Kack",
        "Klein"
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
        "dirne",
        "bär"
      };
  int array1 = random(10);
  int array2 = random(10);
  String vollname = vorName[array1]+nachName[array2];
  return (vollname);

};
