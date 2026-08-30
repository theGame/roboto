#pragma once

#include <Arduino.h>
#include <Wire.h>

class MyGiroscope {
public:
    void setup();

    void update();

    float getYaw() const;

    void resetYaw();

private:
    static constexpr uint8_t ADDRESS = 0x68;
    // The MPU6050 gyroscope returns 6 bytes
    static constexpr uint8_t GYRO_DATA_SIZE = 6;

    static constexpr uint8_t PWR_MGMT_1 = 0x6B;
    static constexpr uint8_t GYRO_XOUT_H = 0x43;

    static constexpr float GYRO_SCALE = 131.0f;
    // Number of gyro readings used to calculate the calibration offset
    static constexpr uint16_t CALIBRATION_SAMPLES = 600;
    // Ignore small gyroscope readings caused by sensor noise
    static constexpr float GYRO_DEAD_ZONE = 0.5f;

    float gyroZOffset = 0;
    float yaw = 0;

    unsigned long previousTime = 0;

    int16_t readInt16();

    void calibrateGyroscope();

    void readGyroscope(
        int16_t& x,
        int16_t& y,
        int16_t& z
    );
};

extern MyGiroscope giroscope;