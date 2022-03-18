#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);
const int pinX = A0;  // Joystick X axis
const int pinY = A1;  // Joystick Y axis
const int buzzer = 7; // Buzzer for when an apple is eaten
//Joystick Values
volatile int joyX = 0;
volatile int joyY = 0;
//Acceration Offset of Gyroscope
double xOffset;
double yOffset;
//Determine whether to change color to golden
bool golden = false;
//Buzzer Start Time
unsigned long start_Buzzer;

// Global character variable to keep track of messaging direction
char currDir = 0;
char newDir = 0;

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
void setup()
{
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  cli();
  configTimer1();
  sei();
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  //Calculate acceleration offset of mpu6050
  xOffset = mpu6050.getAccX();
  yOffset = mpu6050.getAccY();
}

void loop() 
{
  mpu6050.update();
  //Check if an apple has been eaten by the Python script
  if (Serial.available() > 0)
  {
    int incomingByte = Serial.read();
    if (incomingByte == 'E') 
    {
      golden = false;
      start_Buzzer = millis();
      cli();
      digitalWrite(buzzer, HIGH);
      TIMSK1 = (1 << OCIE1A);
      sei();
    }
  }
  joyX = analogRead(pinX);     // Read the X axis analog value
  joyY = analogRead(pinY);     // Read the Y axis analog value
  
  // Orientation of the joystick is set where the pins are DOWN
  // Down
  if (((joyX < 150) && (joyY > joyX)) || (mpu6050.getAngleY() < -60 && mpu6050.getAngleX() > -20 && mpu6050.getAngleX() < 20)) 
  {
    newDir = 's';
  }
  // Up
  else if (((joyX > 900) && (joyY < joyX)) || (mpu6050.getAngleY() > 60 && mpu6050.getAngleX() > -20 && mpu6050.getAngleX() < 20))
  {
    newDir = 'w';
  }
  // Right
  else if (((joyY > 900) && (joyY > joyX)) || (mpu6050.getAngleX() > 50 && mpu6050.getAngleY() > -20 && mpu6050.getAngleY() < 20))
  {
    newDir = 'd';
  }
  // Left
  else if (((joyY < 150) && (joyY < joyX)) || (mpu6050.getAngleX() < -50 && mpu6050.getAngleY() > -20 && mpu6050.getAngleY() < 20))
  {
    newDir = 'a';
  }
  //Print J if not golden and gyroscope is shaking and not tilted
  if(((mpu6050.getAccX() > (xOffset + 1.5)) || (mpu6050.getAccX() < (xOffset - 1.5)) || (mpu6050.getAccY() > (yOffset + 1.5)) || (mpu6050.getAccY() < (yOffset - 1.5))) && mpu6050.getAngleX() < 30 && mpu6050.getAngleX() > -30 && mpu6050.getAngleY() < 30 &&  mpu6050.getAngleY() > -30 && golden == false)
  {
    Serial.println('j');
    golden = true;
  }
  // Determine if a new direction should be sent
  if ((newDir != currDir) && (newDir != 0))
  {
    Serial.println(newDir);
    currDir = newDir;
  }
}

ISR(TIMER1_COMPA_vect)
{
  // After the 100ms duration has been met,
  // disable the interrupt and turn off the buzzer
  if(millis() - start_Buzzer > 20)
  {
    digitalWrite(buzzer, LOW);
    TIMSK1 = 0x00;
  }
}
