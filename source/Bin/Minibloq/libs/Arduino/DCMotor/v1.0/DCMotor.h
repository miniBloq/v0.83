#ifndef DCMotor_h
#define DCMotor_h

/* 	DCMotor

	A small DC motor control class.

	Copyright (C) 2010-2013 Multiplo
	http://robotgroup.com.ar
	http://multiplo.org
	Developers: Julián U. da Silva Gillig
	Created: 2010.04.06 (yyyy.mm.dd)
	Last modified: 2013.09.15 (yyyy.mm.dd)

	IMPORTANT NOTICE: This library is based on code from the SNAPI library, which is under The RobotGroup-Multiplo
	Pacifist License (RMPL). This license is (or may be) not compatible with the GNU Lesser General Public
	License (LGPL), so this notice constitutes an special written exception from Multiplo, to allow deployment
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
    licenses, then RMPL applies here. You can contact us at: info_t1@multiplo.com.ar
*/

//#if (ARDUINO < "100")
	#include "Arduino.h"
// #else
	// #include "WProgram.h"
// #endif


const float dcMotMaxPower = 100.0;
const float dcMotMinPower = -dcMotMaxPower;


class DCMotor
{
	private:
		bool clockwise, braked;
		float power, prevPower, zeroPowerZone;
		int enable_pin, d0_pin, d1_pin;

	public:
		inline DCMotor(const int enable_pin,
                       const int d0_pin, const int d1_pin,
                       const bool clockwise = true) :   clockwise(clockwise),
                                                        braked(false),
                                                        power(0.0), prevPower(0.0), zeroPowerZone(0.1),
                                                        enable_pin(enable_pin),
                                                        d0_pin(d0_pin), d1_pin(d1_pin)
		{
			pinMode(enable_pin, OUTPUT);
			pinMode(d0_pin, OUTPUT);
			pinMode(d1_pin, OUTPUT);
			digitalWrite(enable_pin, LOW);
			digitalWrite(d0_pin, LOW);
			digitalWrite(d1_pin, LOW);
		}

		void setPower(const float value);
		void brake();
		void setClockwise(const bool value);

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

		inline bool isBraked() const
		{
			return braked;
		}
};

#endif
