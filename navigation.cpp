#include "navigation.h"

// public intstance
Navigation navigation;

DirectionType Navigation::decideDirection(const Distances& distances) {
    // Prefer going straight if there is enough space.
    if (distances.canGoForward()) {
        return DirectionType::FRONT;
    }

    DirectionType bestDirection = DirectionType::UNKNOWN;
    uint8_t maxDistance = 0;

    // Front is blocked.
    // Find the best available direction.
    // Check from right to left.
    if (distances.canGoRight() && distances.right > maxDistance) {
        maxDistance = distances.right;
        bestDirection = DirectionType::RIGHT;
    }

    if (distances.canGoRight45() && distances.right45 > maxDistance) {
        maxDistance = distances.right45;
        bestDirection = DirectionType::RIGHT45;
    }

    if (distances.canGoLeft45() && distances.left45 > maxDistance) {
        maxDistance = distances.left45;
        bestDirection = DirectionType::LEFT45;
    }

    if (distances.canGoLeft() && distances.left > maxDistance) {
        maxDistance = distances.left;
        bestDirection = DirectionType::LEFT;
    }

    // Nothing is safe.
    if (maxDistance <= distances.MIN_SAFE_DISTANCE) {
        bestDirection = DirectionType::BACK;
    }

    return bestDirection;
}

void Navigation::drive(DirectionType directionType) {
    switch (directionType) {
        case DirectionType::FRONT:
            // there is no need rotation
            motor(Forward, DRIVE_SPEED);
            delay(DRIVING_DELAY);
            break;

        case DirectionType::RIGHT:
            if (!direction.isRotationInProcess()) {
                direction.startRotation(
                    giroscope.getYaw(),
                    direction.getRotationAngle(DirectionType::RIGHT)
                );
            }
            motor(Clockwise, ROTATION_SPEED);
            delay(ROTATION_DELAY);
            break;

        case DirectionType::RIGHT45:
            if (!direction.isRotationInProcess()) {
                direction.startRotation(
                    giroscope.getYaw(),
                    direction.getRotationAngle(DirectionType::RIGHT45)
                );
            }
            motor(Clockwise, ROTATION_SPEED);
            delay(ROTATION_DELAY);
            break;

        case DirectionType::LEFT45:
            if (!direction.isRotationInProcess()) { 
                direction.startRotation(
                    giroscope.getYaw(),
                    direction.getRotationAngle(DirectionType::LEFT45)
                );
            }
        
            motor(ContraClockwise, ROTATION_SPEED);
            delay(ROTATION_DELAY);
            break;

        case DirectionType::LEFT:
            if (!direction.isRotationInProcess()) { 
                direction.startRotation(
                    giroscope.getYaw(),
                    direction.getRotationAngle(DirectionType::LEFT)
                );
            }
            motor(ContraClockwise, ROTATION_SPEED);
            delay(ROTATION_DELAY);
            break;

        case DirectionType::BACK:
            // Boxed in on every scanned angle: actually reverse instead of
            // just stopping, so the car isn't stuck here forever.
            motor(Backward, DRIVE_SPEED);
            delay(DRIVING_DELAY);
            break;

        case DirectionType::STOP:
        case DirectionType::UNKNOWN:
        default:
            Serial.println("STOP | UNKNOWN ");
            motor(Stop, 0);
            delay(DRIVING_DELAY);
            break;
    }
}