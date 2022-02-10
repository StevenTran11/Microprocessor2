const int red1Pin = 24; //red1 led attach to
const int yellow1Pin = 26; //yellow1 led attach to
const int green1Pin = 28; //green1 led attach to
const int button = 22;//switch attach to
const int buzzer = 30;
bool press = false;
bool next = false;
int CA_1 = 7;
int CA_2 = 8;
int CA_3 = 9;
int CA_4 = 10;
int com = 2;
const int STcp = 12;//Pin connected to ST_CP of 74HC595
const int SHcp = 11;//Pin connected to SH_CP of 74HC595
const int DS = 13; //Pin connected to DS of 74HC595
int count = 0;
int first_digit = 0;
int second_digit = 0;

//display 0,1,2,3,4,5,6,7,8,9
byte datArray[16] {B11111100, B01100000, B11011010, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110, B11101110, B00111110, B10011100, B01111010, B10011110, B10001110};

int startTime;
int limit;
int remainder;
int choice;

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
    choice = 0;
    while (true)
    {
      digitalWrite(buzzer, LOW);
      digitalWrite(yellow1Pin, LOW);
      digitalWrite(green1Pin, LOW);
      digitalWrite(red1Pin, HIGH);
      next = false;
      startTime = millis();
      while(next == false)
      {
          limit = 20000;
          remainder = (limit - (millis() - startTime)) / 1000;
          if(remainder <= 65535  && remainder >= 0)
          {
            twodigit(remainder);
          }
          if(remainder <= 3)
          {
            digitalWrite(buzzer, HIGH);
          }
      }
      digitalWrite(buzzer, LOW);
      digitalWrite(red1Pin, LOW);
      digitalWrite(green1Pin,HIGH);
      digitalWrite(yellow1Pin, LOW);
      next = false;
      startTime = millis();
      while(next == false)
      {
          limit = 20000;
          remainder = (limit - (millis() - startTime)) / 1000;
          if(remainder <= 65535 && remainder >= 0)
          {
            twodigit(remainder);
          }
          if(remainder <= 3)
          {
              digitalWrite(buzzer, HIGH);
          }
      }
      digitalWrite(buzzer, LOW);
      digitalWrite(red1Pin, LOW);
      digitalWrite(green1Pin, LOW);
      digitalWrite(yellow1Pin, HIGH);
      next = false;
      startTime = millis();
      while(next == false)
      {
          limit = 6000;
          remainder = (limit - (millis() - startTime)) / 1000;
          if(remainder <= 65535 && remainder >= 0)
          {
            twodigit(remainder);
          }
          if(remainder <= 3)
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

void onedigit(int which, int value){
  digitalWrite(which, LOW);
  segwrite(value);
  delay(10);
  digitalWrite(which,HIGH);
}

void twodigit(int value)
{
  int digit0=value/10;
  int digit1=value-(digit0 * 10);
  
  onedigit(CA_1,digit0);
  onedigit(CA_2,digit1);
}

// Timer 3 ISR
ISR(TIMER3_COMPA_vect)
{
    digitalWrite(red1Pin, !digitalRead(red1Pin));
}

// Timer 4 ISR
ISR(TIMER4_COMPA_vect)
{
    if((millis() - startTime) >= limit)
    {
      next = true;
    }
}
