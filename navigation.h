#pragma once

#include "distances.h"
#include "direction.h"
#include "my-motor.h"
#include "my-giroscope.h"
#include <Arduino.h>

class Navigation {

public:
  // Desides where to move
  DirectionType decideDirection(const Distances& distances);
  // Starts engine
  void drive(DirectionType direction);

private:
  static constexpr uint8_t DRIVE_SPEED = 100;
  static constexpr uint8_t ROTATION_SPEED = 100;
};

extern Navigation navigation;

