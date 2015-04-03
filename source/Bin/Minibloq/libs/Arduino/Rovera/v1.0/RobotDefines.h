
/***** Global Defines ****/ 


#ifndef ROBOT_DEFINES_H
#define ROBOT_DEFINES_H

#define SerialDisplay Serial

// defines for locations of sensors
const int SENSE_LEFT  = 0;
const int SENSE_RIGHT = 1;

// defines for directions
const int DIR_LEFT   = 0;
const int DIR_RIGHT  = 1;
const int DIR_CENTER = 2;
    
///const char* locationString[] = {"Left", "Right", "Center"}; // labels for debug
// see http://arduino.cc/en/Reference/String for more on character string arrays

///const char* obstacleStr[] = {"none","left edge", "right edge", "front edge"};

// obstacles constants 
const int OBST_NONE       = 0;  // no obstacle detected
const int OBST_LEFT_EDGE  = 1;  // left edge detected 
const int OBST_RIGHT_EDGE = 2;  // right edge detected
const int OBST_FRONT_EDGE = 3;  // edge detect at both left & right sensors
const int OBST_FRONT      = 4;  // obstacle in front
const int OBST_REAR       = 5;  // obstacle behind 

const int LED_PIN = 13;

/// move states:
enum        {MOV_LEFT, MOV_RIGHT, MOV_FORWARD, MOV_BACK, MOV_ROTATE, MOV_STOP};

/**** End of Global Defines ****************/
#endif
