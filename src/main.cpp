#include <Arduino.h>
#include <Communication/BLEManager.h>
#include <Navigation/Navigation.h>
#include <Peripherals/Position/GPS.h>
#include <Peripherals/Position/IMU.h>
#include <Peripherals/Position/Windsense.h>
#include "NimBLEDevice.h"


BLEManager ble;



void setup() {
    Serial.begin(115200);
   GPS::getInstance().begin();

    ble.init();

    // IO32 LED
    pinMode(32, OUTPUT);

    if (!IMU::getInstance().begin()) {
        Serial.println("IMU failed to initialize!");
        while (true);
    }

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
    GPS& gps = GPS::getInstance();
    Windsense& wind = *Windsense::getInstance();
    imu.update();

    // LED on if BLE is connected
    if (ble.isConnected()) {
        digitalWrite(32, HIGH);
    } else {
        digitalWrite(32, LOW);
    }


    static float angle = 0;
    ble.setBatteryLevel(95);

    if (gps.isLocationValid()) {
        ble.setGPSReady(false);
        ble.setLocationAndSpeed(gps.getLatitude(), gps.getLongitude(), gps.getAltitude(), gps.getKnots(), gps.getCourse(), gps.getSatellites(), 0);
    } else {
        ble.setGPSReady(true);
        ble.setLocationAndSpeed(0, 0, 0, 0, 0,69, 0);
    }

    ble.setNavigation(8900, gps.getCourse(), 12345);
    ble.setWind(-1, wind.getAngle(), -1);
    ble.setAccelerometer(imu.getAccelX(), imu.getAccelY(), imu.getAccelZ());
    ble.setGyroscope(imu.getGyroX(), imu.getGyroY(), imu.getGyroZ());
    ble.setMagnetometer(imu.getMagX(), gps.getSatellites(), -1);
    angle += 1;
    ble.loop();

    // Satelites
    Serial.print("Satellites: ");
    Serial.println(gps.getSatellites());








}
