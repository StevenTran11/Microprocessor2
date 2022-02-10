// Lab 1 Assignment

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
const int CA_1 = 7;
const int CA_2 = 8;
const int CA_3 = 9;
const int CA_4 = 10;
const int SRCLK = 11;   // Pin connected to SRCLK/ST_CP of 74HC595. Shift register clock
const int RCLK = 12;    // Pin connected to RCLK/SH_CP of 74HC595. Storage register clock
const int SER = 13;     // Pin connected to SER/DS of 74HC595. Data line


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

void configTimer2()
{
    // 16 MHz clock is used
    TCCR2A  = 0x00;
    TCCR2B  = 0x00;
    TCNT2   = 0x00;

    // Set clock prescaler to clk/1024
    // Set normal mode (interrupt at MAX)
    // Frequency = 15625 / 255 = 61.27 Hz
    TCCR2B  |= (1 << CS22) | (1 << CS20);
    // Enable Overflow Interrupt
    TIMSK2  = (1 << TOIE2);
}   

void configTimer3()
{
    // 16 MHz clock is used
    TCCR3A = 0x00;
    TCCR3B = 0x00;
    TCNT3  = 0x00;
    
    // Set clock prescaler to clk/1024
    TCCR3B |= (1 << CS32) | (1 << CS30);
    // Set CTC mode 
    TCCR3B |= (1 << WGM32);
    // Set counter to 1Hz tick (16MHz / 1024 = 15625)
    OCR3A = 15625;
    // Enable Output Compare A Match Interrupt
    TIMSK3 = (1 << OCIE3A);
}

void configTimer4()
{
    // 16 MHz clock is used
    TCCR4A = 0x00;
    TCCR4B = 0x00;
    TCNT4  = 0x00;
    
    // Set clock prescaler to clk/1024
    TCCR4B |= (1 << CS42) | (1 << CS40);
    // Set CTC mode 
    TCCR4B |= (1 << WGM42);
    // Set counter to 1Hz tick (16MHz / 1024 = 15625)
    OCR4A = 15625;
    // Enable Output Compare A Match Interrupt
    TIMSK4 = (1 << OCIE4A);
}

// Called on boot
void setup() 
{
    // Configure ports
    pinMode(red, OUTPUT);
    pinMode(yellow, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(button, INPUT_PULLUP);
    pinMode(buzzer, OUTPUT);
    pinMode(SRCLK, OUTPUT);
    pinMode(RCLK, OUTPUT);
    pinMode(SER, OUTPUT);
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

    configTimer2();

    // Set Timer 3 interrupt at 1 Hz
    // Used for red LED blink after boot before button has been pressed
    configTimer3();

    // Set Timer 4 interrupt at 1Hz
    configTimer4();

    // Set global interrupt
    sei();

    // Enable serial port
    Serial.begin(9600);
}

// Helper fucntion to shift a number to the 7 segment display
void segwrite(int number)
{
    digitalWrite(RCLK, LOW);
    shiftOut(SER, SRCLK, LSBFIRST, datArray[number]);
    digitalWrite(RCLK, HIGH);
}

// Function to write a one digit number to the 7 segment display
void onedigit(int which, int value)
{
    digitalWrite(which, LOW);
    segwrite(value);
    //delay(50);
    digitalWrite(which, HIGH);
}

// Function to write a two digit number to the 7 segment display
void twodigit(int value)
{
    int digit0 = value / 10;
    int digit1 = value - (digit0 * 10);
    
    onedigit (CA_4,digit0);
    onedigit (CA_3,digit1);
 
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
                  //sevseg.setNumber(remainder, -1, true);
                }

                if(remainder < 3)
                {
                  digitalWrite(buzzer, HIGH);
                }

                //sevseg.refreshDisplay();
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
                    //sevseg.setNumber(remainder, -1, true);
                }

                if(remainder < 3)
                {
                    digitalWrite(buzzer, HIGH);
                }

                //sevseg.refreshDisplay();
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
                  //sevseg.setNumber(remainder, -1, true);
                }

                if(remainder < 3)
                {
                  digitalWrite(buzzer, HIGH);
                }

                //sevseg.refreshDisplay();
            }

            digitalWrite(buzzer, LOW);
        }
    }
}

// ISRs

// Timer 2 ISR
ISR(TIMER2_OVF_vect)
{
    onedigit(CA_1, 1);
    onedigit(CA_2, 2);
    onedigit(CA_3, 3);
    onedigit(CA_4, 4);
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

    //twodigit(remainder);
    onedigit(CA_4, remainder % 10);

    // Generaly don't want prints in ISRs
    Serial.println(remainder);
}
