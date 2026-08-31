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
    static constexpr uint8_t ROTATION_DELAY = 20;

    // Begins tracking a turn towards directionType using the current yaw as the
    // starting point, unless a turn is already in progress.
    void startRotationIfNeeded(DirectionType directionType);

    // The direction chosen when the current rotation started. Reused every
    // tick of that rotation so the target can't drift or get overridden
    // mid-turn by a re-evaluated (and possibly stale) decideDirection() call.
    DirectionType activeRotationDirection = DirectionType::UNKNOWN;

  public:
    // Desides where to move
    DirectionType decideDirection(const Distances& distances);
    // Starts engine
    void drive(DirectionType direction);
    // Keeps driving towards whichever direction the current rotation was started with.
    void continueRotation();
};

extern Navigation navigation;

