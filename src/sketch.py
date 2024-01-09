import numpy as np
import math


def normalize(vector):
    return vector / np.linalg.norm(vector)

def calculate_new_heading(boat_vector, wind_direction_vec_vector, goal_vector):
    dot_product = np.dot(wind_direction_vector, boat_vector)

    new_heading = goal_vector
    if np.any(dot_product < -0.8):
        if np.dot(wind_direction_vector, goal_vector) > -0.8:
            new_heading = goal_vector
            return normalize(new_heading)
        n = 0
        while np.dot(wind_direction_vector, new_heading) < -0.8 and n < 1:         
            n += 0.1
            new_heading1 = np.add(goal_vector, n * np.array([goal_vector[1], -goal_vector[0]]))
            new_heading2 = np.add(goal_vector, n * np.array([-goal_vector[1], goal_vector[0]]))

            # find which heading is closer to the goal using the dot product
            if np.dot(new_heading1, goal_vector) > np.dot(new_heading2, goal_vector):
                new_heading = new_heading1
            else:
                new_heading = new_heading2
        return normalize(new_heading)



def rudder_pd(boat_heading_vec, optimal_heading_vec):
    # Used is a linerar servo. 0.5 is the middle position of the servo and means boat and optimal heading are the same; 0 means full right, 1 means full left: Write a pd controller for the rudder
    # return a value between 0 and 1 for the servo position
    p = 0.5
    d = 0.5
    error = np.dot(boat_heading_vec, optimal_heading_vec)
    derivative = np.dot(boat_heading_vec, optimal_heading_vec) - error

    rudder_position = p * error + d * derivative + 0.5
    # make sure rudder_position is between 0 and 1
    if rudder_position > 1:
        rudder_position = 1
    elif rudder_position < 0:
        rudder_position = 0
    
    return rudder_position

    


def servo_control(position):
    # write to servo
    print(position)


def sailflap_control(wind_direction_deg):
    if wind_direction_deg > 180:
        print("sailflap right")
    else:
        print("sailflap left")
    


def main():
    boatcords_vec = np.array([0.0, 0.0]) # read out of json
    wind_direction_vec = np.array([0.0, 1.0]) # read out of json
    wind_direction_deg = 0.0 # read out of json
    boat_heading_vec = np.array([1.0, 0.0]) # read out of json
    goalcords_vec = np.array([1.0, 1.0])

    goal_vector = np.subtract(goalcords_vec, boatcords_vec) 

    optimal_heading_vec = calculate_new_heading(boat_heading_vec, wind_direction_vec, goal_vector)
    servo_control(rudder_pd(boat_heading_vec, optimal_heading_vec))






