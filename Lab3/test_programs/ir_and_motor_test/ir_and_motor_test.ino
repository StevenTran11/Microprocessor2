// Test Arduino code for IR receiver

#include <IRremote.h>

const int ir_recv_pin = 40;     // Connected to digital signal pin on IR diode

// Motor pins
const int enB = 5;
const int in3 = 3;
const int in4 = 4;

// Global variables for fan info
// Note: Fan speeds are non-linear - Minimum speed appears to be around 128
// 128 - 255 will be the valid range
int fanSpeed = 255;
// Fan Direction Clockwise
bool fanDirCw = true;

void fanIncreaseSpeed()
{
    switch (fanSpeed)
    {
        // 0% up to 25%
        case 0:
            fanSpeed = 128;
            analogWrite(enB, fanSpeed);
            break;

        // 25% up to 50%
        case 128:
            fanSpeed = 170;
            analogWrite(enB, fanSpeed);
            break;

        // 50% up to 75%
        case 170: 
            fanSpeed = 212;
            analogWrite(enB, fanSpeed);
            break;

        // 75% up to 100%
        case 212:
            fanSpeed = 255;
            analogWrite(enB, fanSpeed);
            break;

        // Max speed
        case 255:
            fanSpeed = 255;
            break;

        // Catch all, reset to 50%
        default:
            fanSpeed = 170;
            break;
    }
}

void fanDecreaseSpeed()
{
    switch (fanSpeed)
    {
        // Off
        case 0:
            fanSpeed = 0;
            break;

        // 25% down to 0%
        case 128:
            fanSpeed = 0;
            analogWrite(enB, fanSpeed);
            break;

        // 50% down to 25%
        case 170:
            fanSpeed = 128;
            analogWrite(enB, fanSpeed);
            break;

        // 75% down to 50%
        case 212: 
            fanSpeed = 170;
            analogWrite(enB, fanSpeed);
            break;

        // 100% down to 75%
        case 255:
            fanSpeed = 212;
            analogWrite(enB, fanSpeed);
            break;

        // Catch all, reset to 50%
        default:
            fanSpeed = 170;
            break;
    }
}

void fanChangeDirection()
{
    int i;

    // Change clockwise to counter clockwise
    if (fanDirCw)
    {
        // Need to stop the motor first to be safe
        for (i = fanSpeed; i > 0; i--)
        {
            analogWrite(enB, i);
            delay(2);
        }

        // Switch motor directions
        analogWrite(enB, 0);
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
        delay(5);

        // Now spool motor back up
        for (i = 0; i < fanSpeed; i++)
        {
            analogWrite(enB, i);
            delay(2);
        }

        fanDirCw = false;
    }
    else
    {
        // Need to stop the motor first to be safe
        for (i = fanSpeed; i > 0; i--)
        {
            analogWrite(enB, i);
            delay(2);
        }

        // Switch motor directions
        analogWrite(enB, 0);
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        delay(5);

        // Now spool motor back up
        for (i = 0; i < fanSpeed; i++)
        {
            analogWrite(enB, i);
            delay(2);
        }

        fanDirCw = true;
    }
}

void setup()
{
    Serial.begin(115200);

    // Set all the motor control pins to outputs 
    pinMode(enB, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
  
    // Set motor to clockwise, max speed by default
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enB, fanSpeed);

    // Enable IR receiver
    IrReceiver.begin(ir_recv_pin, ENABLE_LED_FEEDBACK);
}

void loop()
{
    if (IrReceiver.decode())
    {
        switch(IrReceiver.decodedIRData.command)
        {
            // Can be used to lower fan speed
            case 0x07:
                fanDecreaseSpeed();
                Serial.print("Fan speed DOWN = ");
                Serial.println(fanSpeed);
                break;

            // Can be used to raise fan speed
            case 0x09:
                fanIncreaseSpeed();
                Serial.print("Fan speed UP = ");
                Serial.println(fanSpeed);
                break;

            // Can be used to switch fan direction
            case 0x43:
                Serial.println("Changing fan direction");
                fanChangeDirection();
                Serial.println("Fan direction changed");
                break;

            // Can also be used to switch fan direction
            case 0x44:
                Serial.println("Changing fan direction");
                fanChangeDirection();
                Serial.println("Fan direction changed");
                break;

            // Catch all - do nothing
            default:
              Serial.println("Other");
              break;
        }
        Serial.println();

        // Perform a short delay to "debounce" the input
        delay(100);

        IrReceiver.resume();
    }
}

// FOR REFERENCE 
//
//    BUTTON    | RAW DATA  | decodedIrData.command
//--------------|-----------|-------------------------
//    Power     | BA45FF00  |           0x45
//    Vol+      | B946FF00  |           0x46
//    Vol-      | EA15FF00  |           0x15
//  Fast Back   | BB44FF00  |           0x44
//  Play/Pause  | BF40FF00  |           0x40
//  Fast Forw.  | BC43FF00  |           0x43
//    Down      | F807FF00  |           0x07
//     Up       | F609FF00  |           0x09
//    Func      | B847FF00  |           0x47
//     EQ       | E619FF00  |           0x19
//  St/Rept     | F20DFF00  |           0x0D
//      0       | E916FF00  |           0x16
//      1       | F30CFF00  |           0x0C
//      2       | E718FF00  |           0x18
//      3       | A15EFF00  |           0x5E
//      4       | F708FF00  |           0x08
//      5       | E31CFF00  |           0x1C
//      6       | A55AFF00  |           0x5A
//      7       | BD42FF00  |           0x42
//      8       | AD52FF00  |           0x52
//      9       | B54AFF00  |           0x4A
