/*
  Robot interface module for minibloq
  Copyright Michael Margolis 2013
  
*/ 

#include "Rovera.h"
#include "Look.cpp"
#include "Move.cpp"
#include "SoftServo.cpp"
#include "Distance.cpp"
#include "RobotMotor.cpp"


#define Left_Bump       OBST_LEFT_EDGE
#define Right_Bump      OBST_RIGHT_EDGE
#define Front_Ping      OBST_FRONT



void robotBegin(int encoder0Pin, int encoder1Pin)
{
  moveBegin();
  lookBegin();
/*  
  if( encoder0Pin >= 0)
     pinMode(encoder0Pin,INPUT); 
  if( encoder1Pin >= 0)	 
    pinMode(encoder1Pin,INPUT);
  // TODO - add code so encoders are turned off if encoder pins are < 0	
  encoderBegin(0,1); // left motor encoder using interrupt 0 , right using int 1
*/  
}

bool moveWatchful( unsigned long distance, unsigned long duration, bool (*watchFunc)(void))
{
   unsigned long startTime = millis();
   while(true) // do until completed movement or aborted by watch function 
   {
     if(watchFunc() == true) // check for obstacles, abort if func returns true
     {
       //fp("stopped by watchFunc");
       return true;
     }     
     if( millis() - startTime > duration){  
       // fp("stopped by duration\r\n");
        break;
    } 
   }
   return false;
}

void robotMoveForward(int mode, unsigned long distanceCM)
{
  //blink(mode, distance);
   if( mode == 0 ) // oblivious, define in minibloq.h
   {
      moveForwardWatchful(distanceCM, ignoreWatch);
   }   
   else if ( mode == 1 ) // watchful
   {   
      moveForwardWatchful(distanceCM, checkMovement);
   }
}

void robotMoveBackward(int mode, unsigned long distanceCM)
{
  //blink(mode, distance);
   if( mode == 0 ) // oblivious, define in minibloq.h
   {
      moveReverseWatchful(distanceCM, ignoreWatch);
   }   
   else if ( mode == 1 ) // watchful
   {   
      moveReverseWatchful(distanceCM, checkMovement);
   }

}

void robotTimedMoveForward(int mode, unsigned long duration)
{
   if( mode == 0 ) // oblivious, define in minibloq.h
   {
      moveForwardTimedWatchful(duration, ignoreWatch);
   }   
   else if ( mode == 1 ) // watchful
   {   
       moveForwardTimedWatchful(duration, checkMovement);
   }
}

void robotTimedMoveBackward(int mode, unsigned long duration)
{
   if( mode == 0 ) // oblivious, define in minibloq.h
   {
      moveReverseTimedWatchful(duration, ignoreWatch);
   }   
   else if ( mode == 1 ) // watchful
   {   
       moveReverseTimedWatchful(duration, checkMovement);
   }

}

void robotMoveRotate(int mode, int angle)
{
  //blink(mode, distance);
   if( mode == 0 ) // oblivious, define in minibloq.h
   {
      moveRotateWatchful(angle, ignoreWatch);
   }   
   else if ( mode == 1 ) // watchful
   {   
      moveRotateWatchful(angle, checkMovement);
   }
}

void robotMoveRotateCCW(int mode, int angle)
{
  robotMoveRotate(mode, -angle);
}

void robotSetSpeed(int speed)
{
   moveSetSpeed(speed);
}

void robotMoveStop()
{
   moveStop();
}

boolean robotLookForObstacle(int obstacle)
{
  // translate obstacle defined in ninibloq.h to that used look.cpp
  if( obstacle == Left_Bump)
     obstacle = OBST_LEFT_EDGE;
  else if(obstacle == Right_Bump) 
    obstacle = OBST_RIGHT_EDGE;
   else if(obstacle == Front_Ping)	
    obstacle = OBST_FRONT; 
 
   return lookForObstacle(obstacle);
} 