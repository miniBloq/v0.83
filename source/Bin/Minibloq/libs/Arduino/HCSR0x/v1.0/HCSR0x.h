#ifndef HCSR_h
#define HCSR_h

/*
   2014.04.21 (yyyy.mm.dd):
   This class is based on the sketch created by David A. Mellis and Tom Igoe (please see notice below).
   The class is a wrapper written by Julián U. da Silva Gillig and is under the RMPL (RobotGroup-Multiplo-Pacifist-License), but
   the code inside the functions is in the public domain (as is the original code from David A. Mellis).
   availablefrom http://minibloq.org.
*/

/* HC-SR0x Sensors
     
   This sketch reads a HC-SR0x ultrasonic rangefinder and returns the
   distance to the closest object in range. To do this, it sends a pulse
   to the sensor to initiate a reading, then listens for a pulse 
   to return.  The length of the returning pulse is proportional to 
   the distance of the object from the sensor.
   
   It was originally created for the PING))) ultrasonic rangefinder.
 
   http://www.arduino.cc/en/Tutorial/Ping
 
   created 3 Nov 2008
   by David A. Mellis
   modified 30 Jun 2009
   by Tom Igoe
 
   This example code is in the public domain.

*/ 
 
#include <Arduino.h>
 
class HCSRSensor
{
  private:
	int _trig, _echo;
    
    long measure()
    {
      // The sensor is triggered by a HIGH pulse of 2 or more microseconds.
      // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
      pinMode(_echo, INPUT);
      pinMode(_trig, OUTPUT);
      digitalWrite(_trig, LOW);
      delayMicroseconds(2);
      digitalWrite(_trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(_trig, LOW);
       
      // The same pin is used to read the signal from the sensor: a HIGH
      // pulse whose duration is the time (in microseconds) from the sending
      // of the sensor to the reception of its echo off of an object.
      return pulseIn(_echo, HIGH);
    }    

  public:
    HCSRSensor(int trig, int echo) : _trig(trig), _echo(echo)
    {
    }

    HCSRSensor(int trig) : _trig(trig), _echo(trig + 1) //Consecutive pins.
    {
    }
    
    ~HCSRSensor()
    {
    }    
    
	inline void setPins(int value) { _trig = value; _echo = _trig + 1; } //Consecutive pins.
	
    inline long measureCM()
    {
      // The speed of sound is 340 m/s or 29 microseconds per centimeter.
      // The sensor travels out and back, so to find the distance of the
      // object we take half of the distance travelled.
      return measure() / 29 / 2;
	  //return measure() / 14.5;
    }
	
    inline long measureCM(int trig)	
    {	
		setPins(trig);
		return measureCM();
	}
    
    inline long measureInches()
    {
		//##See if this is correct for HC-SR0x sensors:
		return measure()/37;
    }
	
    inline long measureInches(int trig)
    {	
		setPins(trig);
		return measureInches();
	}	
};

#endif
