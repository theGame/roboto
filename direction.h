#pragma once

#include <math.h>

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
};

extern Direction direction;