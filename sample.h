#include "my-servo.h"
#include "my-motor.h"
#include "my-ultrasonic-sensor.h"
#include "my-giroscope.h"
#include "distances.h"
#include "direction.h"
#include "navigation.h"

#pragma once

// Functions for rotating the car using the gyroscope
void rotateUsingGyroscope();

// Functions for avoiding obstacles using the ultrasonic sensor and the gyroscope
void avoidObstacles();