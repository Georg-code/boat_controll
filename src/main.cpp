#include <Arduino.h>
#include <Communication/BLEManager.h>
#include <Navigation/Navigation.h>
#include <Peripherals/Position/GPSManager.h>
#include <Peripherals/Position/IMU.h>
#include <Peripherals/Position/Windsense.h>
#include "NimBLEDevice.h"
#include <Wire.h>
#include <Peripherals/Actuator/Actuator.h>


BLEManager ble;




void setup() {

// ##### LED #####
    // BLE Status
    pinMode(32, OUTPUT);

    // ESP Status
    pinMode(19, OUTPUT);
    digitalWrite(19, HIGH);

    // GPS Status
    pinMode(23, OUTPUT);



    // ##### SENSOR #####
    Serial.begin(115200);


    GPSManager::getInstance().begin(17, 18, 9600);
    Serial.println("GPS initialized");



    Actuator::getInstance().begin(33, 25);

    ble.init();


    if (!IMU::getInstance().begin()) {
        Serial.println("IMU failed to initialize!");
        while (true);
    }

    Wire.begin();



    // setup Windsense
    Windsense::getInstance()->setup();


/* TODO TUESDAY
 * Get the GPS data from the Position class
 * Get the wind data from the Windsense class
 * Controll Rudder
 * Controll Sail
 *
 *  Determine new Course
 *  Navigate the Boat
 *
 *
 *      Add User Interface
 */









}

void loop() {
    digitalWrite(32, LOW);
    IMU& imu = IMU::getInstance();
    GPSManager& gps = GPSManager::getInstance();
    gps.update();
    Windsense& wind = *Windsense::getInstance();
    imu.update();

    // LED on if BLE is connected
    if (ble.isConnected()) {
        digitalWrite(32, HIGH);
    } else {
        digitalWrite(32, LOW);
    }


    if (gps.isLocationUpdated()) {
        digitalWrite(23, HIGH);
        ble.setLocationAndSpeed(gps.getLatitude(), gps.getLongitude(), gps.getAltitude(), gps.getKnots(), gps.getCourse(), gps.getSatellites());
    } else {
        digitalWrite(23, LOW);
    }

    ble.setNavigation(8900, gps.getCourse(), 12345);
    ble.setWind(-1, wind.getAngle(), -1);
    ble.setAccelerometer(imu.getAccelX(), imu.getAccelY(), imu.getAccelZ());
    ble.setGyroscope(imu.getGyroX(), imu.getGyroY(), imu.getGyroZ());
    ble.setMagnetometer(imu.getMagX(), gps.getSatellites(), -1);
    ble.loop();

    // Satelites


    Serial.println("Battery Management System Parameter : \n===============================================================");
    Serial.print("VBAT : "); Serial.println();

    ble.setBatteryLevel(100);







}
