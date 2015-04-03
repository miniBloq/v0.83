/**********************
 * code to look for obstacles
 **********************/
 #include "Look.h"
 #include "Move.h"
 #include "RobotDefines.h"
 
 
 
// servo defines
const int sweepServoPin = 9;  // pin connected to servo
const int servoDelay    = 700; // time in ms for servo to move

const int  MIN_DISTANCE = 20;     // robot stops when object is nearer (in cm)
const int  CLEAR_DISTANCE = 50;  // distance in cm considered attractive to move
const int  MAX_RANGE = 200;   // the maximum range of the distance sensor

// servo angles             left, right, center 
const int servoAngles[] = {150,  30,   90};

const int pingPin =  10; // digital pin 8 on daobot

const byte bumpSensorPin[2] = { 10,11};

int lastObstacle = OBST_NONE;  

bool lookStarted = false;

//#define USE_SERVO1
#ifdef USE_SERVO1
#include "..\..\ServoRC\v1.0\Servo.h"
extern Servo servo1;
#else
 #include "SoftServo.h"
#endif

void lookBegin()
{
   
#if (ARDUINO <= 100)
  pinMode(bumpSensorPin[SENSE_LEFT], INPUT);    // initialize sensors
  digitalWrite(bumpSensorPin[SENSE_LEFT], HIGH);    // turn on pull-ups
  pinMode(bumpSensorPin[SENSE_RIGHT], INPUT);    // initialize sensors 
  digitalWrite(bumpSensorPin[SENSE_RIGHT], HIGH);    // initialize sensors 
#else
  pinMode(bumpSensorPin[SENSE_LEFT], INPUT_PULLUP);    // initialize sensors 
  pinMode(bumpSensorPin[SENSE_RIGHT], INPUT_PULLUP);    // initialize sensors 
#endif  
#ifndef USE_SERVO1
  softServoAttach(sweepServoPin);  /// attaches the servo pin to the servo object 
#endif
}

// returns true if the given obstacle is detected
boolean lookForObstacle(int obstacle)
{
/*  
  if(obstacle == OBST_FRONT)
  {
    int d = lookAt(servoAngles[DIR_CENTER]);
   Serial.print("d=");  Serial.print(d); Serial.print ( " is "); Serial.println( d < MIN_DISTANCE);
  }
*/  
  if(lookStarted == false) {
    lookBegin();
	lookStarted = true;	
	Serial.println("look started");
  }
  
/*   
   if(digitalRead(bumpSensorPin[SENSE_LEFT]) == LOW) 
     Serial.println("left obstacle "); 
	if(digitalRead(bumpSensorPin[SENSE_RIGHT]) == LOW) 
     Serial.println("right obstacle "); 
*/  
  switch(obstacle) {
  case  OBST_FRONT_EDGE: 
    return digitalRead(bumpSensorPin[SENSE_LEFT]) == LOW &&
      digitalRead(bumpSensorPin[SENSE_RIGHT]) == LOW; 
  case  OBST_LEFT_EDGE:  
    return digitalRead(bumpSensorPin[SENSE_LEFT]) == LOW; 
  case  OBST_RIGHT_EDGE: 
    return  digitalRead(bumpSensorPin[SENSE_RIGHT]) == LOW; 
  case  OBST_FRONT:  int d = lookAt(servoAngles[DIR_CENTER]); return  d < MIN_DISTANCE;      
  }
  return false; 
}

// returns the distance of objects at the given angle
// this version rotates the robot
int lookAt(int angle)
{
  if(lookStarted == false) {
    lookBegin();
	lookStarted = true;	
	Serial.println("look started");
  }
#ifdef USE_SERVO1  
  servo1.attachAndWrite(angle);
  delay(servoDelay);
#else
  softServoWrite(angle, servoDelay ); // wait for servo to get into position
#endif

  int distance, samples;
  long cume;
  distance = samples = cume = 0;  
  for(int i =0; i < 4; i++)
  {  
    distance = HC_SR04GetDistance(pingPin);
    if(distance > 0)
    {     
      samples++;
      cume+= distance;
    }  
  }    
  if(samples > 0)
    distance = cume / samples;
  else
    distance = 0;  
  //Serial.print(" cm = "); Serial.println(distance);
  if( angle != servoAngles[DIR_CENTER])
  {
    SerialDisplay.print("looking at dir "); 
    SerialDisplay.print(angle), SerialDisplay.print(" distance= "); 
    SerialDisplay.println(distance); 
#ifdef USE_SERVO1  
  servo1.attachAndWrite(servoAngles[DIR_CENTER]);
  delay(servoDelay/2);
#else
    softServoWrite(servoAngles[DIR_CENTER], servoDelay/2);    
#endif	

  } 
  return distance;  
}

