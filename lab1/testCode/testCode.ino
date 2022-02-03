#include "SevSeg.h"
SevSeg sevseg;
bool start = true;
unsigned long startTime;
unsigned long currentTime = 0;
int timer;
int limit;
int remainder;
bool press = false;

int button = 22;
int red = 24;
int yellow = 26;
int green = 28;
int buzzer = 30;

string list = {"red", "yellow", "green"};

void setup() 
{
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  cli();//stop interrupts
  //set timer1 interrupt at 1Hz
  TCCR4A = 0;// set entire TCCR4A register to 0
  TCCR4B = 0;// same for TCCR1B
  TCNT4  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR4A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR4B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR4B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK4 |= (1 << OCIE1A);
  sei();//allow interrupts

  byte numDigits = 4;
  byte digitPins[] = {2, 3, 4, 5};
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_ANODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = true; // Use 'true' if your decimal point doesn't exist or isn't connected

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
}

//timer1 interrupt 1Hz
ISR(TIMER4_COMPA_vect)
{

}

void loop() 
{
  if(digitalRead(button) == LOW) //check button press
  {
    press = true;
  }
  if(press == false) //red blink
  {
    cli();
    limit = 1000;
    if(start == true)
    {
      startTime = currentTime;
      start = false;
    }
    currentTime = millis();
    timer = currentTime - startTime;
    
    if(timer >= limit)
    {
      start = true;
      digitalWrite(red, !digitalRead(red));
    }
  }
  else
  {
    sei();
    limit = 20000;
    startTime = millis();
    remainder = (limit - timer) / 1000;
    sevseg.setNumber(remainder, -1, true);
    sevseg.refreshDisplay();
  }
}
