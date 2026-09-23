#include "../servo/servo.h"
#include "../motor/motor.h"
#include "../ultrasonic-sensor/ultrasonic-sensor.h"
#include "../giroscope/giroscope.h"
#include "../distances/distances.h"
#include "../direction/direction.h"
#include "../navigation/navigation.h"

#pragma once

// Functions for rotating the car using the gyroscope
void rotateUsingGyroscope();

// Functions for avoiding obstacles using the ultrasonic sensor and the gyroscope
void avoidObstacles();

// Function for performing a chicken head maneuver
void chickenHeadManeuver();