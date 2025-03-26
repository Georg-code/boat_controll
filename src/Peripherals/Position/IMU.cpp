#include "IMU.h"
#include <Wire.h>
#include <math.h>

IMU::IMU() : initialized(false) {}

IMU& IMU::getInstance() {
    static IMU instance;
    return instance;
}

bool IMU::begin() {
    if (!initialized) {
        if (!icm.begin_I2C()) {
            Serial.println("Failed to find ICM20948 chip");
            return false;
        }

        Serial.println("ICM20948 initialized successfully");
        initialized = true;
    }

    return true;
}

void IMU::update() {
    if (!initialized) return;

    icm.getEvent(&accel, &gyro, &temp, &mag);
}

// Accelerometer
float IMU::getAccelX() const { return accel.acceleration.x; }
float IMU::getAccelY() const { return accel.acceleration.y; }
float IMU::getAccelZ() const { return accel.acceleration.z; }

// Gyroscope
float IMU::getGyroX() const { return gyro.gyro.x; }
float IMU::getGyroY() const { return gyro.gyro.y; }
float IMU::getGyroZ() const { return gyro.gyro.z; }

// Magnetometer
float IMU::getMagX() const { return mag.magnetic.x; }
float IMU::getMagY() const { return mag.magnetic.y; }
float IMU::getMagZ() const { return mag.magnetic.z; }


float IMU::getAzimuth(int samples) {
    if (!initialized) return -1;

    float sumSin = 0;
    float sumCos = 0;

    for (int i = 0; i < samples; ++i) {
        update(); // ✅ Get fresh sensor data

        // Get sensor data
        float ax = accel.acceleration.x;
        float ay = accel.acceleration.y;
        float az = accel.acceleration.z;
        float mx = mag.magnetic.x;
        float my = mag.magnetic.y;
        float mz = mag.magnetic.z;

        // Convert to SI units if needed (Adafruit lib usually gives m/s² already)
        float pitch = asin(-ax / 9.81);
        float roll = atan2(ay, az);

        // Tilt-compensated magnetometer
        float Xh = mx * cos(pitch) + mz * sin(pitch);
        float Yh = mx * sin(roll) * sin(pitch) + my * cos(roll) - mz * sin(roll) * cos(pitch);

        // Azimuth in radians
        float heading = atan2(Yh, Xh);

        sumSin += sin(heading);
        sumCos += cos(heading);
    }

    // Average the angle using vector mean
    float avgHeading = atan2(sumSin / samples, sumCos / samples) * 180.0 / PI;

    if (avgHeading < 0)
        avgHeading += 360.0;

    return avgHeading;
}