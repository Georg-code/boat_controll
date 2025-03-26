//
// Created by georg on 08.02.2025.
//

#include "Windsense.h"

#include <AS5600.h>
#include <HardwareSerial.h>

AS5600 as5600;

// Static instance pointer initialization
Windsense* Windsense::instance = nullptr;

Windsense* Windsense::getInstance() {
    if (instance == nullptr) {
        instance = new Windsense();
    }
    return instance;
}

void Windsense::setup() {
    Serial.print("AS5600_LIB_VERSION: ");
    Serial.println(AS5600_LIB_VERSION);

    Wire.begin();

    as5600.begin(4);  //  set direction pin.
    as5600.setDirection(AS5600_CLOCK_WISE);  //  default, just be explicit.

    Serial.println(as5600.getAddress());

    //  as5600.setAddress(0x40);  //  AS5600L only

    const int b = as5600.isConnected();
    Serial.print("Connect: ");
    Serial.println(b);

    delay(1000);
}

int Windsense::getAngle() {
    return as5600.readAngle() * 360 / 4096;
}

int Windsense::getAvgAngle() {
    int sum = 0;
    for (int i = 0; i < 100; i++) {
        sum += as5600.readAngle();
    }



    return sum / 100;
}
