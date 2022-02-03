#include "SevSeg.h"
SevSeg sevseg;
bool start = true;
unsigned long startTime;
unsigned long currentTime = 0;
int timer;
int limit;
bool press = false;

int button = 22;
int red = 24;
int yellow = 26;
int green = 28;
int buzzer = 30;

void setup() 
{
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
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
    
    sevseg.setNumber(20, -1, true);
    sevseg.refreshDisplay();
  }
}
/// END ///
