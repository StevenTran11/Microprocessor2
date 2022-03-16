// Test for MPU6050 Breakout board

#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>

const int mpu6050Int    = 19;   // MPU6050 Interrupt pin
const int sda           = 20;   // I2C SDA
const int scl           = 21;   // I2C SCL

const uint8_t mpu6050Addr  = 0x69;
MPU6050 mpu(mpu6050Addr);

// For debugging
#define OUTPUT_READABLE_ACCELGYRO

void setup()
{
    pinMode(mpu6050Int, INPUT);

    Serial.begin(115200);
    Wire.begin();

    delay(250);

    Serial.println("Initializing I2C");
    mpu.initialize();

    Serial.println("Testing MPU6050");
    Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    Serial.println("Calibrating MPU6050");

    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);


//    // Wait for I2C bus to configure
//    delay(250);
//
//    // Reset the MPU6050
//    Wire.beginTransmission(mpu6050Addr);
//    Wire.write(0x6B);               // PWR_MGMT_1 register
//    Wire.write(0x80);               // Device reset
//    Wire.endTransmission(true);
//
//    // Wait 100ms for reset
//    delay(100);
//    
//    // Set power management register to defaults
//    Wire.beginTransmission(mpu6050Addr);
//    Wire.write(0x6B);               // PWR_MGMT_1 register
//    Wire.write(0x00);               // Wake up MPU6050
//    Wire.endTransmission(true);
//
//    // Set resolution for measurements
//    Wire.beginTransmission(mpu6050Addr);
//    Wire.write(0x1B);               // GYRO_CONFIG
//    Wire.write(0x18);               // Set gyro to 2000deg/s
//    Wire.write(0x18);               // Set accel to 16g   
//    Wire.endTransmission(true);
}

void loop()
{
    int16_t ax, ay, az, gx, gy, gz;

    // Read raw accel/gyro measurements from device
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

//    #ifdef OUTPUT_READABLE_ACCELGYRO
//        // display tab-separated accel/gyro x/y/z values
//        Serial.print("a/g:\t");
//        Serial.print(ax); Serial.print("\t");
//        Serial.print(ay); Serial.print("\t");
//        Serial.print(az); Serial.print("\t");
//        Serial.print(gx); Serial.print("\t");
//        Serial.print(gy); Serial.print("\t");
//        Serial.println(gz);
//    #endif
//
//    #ifdef OUTPUT_BINARY_ACCELGYRO
//        Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
//        Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
//        Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
//        Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
//        Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
//        Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
//    #endif


//    int16_t accelRaw_x, accelRaw_y, accelRaw_z;     // variables for accelerometer raw data
//    int16_t gyroRaw_x, gyroRaw_y, gyroRaw_z;        // variables for gyro raw data
//    int16_t tempRaw;                                // variables for temperature data

    float accel_x, accel_y, accel_z;                // variables for accelerometer raw data
    float gyro_x, gyro_y, gyro_z;                   // variables for gyro raw data

//    // Send I2C transaction
//    Wire.beginTransmission(mpu6050Addr);
//    Wire.write(0x3B);                               // ACCEL_XOUT_H
//    Wire.endTransmission(false);                    // Repeated start
//    Wire.requestFrom(mpu6050Addr, 7*2, 1);          // Request 14 registers
//
//    // Concatenate data
//    accelRaw_x  = Wire.read() << 8 | Wire.read();   // Read 0x3B and 0x3C, ACCEL_XOUT_*
//    accelRaw_y  = Wire.read() << 8 | Wire.read();   // Read 0x3D and 0x3E, ACCEL_YOUT_*
//    accelRaw_z  = Wire.read() << 8 | Wire.read();   // Read 0x3F and 0x40, ACCEL_ZOUT_*
//    tempRaw     = Wire.read() << 8 | Wire.read();   // Read 0x41 and 0x42, TEMP_OUT_*
//    gyroRaw_x   = Wire.read() << 8 | Wire.read();   // Read 0x41 and 0x42, GYRO_XOUT_*
//    gyroRaw_y   = Wire.read() << 8 | Wire.read();   // Read 0x43 and 0x44, GYRO_YOUT_*
//    gyroRaw_z   = Wire.read() << 8 | Wire.read();   // Read 0x45 and 0x46, GYRO_ZOUT_*
//
    // Convert data to human readable data
    accel_x = (float)ax / (float)16384;     // Units are g
    accel_y = (float)ay / (float)16384;     // Units are g
    accel_z = (float)az / (float)16384;     // Units are g
    gyro_x  = (float)gx  / (float)131;      // Units are deg/s
    gyro_y  = (float)gy  / (float)131;      // Units are deg/s
    gyro_z  = (float)gz  / (float)131;      // Units are deg/s

    Serial.print("X accel = ");
    Serial.print(accel_x);
    Serial.println();

    Serial.print("Y accel = ");
    Serial.print(accel_y);
    Serial.println();

    Serial.print("Z accel = ");
    Serial.print(accel_x);
    Serial.println();

    Serial.print("X gyro = ");
    Serial.print(gyro_x);
    Serial.println();

    Serial.print("Y gyro = ");
    Serial.print(gyro_y);
    Serial.println();

    Serial.print("Z gyro = ");
    Serial.print(gyro_z);
    Serial.println();

    delay(1000);
}
