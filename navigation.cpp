#include "navigation.h"

// public intstance
Navigation navigation;

DirectionType Navigation::decideDirection(const Distances& distances) {
    // Prefer going straight if there is enough space.
    if (distances.canGoForward()) {
        Serial.println("decideDirection: FRONT");
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
        Serial.println("decideDirection: RIGHT");
    }

    if (distances.canGoRight45() && distances.right45 > maxDistance) {
        maxDistance = distances.right45;
        bestDirection = DirectionType::RIGHT45;
        Serial.println("decideDirection: RIGHT45");
    }

    if (distances.canGoLeft45() && distances.left45 > maxDistance) {
        maxDistance = distances.left45;
        bestDirection = DirectionType::LEFT45;
        Serial.println("decideDirection: LEFT45");
    }

    if (distances.canGoLeft() && distances.left > maxDistance) {
        maxDistance = distances.left;
        bestDirection = DirectionType::LEFT;
        Serial.println("decideDirection: LEFT");
    }

    // Nothing is safe.
    if (maxDistance <= distances.MIN_SAFE_DISTANCE) {
        bestDirection = DirectionType::BACK;
        Serial.println("decideDirection: BACK");
    }

    return bestDirection;
}

void Navigation::drive(DirectionType directionType) {
    switch (directionType) {
        case DirectionType::FRONT:
            Serial.println("FRONT");
            // there is no need rotation
            motor(Forward, DRIVE_SPEED);
            break;

        case DirectionType::RIGHT:
            Serial.println("RIGHT");
            direction.startRotation(
                giroscope.getYaw(),
                direction.getRotationAngle(DirectionType::RIGHT)
            );
            motor(Clockwise, ROTATION_SPEED);
            break;

        case DirectionType::RIGHT45:
            Serial.println("RIGHT45");
            direction.startRotation(
                giroscope.getYaw(),
                direction.getRotationAngle(DirectionType::RIGHT45)
            );
            motor(Clockwise, ROTATION_SPEED);
            break;

        case DirectionType::LEFT45:
            Serial.println("LEFT45");
            direction.startRotation(
                giroscope.getYaw(),
                direction.getRotationAngle(DirectionType::LEFT45)
            );
            motor(ContraClockwise, ROTATION_SPEED);
            break;

        case DirectionType::LEFT:
            Serial.println("LEFT");
            direction.startRotation(
                giroscope.getYaw(),
                direction.getRotationAngle(DirectionType::LEFT)
            );
            motor(ContraClockwise, ROTATION_SPEED);
            break;

        case DirectionType::STOP:
        case DirectionType::UNKNOWN:
        case DirectionType::BACK:
        default:
            Serial.println("STOP | UNKNOWN | BACK");
            motor(Stop, 0);
            break;
    }
}