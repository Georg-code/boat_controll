#include <math.h>
#include <Arduino.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>
#include <cmath>
#include "BoatControl.h"
#include "JsonHandler.h"

Servo rudder_servo;
Servo sailflap_servo;

unsigned long lastTime;
double Input, Output, Setpoint;
double errSum, lastErr;
double kp, ki, kd;

#define ARRAY_SIZE 2

#define SERVO_RUDDER_PIN 6
#define SERVO_SAILFLAP_PIN 13

void setup()
{
  Serial.begin(9600); // Initialize serial communication

  rudder_servo.attach(6);
  sailflap_servo.attach(13);
}

void loop()

{

  if (Serial.available() > 0)
  {
    String jsonString = Serial.readStringUntil('\n');
    int gps_lat, gps_lng, wind_direction_raw, compass_azimuth;

    if (JsonHandler::parseJson(jsonString, gps_lat, gps_lng, wind_direction_raw, compass_azimuth))
    {
      float boatcords_vec[ARRAY_SIZE] = {gps_lat, gps_lng};
      // convert compass_azimuth to a vector
      Vector2D boat_vec = azimuthToVector(compass_azimuth);

      float wind_direction_vec[ARRAY_SIZE] = {0.0, 0.0};
      float wind_direction_deg = 0.0;
      float boat_heading_vec[ARRAY_SIZE] = {boat_vec.x, boat_vec.y};
      float goalcords_vec[ARRAY_SIZE] = {1.0, 1.0};

      float goal_vector[ARRAY_SIZE];
      for (int i = 0; i < ARRAY_SIZE; i++)
      {
        goal_vector[i] = goalcords_vec[i] - boatcords_vec[i];
      }

      float optimal_heading_vec[ARRAY_SIZE];
      calculate_new_heading(boat_heading_vec, wind_direction_vec, goal_vector, optimal_heading_vec);
      float output = rudder_controller(boat_heading_vec, optimal_heading_vec);
      servo_control(output, rudder_servo);
      sailflap_control(wind_direction_deg, sailflap_servo);
      delay(1000);
    }
  }
}
