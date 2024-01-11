#include <math.h>
#include <Arduino.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>
#include <cmath>
#include "BoatControl.h"
#include <QMC5883LCompass.h>
#include <Wire.h>
#include <AS5040.h>
#include <TinyGPSPlus.h>

QMC5883LCompass compass;
Servo rudder_servo;
Servo sailflap_servo;
TinyGPSPlus gps;

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

#define CSpin 33
#define CLKpin 27
#define DOpin 32

// OUTPUT PINS
#define enc_a 26
#define enc_b 34
#define enc_z 25

AS5040 enc(CLKpin, CSpin, DOpin);

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

  pinMode(BEEPER_PIN, OUTPUT);
  beep(3);
  if (!enc.begin())
    Serial.println("Error setting up AS5040");
}

void loop()

{

  while (Serial2.available() > 0)

    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
      Serial.println(F("No GPS detected: check wiring."));
      while (true)
        ;
    }

  compass.read();

  Serial.print("Winddir: ");
  Serial.println(enc.read() * 0.3515625);

  Serial2.print("Winddir: ");
  Serial2.println(enc.read() * 0.3515625);

  Serial.print("Azimuth: ");
  Serial.println(compass.getAzimuth());

  float wind_direction_raw = enc.read();
  int gps_lat, gps_lng, compass_azimuth;

  float boatcords_vec[ARRAY_SIZE] = {gps.location.lat(), gps.location.lng()};
  Serial.print("Boatcords: ");
  Serial.print(boatcords_vec[0]);
  Serial.print(", ");
  Serial.println(boatcords_vec[1]);
  Vector2D boat_vec = azimuthToVector(fmod((90 - compass.getAzimuth()), 360));

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

    rudder_servo.writeMicroseconds(rudder_pos_storage);
  }

  sailflap_servo.writeMicroseconds(sailflap_control(wind_direction_deg));
}
