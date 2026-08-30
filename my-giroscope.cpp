#include "my-giroscope.h"

// public intstance
MyGiroscope giroscope;

void MyGiroscope::setup() {

    Wire.begin();

    Wire.beginTransmission(ADDRESS);

    Wire.write(PWR_MGMT_1);
    Wire.write(0x00);

    Wire.endTransmission(true);

    delay(100);
    previousTime = micros();
    delay(500);
    calibrateGyroscope();
    Serial.println("MPU6050 ready!");
}


void MyGiroscope::calibrateGyroscope() {

    long sumZ = 0;

    Serial.println("Calibrating gyroscope...");
    Serial.println("DO NOT MOVE THE ROBOT!");

    for (uint16_t i = 0; i < CALIBRATION_SAMPLES; i++) {

        int16_t x;
        int16_t y;
        int16_t z;

        readGyroscope(x, y, z);

        sumZ += z;

        delay(5);
    }

    gyroZOffset =
        (static_cast<float>(sumZ) / CALIBRATION_SAMPLES)
        / GYRO_SCALE;

    Serial.print("Gyro Z offset: ");
    Serial.println(gyroZOffset);

    previousTime = micros();
}


void MyGiroscope::update() {
    int16_t rawX;
    int16_t rawY;
    int16_t rawZ;

    readGyroscope(rawX, rawY, rawZ);

    float gyroZ = rawZ / GYRO_SCALE - gyroZOffset;

    // Ignore small gyro noise
    if (abs(gyroZ) < GYRO_DEAD_ZONE) {
        gyroZ = 0;
    }

    unsigned long currentTime = micros();

    float elapsedTime =
        (currentTime - previousTime) / 1000000.0f;

    previousTime = currentTime;

    yaw += gyroZ * elapsedTime;
}


float MyGiroscope::getYaw() const {
    return yaw;
}


void MyGiroscope::resetYaw() {
    yaw = 0;
    previousTime = micros();
}


/*
 * Read one signed 16-bit value from two bytes.
 */
int16_t MyGiroscope::readInt16() {
  return (Wire.read() << 8) | Wire.read();
}


/*
 * Read raw gyroscope values.
 */
void MyGiroscope::readGyroscope(
    int16_t& x,
    int16_t& y,
    int16_t& z
) {

    Wire.beginTransmission(ADDRESS);

    Wire.write(GYRO_XOUT_H);

    Wire.endTransmission(false);

    Wire.requestFrom(ADDRESS, GYRO_DATA_SIZE, true);

    x = readInt16();
    y = readInt16();
    z = readInt16();
}