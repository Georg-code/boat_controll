#include "Actuator.h"

Actuator::Actuator() {}

Actuator& Actuator::getInstance() {
    static Actuator instance;
    return instance;
}

void Actuator::begin(int rudderPin, int flapPin) {
    rudderServo.setPeriodHertz(50); // Standard 50 Hz servo signal
    flapServo.setPeriodHertz(50);

    rudderServo.attach(rudderPin, 1000, 2000); // 1ms to 2ms pulse
    flapServo.attach(flapPin, 1000, 2000);
}

int Actuator::mapToPulseWidth(float value) {
    value = constrain(value, 0.0f, 1.0f);
    return 1000 + (int)(value * 1000); // 1000 to 2000 µs
}

void Actuator::setRudder(float value) {
    rudderServo.writeMicroseconds(mapToPulseWidth(value));
}

void Actuator::setFlap(float value) {
    flapServo.writeMicroseconds(mapToPulseWidth(value));
}
