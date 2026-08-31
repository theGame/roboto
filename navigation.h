#pragma once

#include "distances.h"
#include "direction.h"
#include "my-motor.h"
#include "my-giroscope.h"
#include <Arduino.h>

class Navigation {
  private:
    static constexpr uint8_t DRIVE_SPEED = 180;
    static constexpr uint16_t DRIVING_DELAY = 300;

    static constexpr uint8_t ROTATION_SPEED = 180;
    static constexpr uint8_t ROTATION_DELAY = 150;
    
  public:
    // Desides where to move
    DirectionType decideDirection(const Distances& distances);
    // Starts engine
    void drive(DirectionType direction);
};

extern Navigation navigation;

