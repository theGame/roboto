#pragma once

#include "my-servo.h"
#include "my-ultrasonic-sensor.h"
#include <Arduino.h>

struct Distances {
    static constexpr uint8_t MIN_SAFE_DISTANCE = 25;

    uint8_t right;
    uint8_t right45;
    uint8_t front;
    uint8_t left45;
    uint8_t left;

    bool isSafe(uint8_t distance) const;

    bool canGoLeft() const;
    bool canGoLeft45() const;
    bool canGoForward() const;
    bool canGoRight45() const;
    bool canGoRight() const;

    void print() const;

    void scan();
};

extern Distances distances;