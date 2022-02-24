const int pinX = A0; // Joystick X axis
const int pinY = A1; // Joystick Y axis
const int buzzer = 7; // digital pin connected to switch output

void setup() {
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}
 
void loop()
{
  int X = analogRead(pinX); // Read the X axis analog value
  int Y = analogRead(pinY); // Read the Y axis analog value
  digitalWrite(7, LOW);
  if (Serial.available() > 0)
  {
      int incomingByte = Serial.read();
      Serial.println(incomingByte);
      if(incomingByte == 'E') 
      {
        digitalWrite(7, HIGH);
        delay(100);
        digitalWrite(7, LOW);
      }
  }
  if (analogRead(pinY) > 1000 && analogRead(pinY) > analogRead(pinX)) 
  {
      Serial.println('s');
      delay(100);
  }
  else if (analogRead(pinY) < 100 && analogRead(pinY) < analogRead(pinX))
  {
      Serial.println('w');
      delay(100);
  }
  else if (analogRead(pinX) > 1000 && analogRead(pinX) > analogRead(pinY))
  {
      Serial.println('d');
      delay(100);
  }
  else if (analogRead(pinX) < 100 && analogRead(pinX) < analogRead(pinY))
  {
      Serial.println('a');
      delay(100);
  }
}
