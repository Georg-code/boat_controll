#include <Arduino.h>
#include <Navigation/Navigation.h>
#include <Peripherals/Windsense/Windsense.h>
#include <Util/BoatLog.h>

void setup() {
    // initialize GPS and Compass
    Serial.begin(115200);
    Position& pos = Position::getInstance();
    pos.begin();
    BoatLog::info("Position Node Initialized");
    Navigation::initGoal();
    BoatLog::navigation("Goal was set to: ", Navigation::getGoal().x(), ", ", Navigation::getGoal().y());
}

void loop() {
    Navigation::navigation_step();

    delay(100);
}
