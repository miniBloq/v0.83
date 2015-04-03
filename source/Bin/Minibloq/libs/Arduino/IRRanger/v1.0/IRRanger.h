#ifndef IRRanger_h
#define IRRanger_h

/* 	IRRanger

	Small IRRanger class (abstract), IRRanger80cm (for use with the GP2Y0A21YK sensor), and IRRanger150cm
	(for the GP2Y0A02YK0F) classes.

	Copyright (C) 2010-2012 Multiplo
	http://robotgroup.com.ar
	http://multiplo.org
	Developers: David Vilaseca, Alan Kharsansky; Julián U. da Silva Gillig
	Created: 2010.06.11 (yyyy.mm.dd)
	Last modified: 2012.01.10 (yyyy.mm.dd)

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

#include <stdint.h>
#include <Arduino.h>


class IRRanger
{
	protected:
		unsigned int analogPin;
		uint8_t readings;
	
		double _A1;
		double _B1;
		double _P1;
	
		double _A2;
		double _B2;
		double _P2;	
		
		unsigned int upperValue;
		unsigned int midValue;
		unsigned int lowerValue;

		int OUT_OF_RANGE;
                
    public:
		enum Ranges
		{
			range10to80cm = 80,
			range20to150cm = 150
		};
	
        inline IRRanger(unsigned int analogPin, Ranges range = range10to80cm, uint8_t readings = 5) :	analogPin(analogPin), 
																										readings(readings),
																										OUT_OF_RANGE(-1)
        {
			setRange(range);
        }		
        //inline virtual ~IRRanger() //##Future: to use if we create the base clase Ranger (for Ping, IRRanger, etc.).
		inline ~IRRanger()
		{
		}

		inline void setPin(int value) { analogPin = value; }
		inline int getPin(int value) const { return analogPin; }

		inline void setReadings(unsigned int value) { readings = value; }
		inline unsigned int getReadings() const { return readings; }

		
        inline uint16_t getRawValue() const
		{
			return analogRead(analogPin);
		}

		inline void setRange(Ranges range)
		{
			if (range == range10to80cm)
			{
				_A1 = 6.2578;
				_B1 = 1.8505;
				_P1 = 10;
				
				_A2 = 8.7819;
				_B2 = 6.0724;
				_P2 = 35;	
				
				upperValue = 515;
				midValue = 167;
				lowerValue = 90;
			}
			else if (range == range20to150cm)
			{
				_A1 = 14.4185;
				_B1 = 1.7244;
				_P1 = 20;
				
				_A2 = 18.2230;
				_B2 = 5.4097;
				_P2 = 70;	
				
				upperValue = 510;
				midValue = 185;
				lowerValue = 101;
			}
		}
		
		inline float measureCM()
		{
			int value = 0;
			int measure = 0;

			if (readings == 0) //Avoids divizion by zero (below).
				readings = 1;

			for (int i = 0; i < readings; i++)
				value += analogRead(analogPin);

			value = value/readings;
			
			if(value > (int)upperValue || value < (int)lowerValue)
				measure=OUT_OF_RANGE; // out of range
			else if(value > (int)midValue)
				measure = (int)((1000.0/value - _B1)*(_A1) + _P1); //f1
			else
				measure = (int)((1000.0/value - _B2)*(_A2) + _P2); //f2 

			return measure;
		}
		
		inline float measureCM(int analogPin)	
		{	
			setPin(analogPin);
			return measureCM();
		}	
		
};

#endif
