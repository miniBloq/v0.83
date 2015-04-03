// Look.h

#ifndef LOOK_H
#define LOOK_H

#include "Distance.h"

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

void lookBegin();

// returns true if the given obstacle is detected
boolean lookForObstacle(int obstacle);

// returns the distance of objects at the given angle
// this version rotates the robot
int lookAt(int angle);

// function to check if robot can continue moving in current direction 
// returns true if robot is blocked moving in current direction
// this version only tests for obstacles in front
bool checkMovement();

// robot uses distance and bump sensors to move around and avoid obstacles
void roam();

#endif
