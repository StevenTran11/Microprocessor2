int enB = 5;
int in3 = 3;
int in4 = 4;

void setup() {
  // Set all the motor control pins to outputs
  
  pinMode(enB, OUTPUT);
 
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Turn off motors - Initial state
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void loop() {
  directionControl();
  delay(1000);
  speedControl();
  delay(1000);
}

// This function lets you control spinning direction of motors
void directionControl() {
  // Set motors to maximum speed
  // For PWM maximum possible values are 0 to 255
  
  analogWrite(enB, 255);

  // Turn on motor A & B
  
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(2000);
  
  // Now change motor directions
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(2000);
  
  // Turn off motors
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}    
// This function lets you control speed of the motors
void speedControl() {
  // Turn on motors
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  
  // Accelerate from zero to maximum speed
  for (int i = 0; i < 256; i++) {
    
    analogWrite(enB, i);
    delay(20);
  }
  
  // Decelerate from maximum speed to zero
  for (int i = 255; i >= 0; --i) {
    
    analogWrite(enB, i);
    delay(20);
  }
  
  // Now turn off motors
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
