#include "distances.h"

Distances distances = {0};

bool Distances::isSafe(uint8_t distance) const {
    return distance > MIN_SAFE_DISTANCE;
}

bool Distances::canGoLeft() const {
    return isSafe(left);
}

bool Distances::canGoLeft45() const {
    return isSafe(left45);
}

bool Distances::canGoForward() const {
    return isSafe(front);
}

bool Distances::canGoRight45() const {
    return isSafe(right45);
}

bool Distances::canGoRight() const {
    return isSafe(right);
}

void Distances::print() const {
    Serial.print("left: " + String(left));
    Serial.print(" | left45: " + String(left45));
    Serial.print(" | front: " + String(front));
    Serial.print(" | right45: " + String(right45));
    Serial.print(" | right: " + String(right));
    Serial.println();
}

void Distances::scan() {
    Serial.println("SCAN START");

    turnServo(0);
    // have a lottle bigger delay
    // there was a small issue, when servo doesn't have enough time to turn right
    delay(700);
    right = getDistance();

    turnServo(45);
    delay(500);
    right45 = getDistance();

    turnServo(90);
    delay(500);
    front = getDistance();

    turnServo(135);
    delay(500);
    left45 = getDistance();

    turnServo(180);
    delay(500);
    left = getDistance();

    turnServo(90);
    delay(500);
    Serial.println("SCAN FINISHED");
}