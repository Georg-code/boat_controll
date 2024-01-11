// BoatControl.cpp
#include "BoatControl.h"

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

float *calculate_new_heading(float boat_vector[ARRAY_SIZE], float wind_direction_vector[ARRAY_SIZE], float goal_vector[ARRAY_SIZE], float new_heading[ARRAY_SIZE])
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
    return new_heading;
}

float rudder_controller(float boat_heading_vec[ARRAY_SIZE], float optimal_heading_vec[ARRAY_SIZE])
{
    // Convert vectors to angles
    float boat_heading_angle = atan2(boat_heading_vec[1], boat_heading_vec[0]) * 180 / M_PI;
    float optimal_heading_angle = atan2(optimal_heading_vec[1], optimal_heading_vec[0]) * 180 / M_PI;

    

    // Calculate signed error
    float error = optimal_heading_angle - boat_heading_angle;
    // Normalize error to be between -1 and 1
    error = error / 180;
    // normalize between 0 and 1
    error = error / 2 + 0.5;

    return error;
}

float servo_control(float position)
{
    // write to servo
    
    float rudder_pos = 1000 + position * 1000;
    if (rudder_pos < 1000)
    {
        rudder_pos = 1000;
    }
    else if (rudder_pos > 2000)
    {
        rudder_pos = 2000;
    }
    // Rudder servo is a linear servo, so the position is between 0 and 1
    return (rudder_pos);
}

float sailflap_control(float wind_direction_deg)
{
    if (wind_direction_deg > 180)
    {
        return (1000);
        Serial.println("Sailflap right");
    }
    else
    {
        return (2000);
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