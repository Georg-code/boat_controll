#pragma once

#include <cstdint>
#include <NimBLEDevice.h>
#include <NimBLEServer.h>          // Needed for NimBLEServer and NimBLEServerCallbacks
#include <NimBLEAdvertising.h>     // Optional, if you're dealing with advertising
#include <NimBLEService.h>         // For NimBLEService
#include <NimBLECharacteristic.h>  // For NimBLECharacteristic

struct ble_gap_conn_desc;
class NimBLEServer;
class NimBLECharacteristic;

class BLEManager {
public:
    BLEManager();

    void init();
    void loop();
    void notifyIfConnected(NimBLECharacteristic* chr);

    // ==== Setters for BLE Characteristics ====
    void setBatteryLevel(uint8_t percent);
    void setLocationAndSpeed(float lat, float lon, int16_t alt, uint16_t speed, uint16_t course, uint8_t satellites, bool gpsReady);
    void setNavigation(uint16_t bearing, uint16_t heading, uint32_t distance);
    void setWind(uint16_t speed, uint16_t direction, uint16_t gust);
    void setAccelerometer(float x, float y, float z);
    void setGyroscope(float x, float y, float z);
    void setMagnetometer(float heading, float fieldStrength, float declination);
    void setGPSReady(bool ready);
    bool isConnected() const;

private:
    // ==== Callback class ====
    class ServerCallbacks : public NimBLEServerCallbacks {
        static void onConnect(NimBLEServer* pServer);

        static void onDisconnect(NimBLEServer* pServer);
    };

    // ==== Helper ====
    static void writeFloatToBuf(uint8_t* buf, int offset, float value);

    // ==== BLE components ====
    NimBLEServer* pServer = nullptr;
    ServerCallbacks* serverCallbacks = nullptr;

    NimBLECharacteristic* batteryLevelChar = nullptr;
    NimBLECharacteristic* locationSpeedChar = nullptr;
    NimBLECharacteristic* navigationChar = nullptr;
    NimBLECharacteristic* satChar = nullptr;

    NimBLECharacteristic* windSpeedChar = nullptr;
    NimBLECharacteristic* windDirChar = nullptr;
    NimBLECharacteristic* gustChar = nullptr;

    NimBLECharacteristic* accelChar = nullptr;
    NimBLECharacteristic* gyroChar = nullptr;

    NimBLECharacteristic* magChar = nullptr;

    NimBLECharacteristic* gpsReadyChar = nullptr;

};
