#ifndef Ping_h
#define Ping_h

/*
   2011.12.23 (yyyy.mm.dd):
   This class is based on the sketch created by David A. Mellis and Tom Igoe (please see notice below).
   The class is a wrapper written by Julián U. da Silva Gillig and is under the RMPL (RobotGroup-Multiplo-Pacifist-License), but
   the code inside the functions is in the public domain (as is the original code from David A. Mellis).
   available from http://robotgroup.com.ar and from http://minibloq.org.
*/

/* Ping))) Sensor
     
   This sketch reads a PING))) ultrasonic rangefinder and returns the
   distance to the closest object in range. To do this, it sends a pulse
   to the sensor to initiate a reading, then listens for a pulse 
   to return.  The length of the returning pulse is proportional to 
   the distance of the object from the sensor.
 
   The circuit:
    * +V connection of the PING))) attached to +5V
    * GND connection of the PING))) attached to ground
    * SIG connection of the PING))) attached to digital pin 7
 
   http://www.arduino.cc/en/Tutorial/Ping
 
   created 3 Nov 2008
   by David A. Mellis
   modified 30 Jun 2009
   by Tom Igoe
 
   This example code is in the public domain.

*/ 
 
#include <Arduino.h>
 
class PingSensor
{
  private:
	int _pin;
    
    long measure()
    {
      // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
      // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
      pinMode(_pin, OUTPUT);
      digitalWrite(_pin, LOW);
      delayMicroseconds(2);
      digitalWrite(_pin, HIGH);
      delayMicroseconds(5);
      digitalWrite(_pin, LOW);
       
      // The same pin is used to read the signal from the PING))): a HIGH
      // pulse whose duration is the time (in microseconds) from the sending
      // of the ping to the reception of its echo off of an object.
      pinMode(_pin, INPUT);
      return pulseIn(_pin, HIGH);
    }    

  public:
    PingSensor(int pin) : _pin(pin)
    {
    }
    
    ~PingSensor()
    {
    }    
    
	inline void setPin(int value) { _pin = value; }
	inline int getPin(int value) const { return _pin; }
	
    inline long measureCM()
    {
      // The speed of sound is 340 m/s or 29 microseconds per centimeter.
      // The ping travels out and back, so to find the distance of the
      // object we take half of the distance travelled.
      return measure() / 29 / 2;
	  //return measure() / 14.5;
    }

    inline long measureCM(int pin)	
    {	
		setPin(pin);
		return measureCM();
	}
    
    inline long measureInches()
    {
      // According to Parallax's datasheet for the PING))), there are
      // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
      // second).  This gives the distance travelled by the ping, outbound
      // and return, so we divide by 2 to get the distance of the obstacle.
      // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
      return measure()/37;
    }
	
    inline long measureInches(int pin)	
    {	
		setPin(pin);
		return measureInches();
	}	
};

#endif
