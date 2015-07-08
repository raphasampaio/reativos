#include "Arduino.h"
#include "event_driven.h"

#define MAX_SENSORS 2
int sensor[2][2];
int n_sensor;

void sensor_listener(int echo, int tri) {
   if(n_sensor >= MAX_SENSORS) {
     return; 
   }

   sensor[n_sensor][0] = echo;
   sensor[n_sensor++][1] = tri;
   pinMode(tri, OUTPUT);
   pinMode(echo, INPUT);
}

long get_distance(int echoPin, int trigPin) {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}

void setup() {
  custom_init();
}

void loop() {
    long distance;
    for(int i = 0; i < n_sensor; i++) {
      distance = get_distance(sensor[i][0], sensor[i][1]);
      cb_distance(i, distance);
    }
}