#ifndef Mot8833_h
#define Mot8833_h

/* 	Mot8833

	A small DC motor control class for the DRV8833 motor controller from Pololu (wwww.pololu.com).

	Copyright (C) 2014 miniBloq (http://minibloq.org)
	Developers: Julián U. da Silva Gillig

	IMPORTANT NOTICE: This library is based on code from the SNAPI library, which is under The RobotGroup-Multiplo
	Pacifist License (RMPL). This license is (or may be) not compatible with the GNU Lesser General Public
	License (LGPL), so this notice constitutes an special written exception from miniBloq, to allow deployment
	under the LGPL, in order to be compiled with the Arduino libraries and still hold compatibility	between
	both licenses.
	But this does not affects the SNAPI original license in any way, and DOES NOT AUTHORIZES ANY THIRD PARTY
	TO USE OR REDISTRIBUTE THE SNAPI LIBRARY UNDER THE LGPL. In other words: This exception ONLY AFFECTS the
	library when compiled with Arduino libraries, if and only if, this license (the RMPL) results in some
	inconpatibility with the LGPL (so in this last case, then LGPL applies):

        This library is free software; you can redistribute it and/or
        modify it under the terms of the GNU Lesser General Public
        License as published by the Free Software Foundation; either
        version 2.1 of the License, or (at your option) any later version.

        This library is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
        Lesser General Public License for more details.

    Any way, we strongly believe in peace, so we would not like to see our work in any non-civil or military
    project. But, due to the possible incompatibilty with the LGPL licenses, this may be only a recommendation
    and an expression of our wishes. But, if some clever guy can determine that LGPL and RMPL are compatible
    licenses, then RMPL applies here.
	
	This work uses the DCMotor library, which is under the same license as this library (RMPL with the exception 
	for the LGPL) and its copyright notice is:
	
	Copyright (C) 2010-2013 Multiplo
	http://robotgroup.com.ar
	http://multiplo.org
*/

//#if (ARDUINO < "100")
	#include "Arduino.h"
// #else
	// #include "WProgram.h"
// #endif


const float dcMotMaxPower = 100.0;
const float dcMotMinPower = -dcMotMaxPower;
const int maxPWMValue = 255;


class Mot8833
{
	private:
		bool clockwise;
		float power, prevPower, zeroPowerZone;
		int pin0, pin1;

	public:
		inline Mot8833(const int pin0, const int pin1,
                       const bool clockwise = true) :   clockwise(clockwise),
                                                        power(0.0), prevPower(0.0), zeroPowerZone(0.1),
                                                        pin0(pin0), pin1(pin1)
		{
			pinMode(pin0, OUTPUT);
			pinMode(pin1, OUTPUT);
			digitalWrite(pin0, LOW);
			digitalWrite(pin1, LOW);
			analogWrite(pin0, 0);
			analogWrite(pin1, 0);
		}

		void setClockwise(const bool value)
		{
			clockwise = value;
			setPower(power);
		}

		inline float getZeroPowerZone() const
		{
			return zeroPowerZone;
		}

		inline void setZeroPowerZone(const float value)
		{
			zeroPowerZone = value;
		}
		
		inline bool getClockwise() const
		{
			return clockwise;
		}

		inline float getPower() const
		{
			return power;
		}

		inline float getPrevPower() const
		{
			return prevPower;
		}		

		void setPower(const float value)
		{
			float tempPower = constrain(value, dcMotMinPower, dcMotMaxPower);

			prevPower = power;
			power = tempPower;

			if (!clockwise)
				tempPower = (-tempPower);

			tempPower = (tempPower/dcMotMaxPower)*maxPWMValue;
			if (tempPower > zeroPowerZone) //Margin for floating point: between -zeroPowerZone and +zeroPowerZone is read as zero.
			{
				//Forward:
				analogWrite(pin0, 0);
				analogWrite(pin1, (int)tempPower);
			}
			else if (tempPower < -zeroPowerZone)
			{
				//Reverse:				
				analogWrite(pin0, (int)(-tempPower));
				analogWrite(pin1, 0);
			}
			else //(tempPower == 0)
			{
				//Stop:
				analogWrite(pin0, 0);
				analogWrite(pin1, 0);
			}
		}
};

#endif
