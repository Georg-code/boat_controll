#include "Position.h"
#include "Adafruit_ICM20948.h"
#include <ArduinoEigen.h>
// Singleton function - return reference instead of value
Position& Position::getInstance() {
    static Position instance;
    return instance;
}

// Constructor initializes software serial
Position::Position() : gpsSerial(rxPin, txPin) {
    gpsSerial.begin(baudRate);
    if (!icm.begin_I2C()) {
        Serial.println("Failed to find ICM20948 chip");
    }
}

// Start the GPS module
void Position::begin() {
    gpsSerial.begin(baudRate);
    // ICM initialization
    if (!icm.begin_I2C()) {
        Serial.println("Failed to find ICM20948 chip");
    }
    icm.setGyroRateDivisor(19);
    icm.setAccelRateDivisor(19);


}

// Read and parse GPS data
void Position::update() {
    while (gpsSerial.available() > 0) {
        gps.encode(static_cast<char>(gpsSerial.read()));
    }
}


// Check if GPS data is valid
bool Position::isValid() const {
    return gps.location.isValid();
}

// Get Latitude
double Position::getLatitude() {
    return isValid() ? gps.location.lat() : 0.0;
}

// Get Longitude
double Position::getLongitude() {
    return isValid() ? gps.location.lng() : 0.0;
}

// Get Speed in sog m/s
double Position::getSpeed() {
    return gps.speed.isValid() ? gps.speed.mps() : 0.0;
}

// Get Speed in knots
double Position::getKnots() {
    return gps.speed.isValid() ? gps.speed.knots() : 0.0;
}

// Get Heading (course over ground in degrees)
Eigen::Vector2d Position::getHeading() {
    // get ICM magnetometer data
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t mag;
    sensors_event_t temp;
    icm.getEvent(&accel, &gyro, &temp, &mag);
    // create 2D vector from magnetometer data
    Eigen::Vector2d heading;
    heading << mag.magnetic.x, mag.magnetic.y;
    return heading;
}

// Get number of satellites
unsigned long Position::getSatellites() {
    return gps.satellites.isValid() ? gps.satellites.value() : 0;
}

// Print GPS data to Serial Monitor
void Position::displayData() {
    if (isValid()) {
        Serial.print("Lat: ");
        Serial.print(getLatitude(), 6);
        Serial.print(" | Lng: ");
        Serial.print(getLongitude(), 6);
        Serial.print(" | Speed: ");
        Serial.print(getSpeed());
        Serial.print(" m/s | Heading: ");

        Serial.print("° | Satellites: ");
        Serial.println(getSatellites());
    } else {
        Serial.println("Waiting for GPS signal...");
    }
}

