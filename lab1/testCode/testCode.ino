#include "SevSeg.h"

SevSeg sevseg;
volatile unsigned long startTime;
unsigned long currentTime = 0;
int timer;
volatile int limit;
volatile int remainder;
volatile bool press = false;

// Ports
int button = 22;
volatile int red = 24;
int yellow = 26;
int green = 28;
int buzzer = 30;
int CA_1 = 7;
int CA_2 = 8;
int CA_3 = 9;
int CA_4 = 10;
const int STcp = 12;    //Pin connected to ST_CP of 74HC595
const int SHcp = 11;    //Pin connected to SH_CP of 74HC595
const int DS = 13;      //Pin connected to DS of 74HC595


const char* list[] = {"red", "green", "yellow"};
volatile int choice = 0;
//  DP G F E D C B A
//0: 1 1 0 0 0 0 0 0
//1: 1 1 1 1 1 0 0 1
//2: 1 0 1 0 0 1 0 0
//3: 1 0 1 1 0 0 0 0
//4: 1 0 0 1 1 0 0 1
//5: 1 0 0 1 0 0 1 0
//6: 1 0 0 0 0 0 1 0
//7: 1 1 1 1 1 0 0 0
//8: 1 0 0 0 0 0 0 0
//9: 1 0 0 1 0 0 0 0
//10:1 0 0 0 1 0 0 0
//11:1 0 0 0 0 0 1 1
//12:1 1 0 0 0 1 1 0
//13:1 0 1 0 0 0 0 1
//14:1 0 0 0 0 1 1 0
//15:1 0 0 0 1 1 1 0
byte datArray[10] {B11111100, B01100000, B11011010, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110};

// Called on boot
void setup() 
{
    // Configure ports
    pinMode(red, OUTPUT);
    pinMode(yellow, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(button, INPUT_PULLUP);
    pinMode(buzzer, OUTPUT);
    pinMode(STcp, OUTPUT);
    pinMode(SHcp, OUTPUT);
    pinMode(DS, OUTPUT);
    pinMode(CA_1, OUTPUT);
    pinMode(CA_2, OUTPUT);
    pinMode(CA_3, OUTPUT);
    pinMode(CA_4, OUTPUT);
    digitalWrite(CA_1, HIGH);
    digitalWrite(CA_2, HIGH);
    digitalWrite(CA_3, HIGH);
    digitalWrite(CA_4, HIGH);

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

// Helper fucntion to shift a number to the 7 segment display
void segwrite(int number)
{
    digitalWrite(STcp, LOW);
    shiftOut(DS, SHcp, LSBFIRST, datArray[number]);
    digitalWrite(STcp, HIGH);
}

// Function to write a one digit number to the 7 segment display
void onedigit(int which, int value)
{
    digitalWrite(which, LOW);
    segwrite(value);
    delay(500);
    digitalWrite(which,HIGH);
}

// Function to write a two digit number to the 7 segment display
void twodigit(int value)
{
    int digit0 = value / 10;
    int digit1 = value - (digit0 * 10);
    
    onedigit (CA_1,digit0);
    onedigit (CA_2,digit1);
 
}

// Main code loop
void loop() 
{
    // Check for button press
    if (digitalRead(button) == HIGH)
    {
        press = true;
    }
    // Once button is pushed, this turns into a one-way trip
    else
    {
        // Disable timer 3 interrupts, they are not needed anymore
        cli();
        TIMSK3 = 0x00;
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

// ISRs

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

    twodigit(remainder);

    // Generaly don't want prints in ISRs
    Serial.println(remainder);
}
