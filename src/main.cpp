#include <math.h>
#include <Arduino.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>
#include <cmath>
#include "BoatControl.h"
#include "JsonHandler.h"
#include <QMC5883LCompass.h>
#include <Wire.h>

QMC5883LCompass compass;
Servo rudder_servo;
Servo sailflap_servo;

unsigned long lastTime;
double Input, Output, Setpoint;
double errSum, lastErr;
double kp, ki, kd;
double rudder_pos_storage;
double sailflap_pos_storage;
double wind_direction_deg;
#define ARRAY_SIZE 2

#define SERVO_RUDDER_PIN 6
#define SERVO_SAILFLAP_PIN 13
#define BEEPER_PIN 14

void beep(int times)
{
  for (int i = 0; i < times; i++)
  {
    digitalWrite(BEEPER_PIN, HIGH);
    delay(100);
    digitalWrite(BEEPER_PIN, LOW);
    delay(100);
  }
}

void setup()
{

  Wire.begin();
  Serial.begin(9600);

  Serial2.begin(9600);
  rudder_servo.attach(19);
  sailflap_servo.attach(5);
  rudder_servo.writeMicroseconds(1500);

  compass.init();
  compass.setSmoothing(10, true);

  pinMode(BEEPER_PIN, OUTPUT);
  beep(3);
}

void loop()

{

  compass.read();
  if (Serial2.available())
  {

    String jsonString = Serial2.readStringUntil('\n');
    float gps_lat, gps_lng, wind_direction_raw, compass_azimuth;
    if (JsonHandler::parseJson(jsonString, gps_lat, gps_lng, wind_direction_raw))
    {
      float boatcords_vec[ARRAY_SIZE] = {gps_lat, gps_lng};
      Vector2D boat_vec = azimuthToVector(compass.getAzimuth() + 180);

      float wind_direction_vec[ARRAY_SIZE] = {cos(wind_direction_raw * 0.3515625), sin(wind_direction_raw * 0.3515625)};

      float wind_direction_deg = wind_direction_raw * 0.3515625;

      float boat_heading_vec[ARRAY_SIZE] = {boat_vec.x, boat_vec.y};

      float goalcords_vec[ARRAY_SIZE] = {1.0, 1.0}; // TODO SET GOAL HERE

      float goal_vector[ARRAY_SIZE];
      for (int i = 0; i < ARRAY_SIZE; i++)
      {
        goal_vector[i] = goalcords_vec[i] - boatcords_vec[i];
      }

      float optimal_heading_vec[ARRAY_SIZE];
      calculate_new_heading(boat_heading_vec, wind_direction_vec, goal_vector, optimal_heading_vec);
      float output = rudder_controller(boat_heading_vec, optimal_heading_vec);
      if (servo_control(output) != rudder_pos_storage)
      {

        rudder_pos_storage = servo_control(output);
        Serial.println(rudder_pos_storage);
        rudder_servo.writeMicroseconds(rudder_pos_storage);
      }

      sailflap_servo.writeMicroseconds(sailflap_control(wind_direction_deg));
      Serial.println(wind_direction_raw);
      Serial.println(wind_direction_deg);
      Serial.println(sailflap_control(wind_direction_deg));
    }
  }
}
