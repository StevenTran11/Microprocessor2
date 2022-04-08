// Test Arduino code for DS1307 RTC Module

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

const int led = 50;                 // Test pin for LED

volatile bool timeFlag = false;     // Flag that marks when to perform a time update

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
  delay(250);
  pinMode(led, OUTPUT);
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
  status = RTC.read(tm);
  // Check if the RTC read was successful
  // If not, manually increment the time until the next read
  // Also check that the DS1307 has been properly initialized and is running
  if (!status)
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
    // TODO: This would be replaced with some servo logic
    // For now, just testing with a simple LED
    if (tm.Second < 30)
    {
      digitalWrite(led, HIGH);
    }
    else
    {
      digitalWrite(led, LOW);
    }
    Serial.print("Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
    // Reset the time update flag
    timeFlag = false;
  }
}

// Timer 1 ISR
// 1Hz tick used to signal a time update 
ISR(TIMER1_COMPA_vect)
{
    timeFlag = true;
}
