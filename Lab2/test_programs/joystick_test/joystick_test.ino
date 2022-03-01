// Test Arduino code for Joystick

const int pinX = A0; // Joystick X axis
const int pinY = A1; // Joystick Y axis

volatile bool joyFlag = false;
volatile int joyX = 0;
volatile int joyY = 0;

// Character variables to keep track of messaging direction
char currDir = 0;
char newDir = 0;

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

void setup() {
  cli();
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  configTimer2();
  Serial.begin(115200);
  sei();
}
 
void loop()
{ 
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
    if (newDir != currDir)
    {
      Serial.println(newDir);
      currDir = newDir;
    }

    // Reset the joystick update flag
    joyFlag = false;
  }
}

// ISRs

// Timer 2 ISR
ISR(TIMER2_OVF_vect)
{
  joyX = analogRead(pinX);     // Read the X axis analog value
  joyY = analogRead(pinY);     // Read the Y axis analog value

  // Mark that a joystick update should occur
  joyFlag = true;
}
