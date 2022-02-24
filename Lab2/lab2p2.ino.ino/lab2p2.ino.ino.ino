#include <MPU6050_tockn.h>
#include <Wire.h>
MPU6050 mpu(Wire);
const int pinX = A0; // Joystick X axis
const int pinY = A1; // Joystick Y axis
const int buzzer = 7;

void setup() {
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  mpu.begin();
  mpu.calcGyroOffsets(true);
}
void loop()
{
  mpu.update();
  digitalWrite(buzzer, LOW);
  if (Serial.available() > 0)
  {
      int incomingByte = Serial.read();
      if(incomingByte == 'E') 
      {
        digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(buzzer, LOW);
      }
  }
  if ((analogRead(pinX) < 100 && analogRead(pinX) < analogRead(pinY))||(mpu.getAccAngleX() < -50 && mpu.getAccAngleY() > -20 && mpu.getAccAngleY() < 20))
  {
    Serial.println('a');
    delay(100);
  }
  else if ((analogRead(pinX) > 1000 && analogRead(pinX) > analogRead(pinY))||(mpu.getAccAngleX() > 50 && mpu.getAccAngleY() > -20 && mpu.getAccAngleY() < 20))
  {
      Serial.println('d');
      delay(100);
  }
  else if ((analogRead(pinY) > 1000 && analogRead(pinY) > analogRead(pinX))||(mpu.getAccAngleY() < -60 && mpu.getAccAngleX() > -20 && mpu.getAccAngleX() < 20))
  {
      Serial.println('s');
      delay(100);
  }
  else if ((analogRead(pinY) < 100 && analogRead(pinY) < analogRead(pinX))||(mpu.getAccAngleY() > 60 && mpu.getAccAngleX() > -20 && mpu.getAccAngleX() < 20))
  {
      Serial.println('w');
      delay(100);
  }
}
