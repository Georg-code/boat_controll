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



Serial.println("ICM20948 Found!");
  // icm.setAccelRange(ICM20948_ACCEL_RANGE_16_G);
  Serial.print("Accelerometer range set to: ");
  switch (icm.getAccelRange()) {
  case ICM20948_ACCEL_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case ICM20948_ACCEL_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case ICM20948_ACCEL_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case ICM20948_ACCEL_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  Serial.println("OK");

  // icm.setGyroRange(ICM20948_GYRO_RANGE_2000_DPS);
  Serial.print("Gyro range set to: ");
  switch (icm.getGyroRange()) {
  case ICM20948_GYRO_RANGE_250_DPS:
    Serial.println("250 degrees/s");
    break;
  case ICM20948_GYRO_RANGE_500_DPS:
    Serial.println("500 degrees/s");
    break;
  case ICM20948_GYRO_RANGE_1000_DPS:
    Serial.println("1000 degrees/s");
    break;
  case ICM20948_GYRO_RANGE_2000_DPS:
    Serial.println("2000 degrees/s");
    break;
  }

  //  icm.setAccelRateDivisor(4095);
  uint16_t accel_divisor = icm.getAccelRateDivisor();
  float accel_rate = 1125 / (1.0 + accel_divisor);

  Serial.print("Accelerometer data rate divisor set to: ");
  Serial.println(accel_divisor);
  Serial.print("Accelerometer data rate (Hz) is approximately: ");
  Serial.println(accel_rate);

  //  icm.setGyroRateDivisor(255);
  uint8_t gyro_divisor = icm.getGyroRateDivisor();
  float gyro_rate = 1100 / (1.0 + gyro_divisor);

  Serial.print("Gyro data rate divisor set to: ");
  Serial.println(gyro_divisor);
  Serial.print("Gyro data rate (Hz) is approximately: ");
  Serial.println(gyro_rate);

  // icm.setMagDataRate(AK09916_MAG_DATARATE_10_HZ);
  Serial.print("Magnetometer data rate set to: ");
  switch (icm.getMagDataRate()) {
  case AK09916_MAG_DATARATE_SHUTDOWN:
    Serial.println("Shutdown");
    break;
  case AK09916_MAG_DATARATE_SINGLE:
    Serial.println("Single/One shot");
    break;
  case AK09916_MAG_DATARATE_10_HZ:
    Serial.println("10 Hz");
    break;
  case AK09916_MAG_DATARATE_20_HZ:
    Serial.println("20 Hz");
    break;
  case AK09916_MAG_DATARATE_50_HZ:
    Serial.println("50 Hz");
    break;
  case AK09916_MAG_DATARATE_100_HZ:
    Serial.println("100 Hz");
    break;
  }
  Serial.println();



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

float IMU::getTemperature() const { return temp.temperature; }


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