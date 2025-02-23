#ifndef POSITION_H
#define POSITION_H

#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <ctime>

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
    long getTime();

private:
    Position(); // Private constructor
    TinyGPSPlus gps;
    SoftwareSerial gpsSerial;

    // Delete copy constructor and assignment operator
    Position(const Position&) = delete;
    Position& operator=(const Position&) = delete;
};

#endif
