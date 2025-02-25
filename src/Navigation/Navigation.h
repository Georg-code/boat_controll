//
// Created by georg on 20.02.2025.
//

#ifndef NAVIGATION_H
#define NAVIGATION_H
// PID
//
#include <Arduino.h>

#include <Peripherals/Position/Position.h>

#include <Preferences.h>


#include <ArduinoEigen.h>

using namespace Eigen;



class Navigation {

  static Eigen::Vector2d position;
  static Eigen::Vector2d heading;
  static Eigen::Vector2d goal;
  static unsigned long lastTime;
  static double prevError;
  static double integral;
  static double currentHeading;
  static double rudder_position;


    double Kp = 1.0;
    double Ki = 0.0;
    double Kd = 0.0;


  public:

    explicit Navigation(double longitude = 0, double latitude = 0, double heading = 0, double goal = 0) {

    }


    static void navigation_step();


    static void setGoal(const Vector2d &goal) {

      Preferences preferences;
      preferences.putString("goal", String(goal.x()) + "," + String(goal.y()));
      Navigation::goal = goal;

    }

  static void initGoal() {

      Preferences preferences;

      if (preferences.getString("goal") != nullptr) {

          const String goal = preferences.getString("goal");

          const int comma = goal.indexOf(',');

          const double longitude = goal.substring(0, comma).toDouble();

          const double latitude = goal.substring(comma + 1).toDouble();

          Navigation::goal = Vector2d(longitude, latitude);
      } else {
        setGoal(Vector2d(0, 0));
      }

    }


    static Vector2d getGoal() {
        return goal;
    }


  double calculateRudder(Vector2d desired_heading) const;

  static Vector2d calculateNextHeading(Vector2d current, Vector2d goal, Vector2d winddir);

};





#endif //NAVIGATION_H
