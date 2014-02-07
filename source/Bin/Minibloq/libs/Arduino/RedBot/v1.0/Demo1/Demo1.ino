/***********************************************************************
RedBot Demo1

Created 30 Jul 2013 by Mike Hord @ SparkFun Electronics.

This code is beerware- feel free to make use of it, with or without
attribution, in your own projects. If you find it helpful, buy me a beer
the next time you see me at the local- or better yet, shop SparkFun!

This is a simple line following demonstration using the RedBot.

It uses three of the line detection sensors to follow the line, and the
accelerometer to start the line following behavior.

***********************************************************************/
// Include the libraries. We make a provision for using the Xbee header
//  via software serial to report values, but that's not really used in
//  the code anywhere.
#include <RedBot.h>
#include <SoftwareSerial.h>

// Instantiate the motor control class. This only needs to be done once
//  and indeed SHOULD only be done once!
RedBotMotor motor;

// Instantiate the sensors. Sensors can only be created for analog input
//  pins; the Xbee software serial uses pins A0 and A1 and the
//  accelerometer uses pins A4 and A5.
RedBotSensor lSen = RedBotSensor(A2);
RedBotSensor cSen = RedBotSensor(A3);
RedBotSensor rSen = RedBotSensor(A6);

// Instantiate the accelerometer. It can only be connected to pins A4
//  and A5, since those are the I2C pins for this board.
RedBotAccel xl;

// Create a software serial connection. See the Arduino documentation
//  for more information about this. The pins used here are the hard
//  wired pins the Xbee header connects to.
SoftwareSerial xbee(15, 14);

void setup()
{
  Serial.begin(57600);
  xbee.begin(57600);
  
  // Enable bump detection. Once a bump occurs, xl.checkBump() can be
  //  used to detect it. We'll use that to start moving.
  xl.enableBump();
}

// Variable used to wait on a bump before starting operation.
boolean following = false;

void loop()
{
  // checkBump() checks a register in the accelerometer to see if a
  //  bump has happened since the last time checkBump() was run. If
  //  one HAS occurred (i.e., the user tapped the accelerometer),
  //  start following the line!
  if  (xl.checkBump() && following == false) following = true;

  if (following)
  {
    followNoCal();
  }
}
