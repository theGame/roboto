#include "my-servo.h"
#include "my-motor.h"
#include "my-ultrasonic-sensor.h"
#include "my-giroscope.h"
#include "sample.h"

void setup() {
    Serial.begin(115200);
    setupServo();
    setupUltrasonicSensor();
    setupMotors();
    giroscope.setup();
}

void loop() {
    // to keep this file klean, 
    // the main loop is just calling the functions from sample.h
    avoidObstacles();
}



