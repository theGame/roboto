#include "my-motor.h"

/* Initialize PINs */
namespace {
  // PWM control pin
  const uint8_t PWM1_PIN = 5;
  const uint8_t PWM2_PIN = 6;

  // 74HCT595N Chip pins
  const uint8_t SHCP_PIN = 2;
  const uint8_t EN_PIN   = 7;
  const uint8_t DATA_PIN = 8;
  const uint8_t STCP_PIN = 4; 

  // Motor directions
  const uint8_t LeftUpBack = 128;
  const uint8_t LeftUpForw = 64;
  const uint8_t LeftDownForw = 16;
  const uint8_t LeftDownBack = 32;
  const uint8_t RightDownBack = 1;
  const uint8_t RightDownForw = 8;
  const uint8_t RightUpForw = 4;
  const uint8_t RightUpBack = 2;
}

const uint8_t Forward = LeftUpForw | LeftDownForw | RightDownForw | RightUpForw;          // forward
const uint8_t Backward = LeftUpBack | LeftDownBack | RightDownBack | RightUpBack;         // back
const uint8_t Turn_Left = LeftUpBack | LeftDownForw | RightUpForw | RightDownBack;        // left translation
const uint8_t Turn_Right = LeftUpForw | LeftDownBack | RightUpBack | RightDownForw;       // Right translation
const uint8_t Top_Left = LeftDownForw | RightUpForw;                                      // Upper left mobile
const uint8_t Bottom_Left = LeftUpBack | RightDownBack;                                   // Lower left mobile
const uint8_t Top_Right = LeftUpForw | RightDownForw;                                     // Upper right mobile
const uint8_t Bottom_Right = LeftDownBack | RightUpBack;                                  // The lower right move
const uint8_t Stop = 0;                                                                   // stop
const uint8_t ContraClockwise = LeftUpBack | LeftDownBack | RightDownForw | RightUpForw;  // Counterclockwise rotation
const uint8_t Clockwise = LeftUpForw | LeftDownForw | RightDownBack | RightUpBack;        // Rotate clockwise

/* Setup motors pins */
void setupMotors() {
  pinMode(SHCP_PIN, OUTPUT);
  pinMode(EN_PIN,   OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(STCP_PIN, OUTPUT);
  pinMode(PWM1_PIN, OUTPUT);
  pinMode(PWM2_PIN, OUTPUT);
}

/* Run the motor in different directions and at different speeds */
void motor(uint8_t Dir, uint8_t Speed) {
  analogWrite(PWM1_PIN, Speed);
  analogWrite(PWM2_PIN, Speed);

  digitalWrite(STCP_PIN, LOW);
  shiftOut(DATA_PIN, SHCP_PIN, MSBFIRST, Dir);
  digitalWrite(STCP_PIN, HIGH);
}

