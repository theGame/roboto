#pragma once

#include <stdint.h>
#include <SimpleKalmanFilter.h>
#include <Arduino.h>

/* Attach ultrasonic sensor pins */
void setupUltrasonicSensor();
/* Measure distance to obsticle */
float getDistance();
/* Measure the distance in front of the car while it is driving */
float getFilteredDistance();
