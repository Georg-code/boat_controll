#include "Navigation.h"
#include "../Util/BoatLog.h"


double Navigation::calculateRudder(const double desiredHeading) const {
    currentHeading =


    double error = desiredHeading - currentHeading;
    if (error > 180) {
        error -= 360;
    } else if (error < -180) {
        error += 360;
    }

    // Compute time difference
    unsigned long now = millis();
    double deltaTime = (now - lastTime) / 1000.0;


    // Falls deltaTime negativ oder 0 ist (z. B. beim ersten Durchlauf)
    if (deltaTime <= 0) {
        deltaTime = 0.001; // Vermeidung von Division durch 0
    }

    // Integral term (accumulate error over time)
    integral += error * deltaTime;

    // Derivative term (rate of change of error)
    double derivative = (error - prevError) / deltaTime;

    // PID output
    double output = Kp * error + Ki * integral + Kd * derivative;

    // Store previous values
    prevError = error;
    lastTime = now;

    output = constrain(output, 1, 2);

    BoatLog::info("Hello");

    return output; // Return rudder angle command
}


// Set goal
