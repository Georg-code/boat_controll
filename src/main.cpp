
#include <Arduino.h>
#include <ESP32Servo.h>
#include <ArduinoJson.h>
#include <cmath>

Servo rudder_servo;
Servo sailflap_servo;

#define ARRAY_SIZE 2

struct Vector2D
{
  double x;
  double y;
};

float normalize(float vector[ARRAY_SIZE])
{
  float magnitude = 0;
  for (int i = 0; i < ARRAY_SIZE; i++)
  {
    magnitude += vector[i] * vector[i];
  }
  magnitude = sqrt(magnitude);

  for (int i = 0; i < ARRAY_SIZE; i++)
  {
    vector[i] /= magnitude;
  }

  return magnitude;
}

void calculate_new_heading(float boat_vector[ARRAY_SIZE], float wind_direction_vector[ARRAY_SIZE], float goal_vector[ARRAY_SIZE], float new_heading[ARRAY_SIZE])
{
  float dot_product = 0;
  for (int i = 0; i < ARRAY_SIZE; i++)
  {
    dot_product += wind_direction_vector[i] * boat_vector[i];
  }

  for (int i = 0; i < ARRAY_SIZE; i++)
  {
    new_heading[i] = goal_vector[i];
  }

  if (dot_product < -0.8)
  {
    if (wind_direction_vector[0] * goal_vector[0] + wind_direction_vector[1] * goal_vector[1] > -0.8)
    {
      normalize(new_heading);
    }
    else
    {
      int n = 0;
      while (wind_direction_vector[0] * new_heading[0] + wind_direction_vector[1] * new_heading[1] < -0.8 && n < 1)
      {
        n += 0.1;
        float new_heading1[ARRAY_SIZE] = {goal_vector[0] + n * goal_vector[1], goal_vector[1] - n * goal_vector[0]};
        float new_heading2[ARRAY_SIZE] = {goal_vector[0] - n * goal_vector[1], goal_vector[1] + n * goal_vector[0]};

        // find which heading is closer to the goal using the dot product
        if (new_heading1[0] * goal_vector[0] + new_heading1[1] * goal_vector[1] > new_heading2[0] * goal_vector[0] + new_heading2[1] * goal_vector[1])
        {
          for (int i = 0; i < ARRAY_SIZE; i++)
          {
            new_heading[i] = new_heading1[i];
          }
        }
        else
        {
          for (int i = 0; i < ARRAY_SIZE; i++)
          {
            new_heading[i] = new_heading2[i];
          }
        }
      }
      normalize(new_heading);
    }
  }
}

float rudder_pd(float boat_heading_vec[ARRAY_SIZE], float optimal_heading_vec[ARRAY_SIZE])
{
  float p = 0.5;
  float d = 0.5;

  float error = boat_heading_vec[0] * optimal_heading_vec[0] + boat_heading_vec[1] * optimal_heading_vec[1];
  float derivative = boat_heading_vec[0] * optimal_heading_vec[0] + boat_heading_vec[1] * optimal_heading_vec[1] - error;

  float rudder_position = p * error + d * derivative + 0.5;
  // make sure rudder_position is between 0 and 1
  if (rudder_position > 1)
  {
    rudder_position = 1;
  }
  else if (rudder_position < 0)
  {
    rudder_position = 0;
  }

  return rudder_position;
}

void servo_control(float position)
{
  // write to servo
  Serial.print("Servo Position: ");
  Serial.println(position);

  // Rudder servo is a linear servo, so the position is between 0 and 1
  rudder_servo.writeMicroseconds(1000 + position * 1000);
}

void sailflap_control(float wind_direction_deg)
{
  if (wind_direction_deg > 180)
  {
    sailflap_servo.writeMicroseconds(1000);
    Serial.println("Sailflap right");
  }
  else
  {
    sailflap_servo.writeMicroseconds(2000);
    Serial.println("Sailflap left");
  }
}

Vector2D azimuthToVector(double azimuth)
{
  // Convert azimuth to radians
  double azimuthRad = azimuth * M_PI / 180.0;

  // Calculate vector components
  double x = cos(azimuthRad);
  double y = sin(azimuthRad);

  // Create and return the 2D vector
  return {x, y};
}

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
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, jsonString);

    if (error)
    {
      Serial.println("Error parsing JSON");
    }
    else
    {
      int gps_lat = doc["gps_lat"];
      int gps_lng = doc["gps_lng"];
      int wind_direction_raw = doc["wind_direction_raw"];
      int compass_azimuth = doc["compass_azimuth"];

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

      servo_control(rudder_pd(boat_heading_vec, optimal_heading_vec));
      delay(1000);
    }
  }
}
