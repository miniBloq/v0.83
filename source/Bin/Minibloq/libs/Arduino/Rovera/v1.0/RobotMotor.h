/*******************************************************
    RobotMotor.h
    low level motor driver interface
	
    Copyright Michael Margolis May 8 2012
********************************************************/

/* if you have the 4WD chassis:
      change the line:
        CHASSIS_2WD 
      to:
  	    CHASSIS_4WD
 */
#ifndef ROBOT_MOTOR_H
#define ROBOT_MOTOR_H 

#define CHASSIS_2WD   // change 2WD to 4WD if using the 4WD chassis

// defines for left and right motors
const int MOTOR_LEFT  = 0;
const int MOTOR_RIGHT = 1;

extern const int MIN_SPEED;
extern int speedTable[];
extern int rotationTime[];
extern const int SPEED_TABLE_INTERVAL;
extern const int NBR_SPEEDS;
extern const int mmPer360;

void motorBegin(int motor); 

// speed range is 0 to 100 percent
void motorSetSpeed(int motor, int speed);

void motorForward(int motor, int speed);

void motorReverse(int motor, int speed);

void motorStop(int motor);

void motorBrake(int motor);

unsigned long motorDistanceMMToTime(unsigned long distance);

#endif