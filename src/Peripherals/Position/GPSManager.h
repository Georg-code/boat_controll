#ifndef GPS_MANAGER_H
#define GPS_MANAGER_H

#include <TinyGPS++.h>
#include <HardwareSerial.h>

class GPSManager {
public:
    static GPSManager& getInstance();

    void begin(int rxPin, int txPin, long baud = 9600);
    void update();
    double getLatitude();
    double getLongitude() ;
    int getSatellites() ;

    bool isLocationValid() const;

    bool isLocationUpdated() const;
    double getAltitude();
    double getKnots();
    double getCourse();



private:
    GPSManager();
    ~GPSManager();
    GPSManager(const GPSManager&) = delete;
    GPSManager& operator=(const GPSManager&) = delete;

    TinyGPSPlus gps;
    HardwareSerial gpsSerial;
    bool initialized;
};

#endif
