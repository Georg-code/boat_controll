#include "Navigation.h"
#include "../Util/BoatLog.h"
/*
#include <ArduinoEigen.h>
#include <Peripherals/Windsense/Windsense.h>

#include "cmath"
#include "string"



using namespace Eigen;

// Calculate the rudder angle based on the current heading and the desired heading
double Navigation::calculateRudder(Vector2d desired_heading) const {

    const double Kp = 1.0;
    const double Ki = 0.0;
    double Kd = 0.0;

// convert desired heading to degrees
    const double desired_heading_deg = atan2(desired_heading.y(), desired_heading.x()) * 180 / M_PI;

    double error = desired_heading_deg - currentHeading;
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


// Calculate the next heading based on the current position, goal and wind direction
static Vector2d calculateNextHeading(const Vector2d& current, const Vector2d& goal, const Vector2d& winddir) {
    constexpr double extra_rotation = 8;


    Vector2d toGoal = (goal - current).normalized();
    Vector2d toWind = (winddir - current).normalized();

    // angle between toGoal and toWind
    const double angle = atan2(toGoal.y(), toGoal.x()) - atan2(toWind.y(), toWind.x());
    if (angle > M_PI / 4) {
      return toGoal;
    }

    const Matrix2d transform_A = Eigen::Rotation2Dd(45 + extra_rotation).toRotationMatrix();
    const Matrix2d transform_B = Eigen::Rotation2Dd(-(45 + extra_rotation)).toRotationMatrix();

    Vector2d heading_A = transform_A * toWind;
    Vector2d heading_B = transform_B * toWind;


    // angle between Heading_A and toGoal
    const double angle_A = atan2(heading_A.y(), heading_A.x()) - atan2(toGoal.y(), toGoal.x());
    // angle between Heading_B and toGoal
    const double angle_B = atan2(heading_B.y(), heading_B.x()) - atan2(toGoal.y(), toGoal.x());

    if (angle_A < angle_B) {
        return heading_A;
    }
    return heading_B;
}


void Navigation::navigation_step() {

    // navigation data
    const Vector2d goal = getGoal();
    const Vector2d winddir = Windsense::getWindDirection();
    const Vector2d currentHeading = Position::getInstance().getHeading();

    // TODO: rotate wind vector relative to boat
    Vector2d heading = calculateNextHeading(currentHeading, goal, winddir);

    // TODO set the rudder



    // TODO sailwinf logic


}

*/