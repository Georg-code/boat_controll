#include "BLEManager.h"

class BLEManager::ServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override {
        Serial.printf("Client connected: %s\n", connInfo.getAddress().toString().c_str());
    }

    void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
        Serial.println("Client disconnected. Restarting advertising...");
        NimBLEDevice::startAdvertising();
    }
};

BLEManager::BLEManager() {
    serverCallbacks = new ServerCallbacks();
}

void BLEManager::init() {
    NimBLEDevice::init("SailingDrone");
    NimBLEDevice::setSecurityAuth(BLE_SM_PAIR_AUTHREQ_SC);
    pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(serverCallbacks);

    /** Battery Service **/
    NimBLEService* battService = pServer->createService("180F");
    batteryLevelChar = battService->createCharacteristic("2A19", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
    batteryLevelChar->setValue((uint8_t)100); // Default to 100%
    battService->start();

    /** Device Information **/
    NimBLEService* deviceInfo = pServer->createService("180A");
    deviceInfo->createCharacteristic("2A29", NIMBLE_PROPERTY::READ)->setValue("SailBot Inc.");
    deviceInfo->createCharacteristic("2A24", NIMBLE_PROPERTY::READ)->setValue("Model X");
    deviceInfo->createCharacteristic("2A25", NIMBLE_PROPERTY::READ)->setValue("SN-00123");
    deviceInfo->createCharacteristic("2A26", NIMBLE_PROPERTY::READ)->setValue("v1.0.0");
    deviceInfo->start();

    /** Location and Navigation **/
    NimBLEService* locNav = pServer->createService("1819");
    locationSpeedChar = locNav->createCharacteristic("2A67", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
    navigationChar    = locNav->createCharacteristic("2A68", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
    locNav->start();

    /** Environmental Sensing **/
    NimBLEService* env = pServer->createService("181A");
    windSpeedChar = env->createCharacteristic("2A72", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
    windDirChar   = env->createCharacteristic("2A73", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
    gustChar      = env->createCharacteristic("2A74", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
    env->start();

    /** Motion Sensor Service **/
    NimBLEService* motion = pServer->createService("1814");
    accelChar = motion->createCharacteristic("2B01", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
    gyroChar  = motion->createCharacteristic("2B02", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
    motion->start();

    /** Magnetometer Service **/
    NimBLEService* mag = pServer->createService("1815");
    magChar = mag->createCharacteristic("2B03", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
    mag->start();

    /** Start advertising **/
    NimBLEAdvertising* adv = NimBLEDevice::getAdvertising();
    adv->setName("SailingDrone");
    adv->addServiceUUID("180F");
    adv->addServiceUUID("1819");
    adv->addServiceUUID("181A");
    adv->addServiceUUID("1814");
    adv->addServiceUUID("1815");
    adv->enableScanResponse(true);
    adv->start();

    Serial.println("BLE Advertising Started");
}

void BLEManager::loop() {
    delay(2000);
}

void BLEManager::notifyIfConnected(NimBLECharacteristic* chr) {
    if (pServer->getConnectedCount()) chr->notify();
}

// ========================== Setters ==========================

void BLEManager::setBatteryLevel(uint8_t percent) {
    batteryLevelChar->setValue(&percent, 1);
    notifyIfConnected(batteryLevelChar);
}

void BLEManager::setLocationAndSpeed(float lat, float lon, int16_t alt, uint16_t speed, uint16_t course) {
    uint8_t buf[14];
    writeFloatToBuf(buf, 0, lat);
    writeFloatToBuf(buf, 4, lon);
    memcpy(buf + 8, &alt, 2);
    memcpy(buf +10, &speed, 2);
    memcpy(buf +12, &course, 2);
    locationSpeedChar->setValue(buf, 14);
    notifyIfConnected(locationSpeedChar);
}

void BLEManager::setNavigation(uint16_t bearing, uint16_t heading, uint32_t distance) {
    uint8_t buf[8];
    memcpy(buf, &bearing, 2);
    memcpy(buf + 2, &heading, 2);
    memcpy(buf + 4, &distance, 4);
    navigationChar->setValue(buf, 8);
    notifyIfConnected(navigationChar);
}

void BLEManager::setWind(uint16_t speed, uint16_t direction, uint16_t gust) {
    windSpeedChar->setValue((uint8_t*)&speed, 2);    notifyIfConnected(windSpeedChar);
    windDirChar->setValue((uint8_t*)&direction, 2);  notifyIfConnected(windDirChar);
    gustChar->setValue((uint8_t*)&gust, 2);          notifyIfConnected(gustChar);
}

void BLEManager::setAccelerometer(float x, float y, float z) {
    uint8_t buf[12];
    writeFloatToBuf(buf, 0, x);
    writeFloatToBuf(buf, 4, y);
    writeFloatToBuf(buf, 8, z);
    accelChar->setValue(buf, 12);
    notifyIfConnected(accelChar);
}

void BLEManager::setGyroscope(float x, float y, float z) {
    uint8_t buf[12];
    writeFloatToBuf(buf, 0, x);
    writeFloatToBuf(buf, 4, y);
    writeFloatToBuf(buf, 8, z);
    gyroChar->setValue(buf, 12);
    notifyIfConnected(gyroChar);
}

void BLEManager::setMagnetometer(float heading, float fieldStrength, float declination) {
    uint8_t buf[12];
    writeFloatToBuf(buf, 0, heading);
    writeFloatToBuf(buf, 4, fieldStrength);
    writeFloatToBuf(buf, 8, declination);
    magChar->setValue(buf, 12);
    notifyIfConnected(magChar);
}

// ======================== Helpers ============================

void BLEManager::writeFloatToBuf(uint8_t* buf, int offset, float value) {
    memcpy(buf + offset, &value, sizeof(float));
}
