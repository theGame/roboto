#include "direction.h"

Direction direction;

bool Direction::isRotationInProcess() {
  return shouldRotate;
}

void Direction::verifyRotationReached(float currentYaw) {
  constexpr float ROTATION_TOLERANCE = 1.0f;

  if (fabs(currentYaw - rotateTo) <= ROTATION_TOLERANCE) {
    shouldRotate = false;
    rotateTo = currentYaw;
  }
}

void Direction::startRotation(float currentYaw, float rotationAngle) {
    rotateTo = currentYaw + rotationAngle;
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