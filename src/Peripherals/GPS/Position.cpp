#include "Position.h"

// Singleton function - return reference instead of value
Position& Position::getInstance() {
    static Position instance;
    return instance;
}

// Constructor initializes software serial
Position::Position() : gpsSerial(rxPin, txPin) {
    gpsSerial.begin(baudRate);
}

// Start the GPS module
void Position::begin() {
    gpsSerial.begin(baudRate);
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
double Position::getHeading() {
    return gps.course.isValid() ? gps.course.deg() : 0.0;
}

// Get number of satellites
unsigned long Position::getSatellites() {
    return gps.satellites.isValid() ? gps.satellites.value() : 0;
}

// Get current time
long Position::getTime() {
    if (gps.date.isValid() && gps.time.isValid()) {
        tm t{};
        t.tm_year = gps.date.year() - 1900;  // Years since 1900
        t.tm_mon = gps.date.month() - 1;    // Months are 0-11
        t.tm_mday = gps.date.day();
        t.tm_hour = gps.time.hour();
        t.tm_min = gps.time.minute();
        t.tm_sec = gps.time.second();
        t.tm_isdst = 0; // No daylight saving time

        return mktime(&t);  // Convert to Unix timestamp
    }
    return 0;
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
        Serial.print(getHeading());
        Serial.print("° | Satellites: ");
        Serial.println(getSatellites());
    } else {
        Serial.println("Waiting for GPS signal...");
    }
}
