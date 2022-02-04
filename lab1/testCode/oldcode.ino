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

const char* list[] = {"red", "green", "yellow"};
int choice = 0;

void setup() 
{
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  cli();
  //set timer1 interrupt at 1Hz
  TCCR4A = 0;
  TCCR4B = 0;
  TCNT4  = 0;
  OCR4A = 15624;
  TCCR4B |= (1 << WGM12);
  TCCR4B |= (1 << CS12) | (1 << CS10);  
  TIMSK4 |= (1 << OCIE1A);
  sei();

  byte numDigits = 4;
  byte digitPins[] = {2, 3, 4, 5};
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = true; // Use 'true' if your decimal point doesn't exist or isn't connected

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
  Serial.begin(9600);
}

//timer1 interrupt 1Hz
ISR(TIMER4_COMPA_vect)
{
  if((millis() - startTime) > limit)
  {
    choice = (choice + 1) % 3;
  }
}

void loop() 
{
  if(digitalRead(button) == LOW) //check button press
  {
    press = true;
  }
  if(press == false) //red blink
  {
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
    while(true)
    {
      sei();
      digitalWrite(yellow, LOW);
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);
      startTime = millis();
      while(list[choice] =="red")
      {
        limit = 20000;
        remainder = (limit - (millis() - startTime)) / 1000;
        if(remainder <= 65535  && remainder >= 0)
        {
          Serial.println(remainder);
          sevseg.setNumber(remainder, -1, true);
        }
        if(remainder < 3)
        {
          digitalWrite(buzzer, HIGH);
        }
        sevseg.refreshDisplay();
      }
      digitalWrite(buzzer, LOW);
      digitalWrite(red, LOW);
      digitalWrite(green,HIGH);
      digitalWrite(yellow, LOW);
      startTime = millis();
      while(list[choice] == "green")
      {
        limit = 20000;
        remainder = (limit - (millis() - startTime)) / 1000;
        if(remainder <= 65535 && remainder >= 0)
        {
          Serial.println(remainder);
          sevseg.setNumber(remainder, -1, true);
        }
        if(remainder < 3)
        {
          digitalWrite(buzzer, HIGH);
        }
        sevseg.refreshDisplay();
      }
      digitalWrite(buzzer, LOW);
      digitalWrite(red, LOW);
      digitalWrite(green, LOW);
      digitalWrite(yellow, HIGH);
      startTime = millis();
      while(list[choice] == "yellow")
      {
        limit = 6000;
        remainder = (limit - (millis() - startTime)) / 1000;
        if(remainder <= 65535 && remainder >= 0)
        {
          Serial.println(remainder);
          sevseg.setNumber(remainder, -1, true);
        }
        if(remainder < 3)
        {
          digitalWrite(buzzer, HIGH);
        }
        sevseg.refreshDisplay();
      }
      digitalWrite(buzzer, LOW);
    }
  }
}
