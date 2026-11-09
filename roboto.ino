#include "servo/servo.cpp"
#include "motor/motor.cpp"
#include "ultrasonic-sensor/ultrasonic-sensor.cpp"
#include "giroscope/giroscope.cpp"
#include "direction/direction.cpp"
#include "distances/distances.cpp"
#include "navigation/navigation.cpp"
#include "sample/sample.cpp"

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
    // avoidObstacles();
    // chickenHeadManeuver();
    rotateUsingGyroscope();
}


