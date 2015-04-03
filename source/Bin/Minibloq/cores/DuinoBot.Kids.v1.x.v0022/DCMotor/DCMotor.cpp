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

#include "DCMotor.h"


const int maxPWMValue = 255;


void DCMotor::setSpeed(const float value)
{
    float tempSpeed = constrain(value, dcMotMinSpeed, dcMotMaxSpeed);

    prevSpeed = speed;
    speed = tempSpeed;

    if (!clockwise)
        tempSpeed = (-tempSpeed);

    tempSpeed = (tempSpeed/dcMotMaxSpeed)*maxPWMValue;
    if (tempSpeed > 0.0)
    {
        //Forward:
        digitalWrite(d0_pin, HIGH); //First sets the direction.
        digitalWrite(d1_pin, LOW);
        analogWrite(enable_pin, (int)tempSpeed); //Then sets the PWM's duty cycle.
    }
    else if (tempSpeed < 0.0)
    {
        //Reverse:
        digitalWrite(d0_pin, LOW);
        digitalWrite(d1_pin, HIGH);
        analogWrite(enable_pin, (int)(-tempSpeed));
    }
    else //(tempSpeed == 0)
    {
        //Free-running:
        digitalWrite(enable_pin, LOW);  //This line would be enough, but the following ensures
        digitalWrite(d1_pin, LOW);      //the full state of the H-bridge, so if someone writes
        digitalWrite(d1_pin, LOW);      //a HIGH to the enable_pin, the state will be "Braked".
    }
    braked = false;
}


void DCMotor::brake()
{
    digitalWrite(d0_pin, HIGH);
    digitalWrite(d1_pin, HIGH);
    digitalWrite(enable_pin, HIGH); //This stops the analogWrite (PWM) for the enable_pin.
    braked = true;
}


void DCMotor::setClockwise(const bool value)
{
    clockwise = value;
    if (!braked)
        setSpeed(speed);
}

// Preinstantiate Objects //////////////////////////////////////////////////////
DCMotor motor0(M0_EN, M0_D0, M0_D1);
DCMotor motor1(M1_EN, M1_D0, M1_D1);