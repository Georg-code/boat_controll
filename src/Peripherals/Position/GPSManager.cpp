#include "GPSManager.h"

GPSManager::GPSManager() : gpsSerial(1), initialized(false) {}

GPSManager::~GPSManager() {}

GPSManager& GPSManager::getInstance() {
    static GPSManager instance;
    return instance;
}

void GPSManager::begin(int rxPin, int txPin, long baud) {
    if (!initialized) {
        gpsSerial.begin(baud, SERIAL_8N1, rxPin, txPin);
        initialized = true;

        // Wait a bit to let GPS send some data
        delay(2000);

        Serial.println("Reading NMEA sentences from GPS:");

        int linesRead = 0;
        while (linesRead < 5) {
            if (gpsSerial.available()) {
                String line = gpsSerial.readStringUntil('\n');
                Serial.println(line);
                linesRead++;
            }
        }

        Serial.println("Finished reading sample NMEA sentences.");
    }
}

void GPSManager::update() {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
    }
}

double GPSManager::getLatitude() {
    return gps.location.isValid() ? gps.location.lat() : 0.0;
}

double GPSManager::getLongitude()  {
    return gps.location.isValid() ? gps.location.lng() : 0.0;
}

int GPSManager::getSatellites()  {
    return gps.satellites.value();
}

bool GPSManager::isLocationValid() const {
    return gps.location.isValid();
}

bool GPSManager::isLocationUpdated() const {
    return gps.location.isUpdated();
}

double GPSManager::getAltitude() {
    return gps.altitude.meters();
}

double GPSManager::getKnots() {
    return gps.speed.knots();
}

double GPSManager::getCourse() {
    return gps.course.deg();
}

