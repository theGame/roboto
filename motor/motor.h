#pragma once

#include <stdint.h>
#include <Arduino.h>

extern const uint8_t Forward;
extern const uint8_t Backward;
extern const uint8_t Turn_Left;
extern const uint8_t Turn_Right;
extern const uint8_t Top_Left;
extern const uint8_t Bottom_Left;
extern const uint8_t Top_Right;
extern const uint8_t Bottom_Right;
extern const uint8_t Stop;
extern const uint8_t ContraClockwise;
extern const uint8_t Clockwise;

void setupMotors();
void motor(uint8_t Dir, uint8_t Speed);