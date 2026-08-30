#include "my-ultrasonic-sensor.h"

// Ultrasonic control pin, and init filter
namespace {
  const uint8_t Trig = 12;
  const uint8_t Echo = 13;

  SimpleKalmanFilter simpleKalmanFilter(1, 1, 0.7);  
}


void setupUltrasonicSensor() {
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
}

float getDistance() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  float distance = pulseIn(Echo, HIGH) / 58.00;
  delay(10);
  return distance;
}

float getFilteredDistance() {
  return simpleKalmanFilter.updateEstimate(getDistance());
}