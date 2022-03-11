// Lab 2

const int pinX = A0;    // Joystick X axis
const int pinY = A1;    // Joystick Y axis
const int buzzer = 50;  // Buzzer for when an apple is eaten

volatile bool joyFlag = false;
volatile int joyX = 0;
volatile int joyY = 0;

// Global character variable to keep track of messaging direction
char currDir = 0;

// Config Timer 1 for 100ms timer
// Used for strobing the buzzer
// Note! Timer 1 Interrupts are not enabled by default
// NOTE NOTE NOTE: TODO: I have tested this with a LED and its pretty faint..
// How long should the duration be for a buzzer?
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
  // Set counter to 5Hz tick (16MHz / 1024 = 15625 / 10Hz = 1563)
  OCR1A = 1562;
}

// Config Timer 2 for 61Hz interrupt
// Used for joystick polling
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

void setup() 
{
    // Clear interrupts before config
    cli();

    // Configure IO
    pinMode(pinX, INPUT);
    pinMode(pinY, INPUT);
    pinMode(buzzer, OUTPUT);
    digitalWrite(buzzer, LOW);

    // Configure timers
    configTimer1();
    configTimer2();

    // Configure serial port to 115200 baud
    Serial.begin(115200);

    // Enable global interrupts
    sei();
}
 
void loop()
{ 
    // Local character variable
    char newDir = 0;

    // Check if an apple has been eaten by the Python script
    if (Serial.available() > 0)
    {
        int incomingByte = Serial.read();
        if (incomingByte == 'E') 
        {
            cli();
            digitalWrite(buzzer, HIGH);
            TIMSK1 = (1 << OCIE1A);
            sei();
        }
    }

    // Only perform a joystick update when commanded by the Timer2 ISR
    if (joyFlag == true)
    {
        // Orientation of the joystick is set where the pins are DOWN
        // (So it can be plugged into a breadboard)
        // That sets the following:
        // UP:    X = 1023  Y = 511
        // DOWN:  X = 0     Y = 511
        // LEFT:  X = 511   Y = 0
        // RIGHT: X = 511   Y = 1023

        // Down
        if ( (joyX < 100) && (joyY > joyX) )
        {
            newDir = 's';
        }
        // Up
        else if ( (joyX > 900) && (joyY < joyX) )
        {
            newDir = 'w';
        }
        // Right
        else if ( (joyY > 900) && (joyY > joyX) )
        {
            newDir = 'd';
        }
        // Left
        else if ( (joyY < 100) && (joyY < joyX) )
        {
            newDir = 'a';
        }

        // Determine if a new direction should be sent
        if ((newDir != currDir) && (newDir != 0))
        {
            Serial.println(newDir);
            currDir = newDir;
        }

        // Reset the joystick update flag
        joyFlag = false;
    }
}

// ISRs

// Timer 1 ISR
// Used to strobe the buzzer when an apple is eaten
ISR(TIMER1_COMPA_vect)
{
    // After the 100ms duration has been met,
    // disable the interrupt and turn off the buzzer
    digitalWrite(buzzer, 0);
    TIMSK1 = 0x00;
}

// Timer 2 ISR
ISR(TIMER2_OVF_vect)
{
    joyX = analogRead(pinX);     // Read the X axis analog value
    joyY = analogRead(pinY);     // Read the Y axis analog value

    // Mark that a joystick update should occur
    joyFlag = true;
}
