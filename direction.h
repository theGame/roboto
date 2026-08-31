#pragma once

#include <math.h>
#include <stdint.h>

enum class DirectionType {
    LEFT,
    LEFT45,
    FRONT,
    RIGHT45,
    RIGHT,
    BACK,
    UNKNOWN,
    STOP
};

class Direction {
public:
   bool isRotationInProcess();
   void verifyRotationReached(float currentYaw);
   void startRotation(float currentYaw, float rotationAngle);
   float getRotationAngle(DirectionType direction);

private:
    // Flag indicate that robo needs to continue rotating
    bool shouldRotate = false; 
    // Target angle the robot should reach during rotation
    float rotateTo = 0.0f;

    // +1 while yaw should be increasing towards rotateTo (right turns),
    // -1 while it should be decreasing (left turns). Used so we detect
    // "reached or passed the target" instead of requiring an exact
    // narrow-window match, which large per-check yaw steps can skip over.
    int8_t rotationDirection = 1;

    float rotationTolerance = 3.0f;
};

extern Direction direction;