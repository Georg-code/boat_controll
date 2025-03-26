#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <Arduino.h>
#include <NimBLEDevice.h>

class BLEManager {
public:
    BLEManager();
    void init();
    void loop();

    // Sensor Data Updaters
    void setBatteryLevel(uint8_t percent);
    void setLocationAndSpeed(float lat, float lon, int16_t alt, uint16_t speed, uint16_t course);
    void setNavigation(uint16_t bearing, uint16_t heading, uint32_t distance);
    void setWind(uint16_t speed, uint16_t direction, uint16_t gust);
    void setAccelerometer(float x, float y, float z);
    void setGyroscope(float x, float y, float z);
    void setMagnetometer(float heading, float fieldStrength, float declination);

private:
    NimBLEServer* pServer;

    // Characteristics
    NimBLECharacteristic* batteryLevelChar;
    NimBLECharacteristic* locationSpeedChar;
    NimBLECharacteristic* navigationChar;
    NimBLECharacteristic* windSpeedChar;
    NimBLECharacteristic* windDirChar;
    NimBLECharacteristic* gustChar;
    NimBLECharacteristic* accelChar;
    NimBLECharacteristic* gyroChar;
    NimBLECharacteristic* magChar;

    // Helper
    void writeFloatToBuf(uint8_t* buf, int offset, float value);
    void notifyIfConnected(NimBLECharacteristic* chr);

    class ServerCallbacks;
    ServerCallbacks* serverCallbacks;
};

#endif
