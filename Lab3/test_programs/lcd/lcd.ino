#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal.h>

RTC_DS1307 RTC;
LiquidCrystal lcd (7, 8, 9, 10, 11, 12);

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

//1Hz interrupt
void configTimer4()
{
  TCCR4A = 0;// set entire TCCR1A register to 0
  TCCR4B = 0;// same for TCCR1B
  TCNT4  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR4A = 15624/1;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR4B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR4B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK4 |= (1 << OCIE4A);
}

void setup ()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  Wire.begin();
  RTC.begin();
  if (!RTC.isrunning())
  {
    Serial.println("RTC is NOT running!");
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  cli();
  configTimer4();
  sei();
}

void loop ()
{
  DateTime now = RTC.now();
  lcd.setCursor(0,0);
  lcd.print("TIME: ");
  lcd.print(now.hour());
  lcd.print(":");
  lcd.print(now.minute());
  lcd.print(":");
  lcd.print(now.second()); 
  
  lcd.setCursor(0,1);
  lcd.print("DATE: ");
  lcd.print(monthName[now.month()]);
  lcd.print(" ");
  lcd.print(now.day());
  lcd.print(" ");
  lcd.print(now.year());
}

ISR(TIMER4_COMPA_vect)
{
  
}
