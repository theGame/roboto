#include "my-giroscope.h"

// public intstance
MyGiroscope giroscope;

MyGiroscope::MyGiroscope() : mpu(Wire) {}

void MyGiroscope::setup() {

    Wire.begin();
  
    byte status = mpu.begin();
    Serial.print(F("MPU6050 status: "));
    Serial.println(status);
    while(status!=0){
        Serial.print(F("Connecting..."));
     } // stop everything if could not connect to MPU6050
  
    Serial.println(F("Calculating offsets, do not move MPU6050"));
    delay(1000);
    
    mpu.calcOffsets(); // gyro and accelero
    Serial.println("Done!\n");
}

void MyGiroscope::update() {
    mpu.update();
}

float MyGiroscope::getAngleZ() {
    return mpu.getAngleZ();
}