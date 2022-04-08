#include <Wire.h>
#include <LiquidCrystal.h>
#include <DS1307RTC.h>
#include <Time.h>

LiquidCrystal lcd (7, 8, 9, 10, 11, 12);
tmElements_t tm;

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
  cli();
  configTimer4();
  sei();
}

void loop ()
{
  
  if (RTC.read(tm))
  {
    lcd.setCursor(0,0);
    lcd.print("TIME: ");
    lcd.print(tm.Hour);
    lcd.print(":");
    lcd.print(tm.Minute);
    lcd.print(":");
    lcd.print(tm.Second); 
    
    lcd.setCursor(0,1);
    lcd.print("DATE: ");
    lcd.print(monthName[tm.Month]);
    lcd.print(" ");
    lcd.print(tm.Day);
    lcd.print(" ");
    lcd.print(tmYearToCalendar(tm.Year));
  }
  else
  {
    if (RTC.chipPresent())
    {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } 
    else
    {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
  }
}

ISR(TIMER4_COMPA_vect)
{
  
}
