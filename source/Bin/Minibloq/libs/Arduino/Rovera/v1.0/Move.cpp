/*************************************
 * Move: mid level movement functions
 *************************************/
#include "Move.h"
#include "RobotMotor.h"
#include "RobotDefines.h"

const char* states[] = {"Left", "Right", "Forward", "Back", "Rotate", "Stop"};

int moveState = MOV_STOP;   // what robot is doing

int  moveSpeed   = 0;     // move speed stored here (0-100%)
int  speedIncrement = 10; // percent to increase or decrease speed


#define NO_ENCODER  // enable this if robot does not have movement encoders
 
/*******************************************/

void moveBegin()
{
  motorBegin(MOTOR_LEFT); 
  motorBegin(MOTOR_RIGHT); 
  moveStop();
}

void moveLeft()
{
  changeMoveState(MOV_LEFT);
  motorForward(MOTOR_LEFT,  0);
  motorForward(MOTOR_RIGHT, moveSpeed);
}

void moveRight()
{
  changeMoveState(MOV_RIGHT);
  motorForward(MOTOR_LEFT,  moveSpeed);
  motorForward(MOTOR_RIGHT, 0);
}

void moveForward()
{
  Serial.print("moveForward @ "); 
  Serial.println(moveSpeed);
  changeMoveState(MOV_FORWARD);
  motorForward(MOTOR_LEFT,  moveSpeed);
  motorForward(MOTOR_RIGHT, moveSpeed);
}

void moveBackward()
{
  Serial.print("moveBackward @ "); 
  Serial.println(moveSpeed);
  changeMoveState(MOV_BACK);
  motorReverse(MOTOR_LEFT, moveSpeed);
  motorReverse(MOTOR_RIGHT, moveSpeed);  
}

void moveRotate(int angle)
{
  moveRotateWatchful(angle, ignoreWatch);   
}

void moveStop()
{
  Serial.println("moveStop");
  changeMoveState(MOV_STOP);
  motorStop(MOTOR_LEFT);
  motorStop(MOTOR_RIGHT);
}

void moveBrake()
{
  Serial.println("moveBrake");
#ifndef NO_ENCODER  
  encoderBrake();
#endif  
  changeMoveState(MOV_STOP);
  motorBrake(MOTOR_LEFT);
  motorBrake(MOTOR_RIGHT);
}

void moveSetSpeed(int speed)
{
  motorSetSpeed(MOTOR_LEFT, speed) ;
  motorSetSpeed(MOTOR_RIGHT, speed) ;
  moveSpeed = speed; // save the value
}

int moveGetSpeed()
{
  return moveSpeed; 
}

// returns false if speed already at MIN_SPEED
bool moveSlower(int decrement)
{
  Serial.print(" Slower: "); 
  if( moveSpeed == MIN_SPEED)
     return false;
  if( moveSpeed >= speedIncrement + MIN_SPEED)
    moveSpeed -= speedIncrement;     
  else moveSpeed = MIN_SPEED;
  moveSetSpeed(moveSpeed); 
  return true;
}


// returns false if speed already at max
bool moveFaster(int increment)
{
  Serial.print(" Faster: ");
  if( moveSpeed == 100)
     return false;
  moveSpeed += speedIncrement; 
  if(moveSpeed > 100)
    moveSpeed = 100;
  moveSetSpeed(moveSpeed);      
  return true;
}

int moveGetState()
{
  return moveState;  
}

// this is the low level movement state.
// it will differ from the command state when the robot is avoiding obstacles
void changeMoveState(int newState)
{
  if(newState != moveState)
  {
    Serial.print("Changing move state from "); 
    Serial.print( states[moveState]);
    Serial.print(" to "); 
    Serial.println(states[newState]);
    moveState = newState;
  } 
}


// return the time in milliseconds to move the given distance in mm at the given speed
unsigned long moveDistanceToTime( unsigned long distance, int speed)
{
int fullRotationTime;
unsigned long result;

  if(distance >= MAX_DISTANCE_ARG)   
     return MAX_TIME_ARG;
	 
  if(speed < MIN_SPEED)
    return 0; // ignore speeds slower then the first table entry
	
 // TODO - Daobot: 
  //return (distance * msPerCM * 10L ) ;  // does not take speed into account

  if(speed >= 100)
    fullRotationTime = rotationTime[NBR_SPEEDS-1]; // the last entry is 100%
  else
  { 
    int index = (speed - MIN_SPEED) / SPEED_TABLE_INTERVAL ; // index into speed and time tables
    int t0 =  rotationTime[index];
    int t1 = rotationTime[index+1];    // time of the next higher speed 
    fullRotationTime = map(speed,  speedTable[index],  speedTable[index+1], t0, t1);  
   // Serial.print("index= ");  Serial.print(index); Serial.print(", t0 = ");  Serial.print(t0);  Serial.print(", t1 = ");  Serial.print(t1); 
   // Serial.print(" full rotation time = ");  Serial.println(time);
  } 
  result = (distance * fullRotationTime) / mmPer360; 
  return result; 
}


