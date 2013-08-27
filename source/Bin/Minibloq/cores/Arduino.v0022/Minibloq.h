//Copyright (C) 1997-2011 Julián U. da Silva Gillig - 
//This file is  is distributed under the RobotGroup-Multiplo Pacifist License (RMPL), either version 1.0 of the licence, 
//or (at your option) any later version. 

//##Aux functions for the code generated with Minibloq:

#include "DCMotor.h"
#include "Ping.h"
#include "IRRanger.h"
//#include "pitches.h"
#include "Servo.h"

#define BuzzerPin 12

#define serial0 Serial
#define serial1 Serial1

//Always cast (at least by now):
#define MOD(term1, term2) ( ((int)term1) % ((int)term2) )

#define timeStamp() millis()

#define M_E		2.7182818284590452354
#define M_PI	3.14159265358979323846


IRrecv irReceiver(2);

DCMotor motor0(3, 4, 5);
DCMotor motor1(11, 10, 9);
PingSensor ping(0);
IRRanger irRanger20to150(1, IRRanger::range20to150cm);
IRRanger irRanger10to80(1, IRRanger::range10to80cm);


#define sensor0  A0
#define sensor1  A1
#define sensor2  A2
#define sensor3  A3
#define sensor4  A4
#define sensor5  A5

//Servos:
Servo servo0;
Servo servo1;
Servo servo2;

//##PWM pins:
#define PWM3   3
#define PWM5   5
#define PWM6   6
#define PWM9   9
#define PWM10   10
#define PWM11  11

//##Digital pins:
#define D0  0
#define D1  1
#define D2  2
#define D3  3
#define D4  4
#define D5  5
#define D6  6
#define D7  7
#define D8  8
#define D9  9
#define D10 10
#define D11 11
#define D12 12
#define D13_LED 13
#define D14 14
#define D15 15
#define D16 16
#define D17 17
#define D18 18


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


//##The result will be a float between 0 and 100, but with higher resolution than the resolution reached 
//using random(min, max):
inline float Random()
{
    return (random(0, 10000)*0.01);
}


//##
inline void toneWithDelay(uint8_t _pin, unsigned int frequency, unsigned long duration)
{
    tone(_pin, frequency, duration);
    delay(duration * 1.30);
}


inline uint8_t DigitalRead(uint8_t _pin)
{
    pinMode(_pin, INPUT);
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


long pingMeasureCM(int pin)
{
	if (irReceiver.isEnabledIRIn())
	{
		irReceiver.disableIRIn();
		delay(10);
	}
	return ping.measureCM(pin);
}


void initBoard()
{
    Serial.begin(115200);

    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);

    //##Uses the analog input 0, that may have the same value in some designs, but it's not so prabably:
    randomSeed(analogRead(0));

	//irReceiver.enableIRIn();

	servo0.setPin(6); //This does not attach the servo (so the pin can be used by other library if the servo is not used).
	servo1.setPin(7);
	servo2.setPin(8);
}
