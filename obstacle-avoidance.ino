#include "my-servo.h"
#include "my-motor.h"
#include "my-ultrasonic-sensor.h"
#include "my-giroscope.h"
#include "distances.h"
#include "direction.h"
#include "navigation.h"

void setup() {
  Serial.begin(115200);
  setupServo();
  setupUltrasonicSensor();
  setupMotors();
}

void loop() {
    // giroscope.update();
    // const float yaw = giroscope.getYaw();

    /*
    if (direction.isRotationInProcess()) {
        direction.verifyRotationReached(yaw);
        
        
        Serial.println("Inside isRotationInProcess"); 
        if (direction.isRotationInProcess()) {
            Serial.println("STILL isRotationInProcess"); 
            navigation.drive(navigation.decideDirection(distances));
            return;
        }

        Serial.println("FINISHED isRotationInProcess"); 

        // Rotation finished
        // navigation.drive(DirectionType::STOP);
    }
    */

    Serial.println("START moving"); 
    // Track the distance in front of the car using a Kalman filter while driving.
    // distances.front = getFilteredDistance();    
    
    // if (distances.canGoForward()) {
    //     Serial.println("FORWARD"); 
    //     navigation.drive(DirectionType::FRONT);
    // } else {
    //     Serial.println("ELSE");
    //     navigation.drive(DirectionType::STOP);

    //     distances.scan();
    //     distances.print();

    //     navigation.drive(navigation.decideDirection(distances));
    // }



    navigation.drive(DirectionType::FRONT);
    motor(Forward, 250);
}