// private generic move function 
bool MoveWatchful( unsigned long distanceCM, unsigned long duration, bool (*watchFunc)(void))
{
#ifndef NO_ENCODER 
   encoderMoveWatchful(distanceCM*10, MAX_TIME, watchFunc);  // encoder units are MM
#else
  unsigned long startTime = millis();

  if(moveGetSpeed() < MIN_SPEED)    
    return true; // ignore speeds too slow to move
     

  while(true) // do until completed movement or aborted by watch function 
  {
    if(watchFunc() == true) // check for obstacles, abort if func returns true
    {
     // fp("stopped by watchFunc");
      return true;
    }    
    if( millis() - startTime > duration){  
     // fp("stopped by duration\r\n");
      break;
    } 
  }
  moveBrake();
  return false;
#endif   
}

/************* high level movement functions using encoder **********/

// calls given watch function, returns true of obstacle found
bool delayWatchful(unsigned long interval, bool (*watchFunc)(void))
{
  unsigned long oldCount;  
  unsigned long myDur;
  unsigned long start = millis();
  while(millis() - start <= interval) {
    // todo - service encoder
    if( watchFunc())
      return true;
  }
  return false;
}
 
bool moveForwardWatchful( unsigned long distanceCM, bool (*watchFunc)(void))
{  
  moveForward();
#ifdef NO_ENCODER
   unsigned long dur = moveDistanceToTime(distanceCM*10, moveGetSpeed());
   bool result = MoveWatchful(distanceCM, dur, watchFunc);  
#else  
  bool result = MoveWatchful(distanceCM, MAX_TIME, watchFunc); 
#endif    
  moveBrake();
  return result;
}

bool moveReverseWatchful( unsigned long distanceCM, bool (*watchFunc)(void))
{
  moveBackward();
#ifdef NO_ENCODER
   unsigned long dur = motorDistanceMMToTime(distanceCM *10);
   bool result = MoveWatchful(distanceCM, dur, watchFunc);  
#else  
  bool result = MoveWatchful(distanceCM, MAX_TIME, watchFunc); 
#endif 
  moveBrake();
  return result;
}

bool moveRotateWatchful( int angle, bool (*watchFunc)(void))
{  
  bool result = false;  // set to true if stopped in watch function
 
  changeMoveState(MOV_ROTATE);
  Serial.print("Rotating ");  

  int distanceMM = 0;
  if( angle < 0)
  {
    distanceMM = map(-angle, 0,360, 0, WHEEL_TRAVEL_PER_360);
#ifdef NO_ENCODER
   unsigned long dur = moveDistanceToTime(distanceMM,moveSpeed);
#else
  unsigned long dur = MAX_TIME;
#endif  	
    Serial.print("angle = "); 
    Serial.print(angle); 
    Serial.print(", dist = "); 
    Serial.println(distanceMM);
    motorReverse(MOTOR_LEFT, moveSpeed);
    motorForward(MOTOR_RIGHT, moveSpeed);
    result = MoveWatchful(distanceMM, dur, watchFunc);
  }
  else if(angle > 0)
  {
    distanceMM = map(angle, 0,360, 0, WHEEL_TRAVEL_PER_360); 
#ifdef NO_ENCODER
  unsigned long dur = motorDistanceMMToTime(distanceMM);
#else
  unsigned long dur = MAX_TIME;
#endif  	
    Serial.print("angle = "); 
    Serial.print(angle); 
    Serial.print(", dist = "); 
    Serial.println(distanceMM);
    motorForward(MOTOR_LEFT, moveSpeed);
    motorReverse(MOTOR_RIGHT, moveSpeed);
    result = MoveWatchful(distanceMM, dur, watchFunc);
  }
  moveBrake();
  return  result;
}

bool moveForwardTimedWatchful( unsigned long durationMS, bool (*watchFunc)(void))
{
  moveForward();
  bool result = MoveWatchful(MAX_DISTANCE, durationMS, watchFunc); 
  moveBrake();
  return result;
}

bool moveReverseTimedWatchful( unsigned long durationMS, bool (*watchFunc)(void))
{
  moveBackward();
  bool result = MoveWatchful(MAX_DISTANCE, durationMS, watchFunc); 
  moveBrake();
  return result;
}

// don't bother watching
bool ignoreWatch()
{
  return false;  
}





