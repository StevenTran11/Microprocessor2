// Test Arduino code for DS1307 RTC Module

#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <LiquidCrystal.h>

volatile bool timeFlag = false;     // Flag that marks when to perform a time update
// LCD module connections (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd (7, 8, 9, 10, 11, 12);

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

void print2digits(int number) 
{
  if (number >= 0 && number < 10) 
  {
    Serial.write('0');
  }
  Serial.print(number);
}

void configTimer1()
{
  // 16 MHz clock is used
  TCCR1A = 0x00;
  TCCR1B = 0x00;
  TCNT1  = 0x00;
  
  // Set clock prescaler to clk/1024
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // Set CTC mode 
  TCCR1B |= (1 << WGM12);
  // Set counter to 1Hz tick (16MHz / 1024 = 15625)
  OCR1A = 15625;
  // Enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
}

void setup()
{
  cli();
  Serial.begin(115200);
  lcd.begin(16, 2);
  configTimer1();
  Serial.println("DS1307 RTC Module Test");
  Serial.println("----------------------");
  sei();
}

void loop()
{
  tmElements_t tm;
  bool status;
  if (timeFlag)
  {
    // Perform a time read from the DS1307
    // Check if the RTC read was successful
    // If not, manually increment the time until the next read
    // Also check that the DS1307 has been properly initialized and is running
    status = RTC.read(tm);
    if(!status)
    {
      if (RTC.chipPresent())
      {
        Serial.println("The DS1307 is stopped.  Please run the SetTime");
        Serial.println("example to initialize the time and begin running.");
        Serial.println();
      } 
      else
      {
        if (tm.Second < 59)
        {
          tm.Second += 1;
        }
        else
        {
          tm.Second = 0;
          if (tm.Minute < 59)
          {
            tm.Minute += 1;
          }
          else
          {
            tm.Minute = 0;
          
            if (tm.Hour < 23)
            {
              tm.Hour += 1;
            }
            else
            {
              tm.Hour = 0;
            }
          }
        }
      }
    }
    // Reset the time update flag
    lcd.setCursor(0,0);
    lcd.print("TIME: ");
    lcd.print(tm.Hour);
    lcd.print(":");
    lcd.print(tm.Minute);
    lcd.print(":");
    if(tm.Second < 10)
      lcd.print('0');
    lcd.print(tm.Second); 
    
    lcd.setCursor(0,1);
    lcd.print("DATE: ");
    lcd.print(monthName[tm.Month - 1]);
    lcd.print(" ");
    lcd.print(tm.Day);
    lcd.print(" ");
    lcd.print(tmYearToCalendar(tm.Year));
    timeFlag = false;
  }
}
// Timer 1 ISR
// 1Hz tick used to signal a time update 
ISR(TIMER1_COMPA_vect)
{
    timeFlag = true;
}
