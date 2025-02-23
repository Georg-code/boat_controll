//
// Created by georg on 20.02.2025.
//

#include "Navigation.h"


double Navigation::getRudderPos(double currentHeading, double desiredHeading) const {
    double error = desiredHeading - currentHeading;
    if (error > 180) {
        error -= 360;
    } else if (error < -180) {
        error += 360;
    }

    // Compute time difference
    unsigned long now = Position::getTime();
    double deltaTime = (now - lastTime) / 1000.0; // Convert to seconds
    lastTime = Position::getTime();

    // Integral term (accumulate error over time)
    integral += error * deltaTime;

    // Derivative term (rate of change of error)
    double derivative = (deltaTime > 0) ? (error - prevError) / deltaTime : 0;

    // PID output
    double output = Kp * error + Ki * integral + Kd * derivative;

    // Store previous error
    prevError = error;

    // Constrain output (e.g., rudder range -45 to +45 degrees)
    output = constrain(output, 1, 2);

    return output; // Return rudder angle command


}
