//
// Created by georg on 08.02.2025.
//


#include <vector>
#include <ArduinoEigen.h>

#include "AS5600.h"
#ifndef WINDSENSE_H
#define WINDSENSE_H

AS5600L as5600;

class Windsense {

public:
    static void setup();

    static int getAngle();

    static int getAvgAngle();

    static Eigen::Vector2d getWindDirection();

};



#endif //WINDSENSE_H
