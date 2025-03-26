#include "GPS.h"


GPS::GPS()
    : serial(2), initialized(false) {}

GPS& GPS::getInstance() {
    static GPS instance;
    return instance;
}

void GPS::begin(int rxPin, int txPin, uint32_t baud) {
    if (!initialized) {
        serial.begin(baud, SERIAL_8N1, rxPin, txPin);
        initialized = true;
    }

    Serial.println("GPS initialized");
}

void GPS::update() {
    while (serial.available() > 0) {
        gps.encode(serial.read());
    }
}

bool GPS::isLocationValid() const {
    return gps.location.isValid();
}

double GPS::getLatitude() {
    return gps.location.lat();
}

double GPS::getLongitude() {
    return gps.location.lng();
}

bool GPS::isTimeValid() const {
    return gps.time.isValid() && gps.date.isValid();
}

int GPS::getHour() {
    return gps.time.hour();
}

int GPS::getMinute() {
    return gps.time.minute();
}

int GPS::getSecond() {
    return gps.time.second();
}

int GPS::getYear() {
    return gps.date.year();
}

int GPS::getMonth() {
    return gps.date.month();
}

int GPS::getDay() {
    return gps.date.day();
}



int GPS::getKnots() {
    return gps.speed.knots();
}
int GPS::getCourse() {
    return gps.course.deg();
}

int GPS::getAltitude() {
    return gps.altitude.meters();
}

int GPS::getSatellites() {
    return gps.satellites.value();
}
