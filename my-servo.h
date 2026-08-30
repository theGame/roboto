#pragma once

#include <Servo.h>  //servo library
#include <Arduino.h>

/* Attach servo on pin 9 to servo object */
void setupServo();
/* Turn the servo to the specified angle */
void turnServo(uint8_t angle);