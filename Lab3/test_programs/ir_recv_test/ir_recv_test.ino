// Test Arduino code for IR receiver

#include <IRremote.h>

const int ir_recv_pin = 40;     // Connected to digital signal pin on IR diode

void setup()
{
    Serial.begin(115200);
    IrReceiver.begin(ir_recv_pin, ENABLE_LED_FEEDBACK);
}

void loop()
{
    if (IrReceiver.decode())
    {
        //Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
        //IrReceiver.printIRResultShort(&Serial);

        switch(IrReceiver.decodedIRData.command)
        {
            // Can be used to lower fan speed
            case 0x07:
                Serial.println("DOWN");
                break;

            // Can be used to raise fan speed
            case 0x09:
                Serial.println("UP");
                break;

            // Can be used to switch fan direction
            case 0x43:
                Serial.println("Fast forward");
                break;

            // Can also be used to switch fan direction
            case 0x44:
                Serial.println("Fast back");
                break;

            // Catch all - do nothing
            default:
              Serial.println("Other");
              break;
        }

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
