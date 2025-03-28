#include "Navigation.h"
#include "../Util/BoatLog.h"

#include <ArduinoEigen.h>
#include <Peripherals/Actuator/Actuator.h>
#include <Peripherals/Position/IMU.h>
#include <Peripherals/Position/Windsense.h>

#include "cmath"
#include "string"





using namespace Eigen;


double Navigation::calculateRudder(Vector2d desired_heading) {

    const double Kp = 1.0;
    const double Ki = 0.0;
    double Kd = 0.0;

// convert desired heading to degrees
    const double desired_heading_deg = std::atan2(desired_heading.x(), desired_heading.y()) * 180 / M_PI;
    const double current_heading_deg = std::atan2(headingVec.x(), headingVec.y()) * 180 / M_PI;
    double error = desired_heading_deg - current_heading_deg;
    if (error > 180) {
        error -= 360;
    } else if (error < -180) {
        error += 360;
    }

    // Compute time difference
    const unsigned long now = millis();
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

    BoatLog::navigation("Rudder angle: " + std::to_string(output));

    return output; // Return rudder angle command
}


constexpr double deg2rad(double deg) {
    return deg * M_PI / 180.0;
}



void Navigation::navigation_step() {

    // navigation data
    const Vector2d goal = getGoal();
    Navigation::goal = goal;
    const double winddir = deg2rad(Windsense::getInstance()->getAngle());
    const double currentHeading = deg2rad(IMU::getInstance().getAzimuth());

    Eigen::Rotation2Dd R(currentHeading);

    double rudder_value = 0.5;

    const Vector2d currentPos(GPSManager::getInstance().getLongitude(), GPSManager::getInstance().getLatitude());
    position = currentPos;
    const Vector2d nextOptimalHeading = calculateNextHeading(currentPos, goal, Vector2d(winddir, 0));

    const Vector2d heading = Vector2d(std::sin(currentHeading), std::cos(currentHeading));
    headingVec = heading;
    const Vector2d currrentWind = R*Vector2d(std::sin(winddir), std::cos(winddir));

    // check if the wind is coming from the front (+- 45°)
    if (std::abs(heading.dot(currrentWind)) < 0.707) {
        // find 2 possible headings and choose the one that is closer to the goal
        Vector2d option1 = R*Vector2d(std::sin(winddir + deg2rad(45)), std::cos(winddir + deg2rad(45)));
        Vector2d option2 = R*Vector2d(std::sin(winddir - deg2rad(45)), std::cos(winddir - deg2rad(45)));

        if ((goal - currentPos).dot(option1) > (goal - currentPos).dot(option2)) {
            rudder_value = calculateRudder(option1);
        } else {
            rudder_value = calculateRudder(option2);
        }
    } else {
        rudder_value = calculateRudder(nextOptimalHeading);
    }

    int flap_value = (std::abs(heading.dot(currrentWind)) < 0.707) ? 1 : 0;


    Actuator::getInstance().setRudder(rudder_value);
    Actuator::getInstance().setFlap(flap_value);












}

