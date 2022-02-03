#include "SevSeg.h"
SevSeg sevseg; //Instantiate a seven segment object
bool start = true;
unsigned long startTime;
unsigned long currentTime = 0;
int timer;
int limit;

void setup()
{
  byte numDigits = 2;
  byte digitPins[] = {2, 3, 4, 5};
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_ANODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = true; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
  Serial.begin(9600);
}

void loop()
{
  limit = 20000;
  if(start == true)
  {
    startTime = currentTime;
    start = false;
  }
  currentTime = millis();
  timer = currentTime - startTime;
  Serial.println(timer);
  if(timer >= limit)
  {
    start = true;
  }
  //sevseg.setNumber(timer % 100, -1, true); // Displays '3.141'
  //sevseg.refreshDisplay();
}
