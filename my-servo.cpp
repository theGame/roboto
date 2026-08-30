#include "my-servo.h"

/* Creates servo object to control servo, with defined PIN */
namespace {
  const uint8_t ServoPin = 9;
  Servo servo;
}

void setupServo() {
  servo.attach(ServoPin, 500, 2400);
  turnServo(90);
  delay(500);
}

void turnServo(uint8_t angle) {
  servo.write(angle);
}