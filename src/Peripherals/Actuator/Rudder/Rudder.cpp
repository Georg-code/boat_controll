//
// Created by georg on 13.02.2025.
//

#include "Rudder.h"
#include <Arduino.h>

#include <Navigation/Navigation.h>


void Rudder::setRudderPos() {
  



   Navigation::calculateNextHeading(0, 0 , Navigation::getGoal());
}
