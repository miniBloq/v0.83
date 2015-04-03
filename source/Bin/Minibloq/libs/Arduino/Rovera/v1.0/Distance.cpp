/****************************
   distance sensor code   
****************************/
#include "Distance.h"

/*********************************
 code for official Ping))) distance sensor
**********************************/

// Returns the distance in cm
// this will return 0 if no ping sensor is connected or the distance is greater than around 10 feet
int pingGetDistance(int pingPin)
{
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH, 20000); // if a pulse does not arrive in 20 ms then the ping sensor is not connected
  if(duration >=20000)
    duration = 10000;
  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  return cm;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

/*********************************
 code for HC-SR04 4 pin sonar sensor using single pin
**********************************/
// Returns the distance in cm
// this will return 0 if no ping sensor is connected or the distance is greater than around 10 feet
int HC_SR04GetDistance(int pingPin)
{
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH, 20000); // if a pulse does not arrive in 20 ms then the ping sensor is not connected
  if(duration >=20000)
    duration = 10000;
  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  return cm;
}


/*********************************
 code for sharp IR distance sensor
**********************************/
const long referenceMv = 5000; // the reference voltage in millivoltse

int irGetDistance(byte pin)
{
  int val = analogRead(pin);
  int mV = map(val, 0, 1023, 0 , referenceMv);
  // or: 
  //int mV = (val,  * referenceMv) / 1023;
  
  int cm = mvToDistance(mV);
  return cm;
}

// the following is used to interpolate the distance from a table
// table entries are distances in steps of 250 millivolts
const int TABLE_ENTRIES = 11;
const int firstElement = 250; // first entry is 250 mV
const int INTERVAL  = 250; // millivolts between each element
static int distanceTable[TABLE_ENTRIES] = {200,130,90,64,50,41,35,30,25,20,15};

int mvToDistance(int mV)
{
   if( mV < firstElement )
      return distanceTable[0];
   if( mV >  INTERVAL * TABLE_ENTRIES )
      return distanceTable[TABLE_ENTRIES-1];
   else
   {
      int index = mV / INTERVAL;   // highest table element <= mV value 
      int mV0 =  index * INTERVAL; // mV value of this element
      int mV1 = mV0 + INTERVAL;    // mV value of the next higher element 
      int result = map(mV, mV0, mV1, distanceTable[index-1], distanceTable[index]);  
      result = map(result, 0, 200, 0, 79); // convert from cm to inches 
      return result;
   }
}