// function to check if robot can continue moving in current direction 
// returns true if robot is blocked moving in current direction
// this version only tests for obstacles in front
bool checkMovement()
{
  if(Serial.read() == 'h')
    return true;  // abort on serial command

  bool isBlocked = false; // default return value if no obstacles
  if(moveGetState() == MOV_FORWARD)
  {
    if(lookForObstacle(OBST_FRONT) == true) {    
      Serial.println("Front Blocked"); 
      isBlocked = true;   
    }
    else if(lookForObstacle( OBST_LEFT_EDGE) == true) {    
      Serial.println("Bumped Left Front"); 
      isBlocked = true;   
    }
    else if(lookForObstacle( OBST_RIGHT_EDGE) == true) {    
      Serial.println("Bumped Right Front"); 
      isBlocked = true;   
    }
    else if(lookForObstacle( OBST_FRONT_EDGE) == true) {    
      Serial.println("Bumped Front"); 
      isBlocked = true;   
    }

  }
  return isBlocked; 
}

/*
// Look for and avoid obstacles using servo to scan 
 void roam()
 {
 int wasMoving = moveGetState();  
 int distance = lookAt(servoAngles[DIR_CENTER]);
 if(distance == 0) 
 { 
 moveStop();
 SerialDisplay.println("No front sensor");
 return;  // no sensor
 }
 else if(distance <= MIN_DISTANCE) 
 {
 moveStop();
 //SerialDisplay.print("Scanning:"); 
 int leftDistance  = lookAt(servoAngles[DIR_LEFT]);     
 if(leftDistance > CLEAR_DISTANCE)  {
 //   SerialDisplay.print(" moving left: ");
 moveRotate(-90);
 }
 else {
 delay(500);
 int rightDistance = lookAt(servoAngles[DIR_RIGHT]);
 if(rightDistance > CLEAR_DISTANCE) {
 //  SerialDisplay.println(" moving right: ");
 moveRotate(90);   
 }
 else {
 // SerialDisplay.print(" no clearence : ");
 distance = max( leftDistance, rightDistance);
 if(distance < CLEAR_DISTANCE/2) {
 moveReverseTimedWatchful(1000, checkMovement); // back up for one second  
 moveRotate(-180); // turn around
 }
 else {
 if(leftDistance > rightDistance)
 moveRotate(-90);
 else
 moveRotate(90);   
 }                  
 } 
 }
 }
 lookAt(servoAngles[DIR_CENTER]);
 if(wasMoving == MOV_FORWARD)
 moveForward();  
 }
 */



// robot uses distance and bump sensors to move around and avoid obstacles
void roam()
{
  /// code for roaming around and avoiding obstacles
  if(lookForObstacle(OBST_FRONT) == true) {    
    moveStop();
    Serial.println("Front Blocked"); 
    lastObstacle = OBST_FRONT;
    //ledShowDistance();
    {
      //SerialDisplay.print("Scanning:"); 
      int leftDistance  = lookAt(servoAngles[DIR_LEFT]);     
      if(leftDistance > CLEAR_DISTANCE)  {
        //   SerialDisplay.print(" moving left: ");
        moveRotateWatchful( -90, checkMovement); 
      }
      else {
        delay(500);
        int rightDistance = lookAt(servoAngles[DIR_RIGHT]);
        if(rightDistance > CLEAR_DISTANCE) {
          //  SerialDisplay.println(" moving right: ");
          moveRotateWatchful( 90, checkMovement);  
        }
        else {
          // SerialDisplay.print(" no clearence : ");
          int distance = max( leftDistance, rightDistance);
          if(distance < CLEAR_DISTANCE/2) {
            moveReverseTimedWatchful(1000, checkMovement); // back up for one second  
            moveRotateWatchful(-180, ignoreWatch); ; // turn around
          }
          else {
            if(leftDistance > rightDistance)
              moveRotateWatchful( -90, checkMovement);
            else
               moveRotateWatchful( 90, checkMovement); 
          }                  
        } 
      }

    }   

  }

  if( lookForObstacle(OBST_FRONT_EDGE) == true)
  {
    Serial.println("Bumped Front"); 
    lastObstacle = OBST_FRONT_EDGE;
    moveReverseTimedWatchful(500, checkMovement); 
    moveRotate(120);
    while(lookForObstacle(OBST_FRONT_EDGE) == true )
      moveStop(); // stop motors if still at edge
  }
  else if(lookForObstacle(OBST_LEFT_EDGE) == true)
  {
    Serial.println("Bumped Left Front"); 
    moveReverseTimedWatchful(300, checkMovement); 
    //swerve(speed-20, speed, MOV_BACK, 500);
    if(lastObstacle == OBST_RIGHT_EDGE)
       moveRotateWatchful( 180, checkMovement); // turn around
    else  
       moveRotateWatchful( 45, checkMovement);
    lastObstacle = OBST_LEFT_EDGE;       
  }
  else if(lookForObstacle(OBST_RIGHT_EDGE) == true)  
  {
    Serial.println("Bumped Right Front"); 
    moveReverseTimedWatchful(300, ignoreWatch); 
    //swerve(speed, speed-20, MOV_BACK, 500);
    if(lastObstacle == OBST_LEFT_EDGE)
      moveRotateWatchful( 180, checkMovement);
    else  
        moveRotateWatchful( -45, checkMovement); // turn left 
    lastObstacle = OBST_RIGHT_EDGE;
  }
  else
  {
    //moveSetSpeed(MIN_SPEED+10);
    moveForward();    
  }
  delay(1);  
}


