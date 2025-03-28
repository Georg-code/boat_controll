#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <ESP32Servo.h>

class Actuator {
public:
    static Actuator& getInstance();

    void begin(int rudderPin, int flapPin);
    void setRudder(float value); // 0.0 - 1.0
    void setFlap(float value);   // 0.0 - 1.0

private:
    Actuator();
    Actuator(const Actuator&) = delete;
    Actuator& operator=(const Actuator&) = delete;

    Servo rudderServo;
    Servo flapServo;

    int mapToPulseWidth(float value); // 1000–2000 µs
};

#endif
