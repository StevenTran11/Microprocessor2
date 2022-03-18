#include <Wire.h>

long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;



const int pinX = A0;    // Joystick X axis
const int pinY = A1;    // Joystick Y axis
//const int buzzer = 7;  // Buzzer for when an apple is eaten

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

// Config Timer 2 for 61Hz interrupt1
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
    
    //pinMode(buzzer, OUTPUT);
    //digitalWrite(buzzer, LOW);
    // Configure timers
    configTimer1();
    configTimer2();
    // Configure serial port to 115200 baud
    Serial.begin(9600);
    
    // Enable global interrupts
    sei();
    Wire.begin();
    setupMPU();
}
 
void loop()
{
    // Local character variable
    char newDir = 0;
    char shake = 0;
    recordAccelRegisters();
  
  
    recordGyroRegisters();
    if((rotX>50)||(rotY>50)||(rotZ>50)){
    shake= 'j';}
  //printData();
    Serial.print(shake);
    delay(100);
   
    // Check if an apple has been eaten by the Python script
    
    
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
        else if ( (joyX > 950) && (joyY < joyX) )
        {
            newDir = 'w';
        }
        // Right
        else if ( (joyY > 950) && (joyY > joyX) )
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



void setupMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
  Wire.endTransmission(); 
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission(); 
}

void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processAccelData();
}

void processAccelData(){
  gForceX = accelX / 16384.0;
  gForceY = accelY / 16384.0; 
  gForceZ = accelZ / 16384.0;
}

void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  while(Wire.available() < 6);
  gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  processGyroData();
}

void processGyroData() {
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0; 
  rotZ = gyroZ / 131.0;
}

void printData() {
  Serial.print("Gyro (deg)");
  Serial.print(" X=");
  Serial.print(rotX);
  Serial.print(" Y=");
  Serial.print(rotY);
  Serial.print(" Z=");
  Serial.print(rotZ);
  
  Serial.print(" Accel (g)");
  Serial.print(" X=");
  Serial.print(gForceX);
  Serial.print(" Y=");
  Serial.print(gForceY);
  Serial.print(" Z=");
  Serial.println(gForceZ);
}


// ISRs

// Timer 1 ISR
// Used to strobe the buzzer when an apple is eaten
ISR(TIMER1_COMPA_vect)
{
    // After the 100ms duration has been met,
    // disable the interrupt and turn off the buzzer
    //digitalWrite(buzzer, 0);
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
