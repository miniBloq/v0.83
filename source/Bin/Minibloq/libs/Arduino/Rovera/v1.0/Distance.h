// Distance.h

#ifndef DISTANCE_H
#define DISTANCE_H

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

/*********************************
 code for official Ping))) distance sensor
**********************************/
int pingGetDistance(int pingPin);
long microsecondsToCentimeters(long microseconds);

/*********************************
 code for HC-SR04 4 pin sonar sensor using single pin
**********************************/
// Returns the distance in cm
// this will return 0 if no ping sensor is connected or the distance is greater than around 10 feet
int HC_SR04GetDistance(int pingPin);

/*********************************
 code for sharp IR distance sensor
**********************************/
int irGetDistance(byte pin);
int mvToDistance(int mV);

#endif
