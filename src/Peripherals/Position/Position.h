#ifndef POSITION_H
#define POSITION_H

#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <ctime>
#include "Adafruit_ICM20948.h"

class Position {
    int rxPin = 34;
    int txPin = 35;
    int baudRate = 9600;

public:
    // Change to return reference instead of value
    static Position& getInstance();

    void begin();
    void update();
    bool isValid() const;
    double getLatitude();
    double getLongitude();
    double getSpeed();
    double getKnots();
    double getHeading();
    unsigned long getSatellites();
    void displayData();

    double computeAzimuth();

    Position(const Position&) = delete;
    Position& operator=(const Position&) = delete;

private:
    Position(); // Private constructor
    TinyGPSPlus gps;
    SoftwareSerial gpsSerial;
    Adafruit_ICM20948 icm;



};

#endif
