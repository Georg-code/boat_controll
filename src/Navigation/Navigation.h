//
// Created by georg on 20.02.2025.
//

#ifndef NAVIGATION_H
#define NAVIGATION_H
// PID
//
#include <Arduino.h>
#include <PID_v1.h>
#include <Peripherals/Position/Position.h>


class Navigation {

  static double longitude;
  static double latitude;
  static double heading;
  static double goal;
  static unsigned long lastTime;
  static double prevError;
  static double integral;
  static double currentHeading;



    double Kp = 1.0;
    double Ki = 0.0;
    double Kd = 0.0;


  public:

    Navigation(double longitude, double latitude, double heading, double goal) {
        Navigation::longitude = longitude;
        Navigation::latitude = latitude;
        Navigation::heading = heading;
        Navigation::goal = goal;

    }


    void setGoal() {

    }

    void getGoal() {

    }



  double calculateRudder(double desiredHeading) const;
};



#endif //NAVIGATION_H
