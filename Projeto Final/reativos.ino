#include <Stepper.h>
#include "event_driven.h"

Stepper stepper(2048, 10, 8, 9, 7);

#define echoPin1 3 
#define trigPin1 4
#define echoPin2 5 
#define trigPin2 6

int direction;

void custom_init() {
  Serial.begin(9600);
  direction = 1;
  stepper.setSpeed(14);
  
  sensor_listener(echoPin1, trigPin1);
  sensor_listener(echoPin2, trigPin2);
}

void cb_distance(int sensor, long distance) {
  if (distance < 15) {
    if (sensor == 0 && direction == -1) {
      direction = 1;
    }
    else if (sensor == 1 && direction == 1) {
      direction = -1;
    }
  }
  else
    stepper.step(direction * 256);
    
  Serial.print(distance);
  Serial.print(" cm ");
  Serial.print(sensor);
  Serial.println("");
}
