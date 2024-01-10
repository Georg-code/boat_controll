// BoatControl.h
#ifndef BOAT_CONTROL_H
#define BOAT_CONTROL_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>

#define ARRAY_SIZE 2

struct Vector2D
{
    double x;
    double y;
};

float normalize(float vector[ARRAY_SIZE]);
float *calculate_new_heading(float boat_vector[ARRAY_SIZE], float wind_direction_vector[ARRAY_SIZE], float goal_vector[ARRAY_SIZE], float new_heading[ARRAY_SIZE]);
float rudder_controller(float boat_heading_vec[ARRAY_SIZE], float optimal_heading_vec[ARRAY_SIZE]);
void servo_control(float position, Servo rudder_servo);
void sailflap_control(float wind_direction_deg, Servo sailflap_servo);
Vector2D azimuthToVector(double azimuth);

#endif // BOAT_CONTROL_H
