#include "SevSeg.h"

SevSeg sevseg;
bool start = true;
volatile unsigned long startTime;
unsigned long currentTime = 0;
int timer;
volatile int limit;
int remainder;
volatile bool press = false;

int button = 22;
volatile int red = 24;
int yellow = 26;
int green = 28;
int buzzer = 30;

const char* list[] = {"red", "green", "yellow"};
volatile int choice = 0;

// Called on boot
void setup() 
{
    pinMode(red, OUTPUT);
    pinMode(yellow, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(button, INPUT_PULLUP);
    pinMode(buzzer, OUTPUT);

    // Clear global interrupt flag
    cli();

    // Set Timer 3 interrupt at 1 Hz
    // Used for red LED blink after boot before button has been pressed
    TCCR3A = 0;
    TCCR3B = 0;
    TCNT3  = 0;
    OCR3A = 15624;
    TCCR3B |= (1 << WGM32);
    TCCR3B |= (1 << CS32) | (1 << CS30);  
    TIMSK3 = (1 << OCIE3A);

    // Set Timer 4 interrupt at 1Hz
    TCCR4A = 0;
    TCCR4B = 0;
    //TCNT4  = 0;
    OCR4A = 15624;
    TCCR4B |= (1 << WGM42);
    TCCR4B |= (1 << CS42) | (1 << CS40);  
    TIMSK4 |= (1 << OCIE4A);

    // Set global interrupt
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

// Timer 3 ISR
ISR(TIMER3_COMPA_vect)
{
    digitalWrite(red, !digitalRead(red));
}

// Timer 4 ISR
ISR(TIMER4_COMPA_vect)
{
    if((millis() - startTime) > limit)
    {
      choice = (choice + 1) % 3;
    }
}

// Main code loop
void loop() 
{
    // Check for button press
    if (digitalRead(button) == LOW)
    {
        press = true;
    }
    // Once button is pushed, this turns into a one-way trip
    else
    {
        // Disable timer 3 interrupts, they are not needed anymore
        cli();
        TIMSK3 = 0x00;
        Serial.println("fiona");
        sei();

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
