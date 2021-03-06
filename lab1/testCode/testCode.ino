// Lab 1 Code

// Pin Definitions
volatile const int red1Pin = 24; //red1 led attach to
volatile const int yellow1Pin = 26; //yellow1 led attach to
const int green1Pin = 28; //green1 led attach to
const int button = 22;//switch attach to
const int buzzer = 30;
volatile const int CA_1 = 7;
volatile const int CA_2 = 8;
volatile const int CA_3 = 9;
volatile const int CA_4 = 10;
volatile const int com = 2;
volatile const int STcp = 12;//Pin connected to ST_CP of 74HC595
volatile const int SHcp = 11;//Pin connected to SH_CP of 74HC595
volatile const int DS = 13; //Pin connected to DS of 74HC595

// State Machine helper variables
volatile bool press = false;
volatile bool next = false;

// Seven Segment Display Lookup Table
//display 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F
byte datArray[16] {B11111100, B01100000, B11011010, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110, B11101110, B00111110, B10011100, B01111010, B10011110, B10001110};

// Traffic Light Definition
typedef enum 
{
  Red, 
  Green, 
  Yellow
} light_t;
volatile light_t lightState = Red;

// Timer counter
volatile int limit = 0;

// Note! Timer 1 Interrupts are not enabled by default
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
  // Set counter to 5Hz tick (16MHz / 1024 = 15625 / 5Hz = 3125)
  OCR1A = 3125;
}

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
  //set timer1 interrupt at 1Hz
  TCCR4A = 0;// set entire TCCR4A register to 0
  TCCR4B = 0;// same for TCCR1B
  TCNT4  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR4A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR4B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR4B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK4 |= (1 << OCIE1A);
}

// Helper function to update the stoplight
void lightUpdate(light_t state)
{
  if (state == Red)
  {
    digitalWrite(yellow1Pin, LOW);
    digitalWrite(green1Pin, LOW);
    digitalWrite(red1Pin, HIGH);
  }
  else if (state == Green)
  {
    digitalWrite(yellow1Pin, LOW);
    digitalWrite(green1Pin, HIGH);
    digitalWrite(red1Pin, LOW);
  }
  else if (state == Yellow)
  {
    digitalWrite(yellow1Pin, HIGH);
    digitalWrite(green1Pin, LOW);
    digitalWrite(red1Pin, LOW);
  }
}

void setup()
{
  pinMode(com, OUTPUT);
  pinMode(red1Pin, OUTPUT);  //set the redPin as an output
  pinMode(yellow1Pin, OUTPUT);  //set the yellowPin as an output
  pinMode(green1Pin, OUTPUT);  //set the greenPin as an output
  pinMode(button, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  //set pins to output
  pinMode(STcp, OUTPUT);
  pinMode(SHcp, OUTPUT);
  pinMode(DS, OUTPUT);
  pinMode(CA_1, OUTPUT);
  pinMode(CA_2, OUTPUT);
  pinMode(CA_3, OUTPUT);
  pinMode(CA_4, OUTPUT);
  digitalWrite(com, HIGH);
  digitalWrite(CA_1, HIGH);
  digitalWrite(CA_2, HIGH);
  digitalWrite(CA_3, HIGH);
  digitalWrite(CA_4, HIGH);

  //Clear global interrupt flag
  cli();

  configTimer1();
  configTimer2();
  
  //Set Timer 3 interrupt at 1 Hz
  //Used for red LED blink after boot before button has been pressed
  configTimer3();
  //Set Timer 4 interrupt at 1Hz
  //Chooses LED color
  configTimer4();
  //Set global interrupt
  sei();
  
  Serial.begin(9600);  // start serial port at 9600 bps:
}

void loop()
{
  if (digitalRead(button) == LOW) //check button press
  {
    press = true;
  }
  if (press == true)
  {
    cli();
    //Stop Timer 3 Start Timer 4
    TIMSK3 = 0x00;
    sei();
    while (true)
    {
      digitalWrite(buzzer, LOW);
      lightState = Red;
      lightUpdate(lightState);
      next = false;
      limit = 20;
      while(next == false)
      {
          Serial.println("garbage");
          if(limit <= 3)
          {
            digitalWrite(buzzer, HIGH);
          }
      }
      digitalWrite(buzzer, LOW);
      lightState = Green;
      lightUpdate(lightState);
      next = false;
      limit = 20;
      while(next == false)
      {
          Serial.println("trash");
          if(limit <= 3)
          {
              digitalWrite(buzzer, HIGH);
          }
      }
      digitalWrite(buzzer, LOW);
      lightState = Yellow;
      lightUpdate(lightState);
      next = false;
      limit = 6;
      while(next == false)
      {
          Serial.println("savemepls");
          if(limit <= 3)
          {
            digitalWrite(buzzer, HIGH);
          }
      }
    }
  }
}

void segwrite(int number){
  digitalWrite(STcp, LOW);
  shiftOut(DS, SHcp, LSBFIRST, datArray[number]);
  digitalWrite(STcp, HIGH);
}

// Function to write a one digit number to the 7 segment display
void onedigit(int which, int value)
{
    segwrite(value);
    digitalWrite(which, LOW);
    delayMicroseconds(500);
    digitalWrite(which, HIGH);
}


void twodigit(int value)
{
  int digit0=value/16;
  int digit1=value-(digit0 * 16);
  
  onedigit(CA_1,digit0);
  onedigit(CA_2,digit1);
}

// Timer 1 ISR
// Used to flash yellow LED at 5Hz
ISR(TIMER1_COMPA_vect)
{
  if (lightState == Yellow)
  {
    digitalWrite(yellow1Pin, !digitalRead(yellow1Pin)); 
  }
}

// Timer 2 ISR
// Used to strobe the 7 segment display
ISR(TIMER2_OVF_vect)
{
  if (press == true)
  {
    twodigit(limit);
  }
  else
  {
    twodigit(0);
  }
}

// Timer 3 ISR
// Used to flash red LED at start
ISR(TIMER3_COMPA_vect)
{
    digitalWrite(red1Pin, !digitalRead(red1Pin));
}

// Timer 4 ISR
// 1 second tick
ISR(TIMER4_COMPA_vect)
{
    limit--;
    if(limit == 0)
    {
      next = true;
    }

    // Enable flashing yellow light when time is less than 3
    if ( (limit <= 3) && (lightState == Yellow) )
    {
      // Enable Output Compare A Match Interrupt
      TIMSK1 = (1 << OCIE1A);
    }
    // Otherwise clear the timer1 interrupt
    else if (limit == 0)
    {
      digitalWrite(yellow1Pin, 0);
      TIMSK1 = 0x00;
    }
}
