//
// Created by georg on 13.02.2025.
//

#ifndef RUDDER_H
#define RUDDER_H
#include "../Actuator.h"
#include <PID_v1.h>

class Rudder : public Actuator {
    public:
       static static void setRudderPos();

};



#endif //RUDDER_H
