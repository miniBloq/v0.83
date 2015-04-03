/*******************************************************
    RobotMotor.cpp // Adafruit 2WD version
    low level motor driver for use with adafruit motor shield and 2WD robot

    Motor constants used are defined AFMotor.h

    Copyright Michael Margolis May 8 2012
********************************************************/

#include <Arduino.h>  
#include "./AFMotor.h"  // adafruit motor shield library
#include "./RobotMotor.h"

#include "AFMotor.cpp" // temp for minibloq

const int differential = 6; // % faster left motor turns compared to right  

// tables hold time in ms to rotate robot 360 degrees at various speeds 
// this enables conversion of rotation angle into timed motor movement 
// The speeds are percent of max speed
// Note: low cost motors do not have enough torque at low speeds so
// the robot will not move below this value 
// Interpolation is used to get a time for any speed from MIN_SPEED to 100%

const int MIN_SPEED = 40; // first table entry is 40% speed
const int SPEED_TABLE_INTERVAL = 10; // each table entry is 10% faster speed
const int NBR_SPEEDS =  1 + (100 - MIN_SPEED)/ SPEED_TABLE_INTERVAL;
 
int speedTable[NBR_SPEEDS]  =  {40,     50,   60,   70,   80,   90,  100}; // speeds  
int rotationTime[NBR_SPEEDS] = {2000, 1500, 1100, 900, 800, 700, 500}; // time 

const int msPerCM = 40; // how many milliseconds for the robot to move 1 cm
const int mmPer360 = 140 * 3.1416; // how many mm a wheel needs to move to rotate the robot 360 degrees (2WD = circumference of wheel track distance)

AF_DCMotor motors[] = {
    AF_DCMotor(1, MOTOR12_1KHZ), // left is Motor #1 
    AF_DCMotor(2, MOTOR12_1KHZ)  // right is Motor #2
  };

int  motorSpeed[2]  = {0,0}; // left and right motor speeds stored here (0-100%)

void motorBegin(int motor)
{
  motorStop(motor);
}

// speed range is 0 to 100 percent
void motorSetSpeed(int motor, int speed)
{
  if( motor == MOTOR_LEFT && speed > differential)
    speed -= differential;
  motorSpeed[motor] = speed;           // save the value
  int pwm = map(speed, 0,100, 0,255);  // scale to PWM range
   
  motors[motor].setSpeed(pwm) ;
}

void motorForward(int motor, int speed)
{
  motorSetSpeed(motor, speed);
  motors[motor].run(FORWARD); 
}

void motorReverse(int motor, int speed)
{
  motorSetSpeed(motor, speed);
  motors[motor].run(BACKWARD); 
}

void motorStop(int motor)
{
  // todo set speed to 0 ???
  motors[motor].run(RELEASE);      // stopped
}

void motorBrake(int motor)
{
  motors[motor].run(BRAKE);      // stopped
}