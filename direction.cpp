#include "direction.h"
#include <Arduino.h>
 
Direction direction;
 
bool Direction::isRotationInProcess() {
  return shouldRotate;
}
 
void Direction::verifyRotationReached(float currentYaw) {
  // Check whether the turn has reached or passed its target, based on the
  // direction yaw should be moving in, rather than requiring yaw to land
  // inside a narrow +/-tolerance window. At the current rotation speed/delay,
  // yaw can jump 20-30+ degrees between checks, which can step clean over a
  // 2-degree-wide window and leave the robot spinning forever.
  bool reached = (rotationDirection >= 0)
      ? (currentYaw >= rotateTo - rotationTolerance)
      : (currentYaw <= rotateTo + rotationTolerance);

 
  if (reached) {
    Serial.println("RESET!");
    shouldRotate = false;
    rotateTo = currentYaw;
  }
}
 
void Direction::startRotation(float currentYaw, float rotationAngle) {
    rotateTo = currentYaw + rotationAngle;
    rotationDirection = (rotationAngle >= 0) ? 1 : -1;
    shouldRotate = true;
}
 
float Direction::getRotationAngle(DirectionType direction) {
  switch (direction) {
    case DirectionType::LEFT:
      return -90.0f;
    case DirectionType::LEFT45:
      return -45.0f;
 
    case DirectionType::FRONT:
      return 0.0f;
 
    case DirectionType::RIGHT45:
      return 45.0f;
 
    case DirectionType::RIGHT:
      return 90.0f;
 
    default:
      return 0.0f;
    }
}