#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_light.h>

class MyGiroscope {
public:
    MyGiroscope();

    void setup();
    /* Reads the latest sensor values; call once per loop() iteration. */
    void update();
    /* Current rotation angle around Z (yaw), in degrees. */
    float getAngleZ();

    MPU6050 mpu;
};

extern MyGiroscope giroscope;