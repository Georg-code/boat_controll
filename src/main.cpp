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
    imu.update();

    // average of 100 samples of azimuth from IMU

    static float angle = 0;
    ble.setBatteryLevel(95);
    ble.setLocationAndSpeed(51.5074, -0.1278, 15, 1030, 9000);
    ble.setNavigation(8900, 8700, 12345);
    ble.setWind(1230, 4560, 1500);
    ble.setAccelerometer(0.01, 0.00, 9.81);
    ble.setGyroscope(0.1, 0.2, 0.3);
    ble.setMagnetometer(angle, 48.5, 1.2);
    angle += 1;
    ble.loop();







}
