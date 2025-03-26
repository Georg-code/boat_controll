#ifndef GPS_H
#define GPS_H

#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

class GPS {
public:
    static GPS& getInstance();

    void begin(int rxPin = 18, int txPin = 17, uint32_t baud = 9600);
    void update();

    bool isLocationValid() const;
    double getLatitude();
    double getLongitude();

    bool isTimeValid() const;
    int getHour();
    int getMinute();
    int getSecond();
    int getYear();
    int getMonth();
    int getDay();

private:
    GPS();                          // Private constructor
    GPS(const GPS&) = delete;       // Prevent copy
    GPS& operator=(const GPS&) = delete; // Prevent assignment

    TinyGPSPlus gps;
    HardwareSerial serial;
    bool initialized;
};

#endif
