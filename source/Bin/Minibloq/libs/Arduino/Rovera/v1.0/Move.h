// Move.h

#ifndef MOVE_H
#define MOVE_H

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

const unsigned long MAX_DISTANCE_ARG = 99999999;  // largest value in mm for distance argument (todo - remove hard coded values in constMaxDist block)
const unsigned long MAX_TIME_ARG = 99999999;      // largest value in ms for time argument 


const float WHEEL_TRACK            = 275.0;              //tyre seperation in mm  
const float FRICTION_FACTOR        = 1.2;                //increase movement to counteract rotational friction
const float WHEEL_TRAVEL_PER_360   = WHEEL_TRACK * 3.1416 * FRICTION_FACTOR;  // mm per robot revolution 

const unsigned long MAX_TIME     = 4294967295; // just under 50 days
const unsigned long MAX_DISTANCE = 99999999;   // very far

  
void moveBegin();
void moveLeft();
void moveRight();
void moveForward();
void moveBackward();
void moveRotate(int angle);
void moveStop();
void moveBrake();
void moveSetSpeed(int speed);
int moveGetSpeed();
int moveGetState();
// calls given watch function, returns true of obstacle found
bool delayWatchful(unsigned long interval, bool (*watchFunc)(void));
bool moveForwardWatchful( unsigned long distance, bool (*watchFunc)(void));
bool moveReverseWatchful( unsigned long distance, bool (*watchFunc)(void));
bool moveRotateWatchful( int angle, bool (*watchFunc)(void));
bool moveForwardTimedWatchful( unsigned long durationMS, bool (*watchFunc)(void));
bool moveReverseTimedWatchful( unsigned long durationMS, bool (*watchFunc)(void));
bool ignoreWatch();
void changeMoveState(int newState);
unsigned long moveDistanceToTime( unsigned long distance, int speed);


#endif
