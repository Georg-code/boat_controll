#ifndef IMU_H
#define IMU_H

#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>

class IMU {
public:
    static IMU& getInstance();

    bool begin();
    void update();

    // Accelerometer
    float getAccelX() const;
    float getAccelY() const;
    float getAccelZ() const;

    // Gyroscope
    float getGyroX() const;
    float getGyroY() const;
    float getGyroZ() const;

    // Magnetometer
    float getMagX() const;
    float getMagY() const;
    float getMagZ() const;

    float getAzimuth(int samples = 100);

private:
    IMU();
    IMU(const IMU&) = delete;
    IMU& operator=(const IMU&) = delete;

    Adafruit_ICM20948 icm;

    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t mag;
    sensors_event_t temp;

    bool initialized;
};

#endif
