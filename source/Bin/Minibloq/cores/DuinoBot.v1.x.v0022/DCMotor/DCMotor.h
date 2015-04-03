#ifndef DCMotor__h
#define DCMotor__h

/* 	DCMotor

	A small DC motor control class.

	Copyright (C) 2010 Multiplo
	http://robotgroup.com.ar
	http://multiplo.org
	Developers: Julián U. da Silva Gillig
	Created: 2010.04.06 (yyyy.mm.dd)
	Last modified: 2010.09.14 (yyyy.mm.dd)

	IMPORTANT NOTICE: This library is based on code from the SNAPI library, which is under The Multiplo
	Pacifist License (MPL). This license is (or may be) not compatible with the GNU Lesser General Public
	License (LGPL), so this notice constitutes an special written exception from Multiplo, to allow deployment
	under the LGPL, in order to be compiled with the Arduino libraries and still hold compatibility	between
	both licenses.
	But this does not affects the SNAPI original license in any way, and DOES NOT AUTHORIZES ANY THIRD PARTY
	TO USE OR REDISTRIBUTE THE SNAPI LIBRARY UNDER THE LGPL. In other words: This exception ONLY AFFECTS the
	library when compiled with Arduino libraries, if and only if, this license (the MPL) results in some
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
    and an expression of our wishes. But, if some clever guy can determine that LGPL and MPL are compatible
    licenses, then MPL applies here. You can contact us at: info_t1@multiplo.com.ar
*/

#include "../WProgram.h"
#include "../pins.h"


const float dcMotMaxSpeed = 100.0;
const float dcMotMinSpeed = -dcMotMaxSpeed;


class DCMotor
{
	private:
		bool clockwise, braked;
		float speed, prevSpeed;
		int enable_pin, d0_pin, d1_pin;

	public:
		inline DCMotor(const int enable_pin,
                       const int d0_pin, const int d1_pin,
                       const bool clockwise = true) :   clockwise(clockwise),
                                                        braked(false),
                                                        speed(0.0), prevSpeed(0.0),
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

		void setSpeed(const float value);
		void brake();
		void setClockwise(const bool value);

		inline bool getClockwise() const
		{
			return clockwise;
		}

		inline float getSpeed() const
		{
			return speed;
		}

		inline float getPrevSpeed() const
		{
			return prevSpeed;
		}

		inline bool isBraked() const
		{
			return braked;
		}
};

extern DCMotor motor0,motor1;
#endif
