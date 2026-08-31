#include "sample.h"

// Implement the logic for rotating the car using the gyroscope
// This function can be called in the loop to perform rotation based on gyroscope readings
void rotateUsingGyroscope() {
    giroscope.update();
    if (direction.isRotationInProcess()) {
        direction.verifyRotationReached(giroscope.getAngleZ());
        return;
    } else {
        navigation.drive(DirectionType::FRONT);
        delay(1000);
        navigation.drive(DirectionType::RIGHT);
        delay(1000);
    }
}


// Implement the logic for avoiding obstacles using the ultrasonic sensor and the gyroscope
// This function can be called in the loop to perform obstacle avoidance maneuvers
void avoidObstacles() {
    giroscope.update();

    if (direction.isRotationInProcess()) {
        direction.verifyRotationReached(giroscope.getAngleZ());

        Serial.println("Inside isRotationInProcess");
        if (direction.isRotationInProcess()) {
            Serial.println("STILL isRotationInProcess"); 
            navigation.drive(navigation.decideDirection(distances));
            return;
        }

        Serial.println("FINISHED isRotationInProcess"); 

        // Rotation finished
        navigation.drive(DirectionType::STOP);
    }
    

    // Track the distance in front of the car using a Kalman filter while driving.
    distances.front = getFilteredDistance();    
    
    if (distances.canGoForward()) {
        Serial.println("FORWARD"); 
        navigation.drive(DirectionType::FRONT);
    } else {
        Serial.println("ELSE");
        navigation.drive(DirectionType::STOP);

        distances.scan();
        distances.print();

        navigation.drive(navigation.decideDirection(distances));
    }
}