
#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);
const int pinX = A0;  // Joystick X axis
const int pinY = A1;  // Joystick Y axis
const int buzzer = 7; // Buzzer for when an apple is eaten

volatile bool joyFlag = false;
volatile int joyX = 0;
volatile int joyY = 0;
double xOffset;
double yOffset;

// Global character variable to keep track of messaging direction
char currDir = 0;

// 16 MHz clock is used
void configTimer1()
{
  TCCR1A = 0x00;
  TCCR1B = 0x00;
  TCNT1  = 0x00;
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TCCR1B |= (1 << WGM12);
  OCR1A = 1562;
}
// Config Timer 2 for 61Hz interrupt
// Used for joystick polling
void configTimer2()
{
  TCCR2A  = 0x00;
  TCCR2B  = 0x00;
  TCNT2   = 0x00;
  TCCR2B  |= (1 << CS22) | (1 << CS20);
  // Enable Overflow Interrupt
  TIMSK2  = (1 << TOIE2);
}
void setup()
{
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  cli();
  configTimer1();
  configTimer2();
  sei();
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  //Calculate acceleration offset of mpu6050 to use in shaking algorithm
  xOffset = mpu6050.getAccX();
  yOffset = mpu6050.getAccY();
}

void loop() 
{
  mpu6050.update();
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
    if (((joyX < 100) && (joyY > joyX)) || (mpu6050.getAngleY() < -60 && mpu6050.getAngleX() > -20 && mpu6050.getAngleX() < 20)) 
    {
      newDir = 's';
    }
    // Up
    else if (((joyX > 950) && (joyY < joyX)) || (mpu6050.getAngleY() > 60 && mpu6050.getAngleX() > -20 && mpu6050.getAngleX() < 20))
    {
      newDir = 'w';
    }
    // Right
    else if (((joyY > 950) && (joyY > joyX)) || (mpu6050.getAngleX() > 50 && mpu6050.getAngleY() > -20 && mpu6050.getAngleY() < 20))
    {
      newDir = 'd';
    }
    // Left
    else if (((joyY < 100) && (joyY < joyX)) || (mpu6050.getAngleX() < -50 && mpu6050.getAngleY() > -20 && mpu6050.getAngleY() < 20))
    {
      newDir = 'a';
    }
    //I dont want to add delay but this j is printing multiple times.  Have to fix this part
    if(((mpu6050.getAccX() > (xOffset + 0.8)) || (mpu6050.getAccX() < (xOffset - 0.8)) || (mpu6050.getAccY() > (yOffset + 0.8)) || (mpu6050.getAccY() < (yOffset - 0.8))) && mpu6050.getAngleX() < 30 && mpu6050.getAngleX() > -30 && mpu6050.getAngleY() < 30 &&  mpu6050.getAngleY() > -30)
    {
      Serial.println('j');
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
