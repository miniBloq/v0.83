#ifndef minibloq_h
#define minibloq_h

//Copyright (C) 2010-2013 Julián U. da Silva Gillig
//This file is  is distributed under the RobotGroup-Multiplo Pacifist License (RMPL), either version 1.0 of the licence, 
//or (at your option) any later version. 

//Always cast (at least by now):
#define MOD(term1, term2) ( ((int)term1) % ((int)term2) )

#define timeStamp() millis()

#define M_E		2.7182818284590452354
#define M_PI	3.14159265358979323846

#ifndef INPUT_PULLUP
	#define INPUT_PULLUP 0x2
#endif

//Forwared declaration; could not exist and it will be ok (since this function only exist in components with text-based code files:
void go();


//Only functions which reliy on the Arduino core, not on external libraries, are included here:

inline uint8_t boolToPinLevel(bool value)
{
    if (value)
        return HIGH;
    return LOW;
}


inline bool pinLevelToBool(uint8_t value)
{
    //Only returns true if value is HIGH, otherwise, returns false:
    return (value == HIGH);
}


inline bool XOR(bool term1, bool term2) { return term1 != term2; }


//The result will be a float between 0 and 100, but with higher resolution than the resolution reached 
//using random(min, max):
inline float Random()
{
    return (random(0, 10000)*0.01);
}


inline uint8_t DigitalRead(uint8_t _pin)
{
    pinMode(_pin, INPUT_PULLUP);
    return pinLevelToBool(digitalRead(_pin));
}


inline void DigitalWrite(uint8_t _pin, bool value)
{
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, boolToPinLevel(value));
}

inline float AnalogRead(uint8_t _pin)
{
    //##Optimization: This can be precomputed:
    return (((float)analogRead(_pin))*100.0)/1023.0;
}


inline float PulseIn(uint8_t _pin, bool value, float timeOut)
{
	pinMode(_pin, INPUT);
	return pulseIn(_pin, boolToPinLevel(value), (unsigned long) timeOut);
}


inline void AnalogWrite(uint8_t _pin, float value)
{
    float tempValue = constrain(value, 0, 100);
    tempValue = tempValue*2.55; // =tempValue*255/100;
    pinMode(_pin, OUTPUT);
    analogWrite(_pin, (int)tempValue);
}

#endif
