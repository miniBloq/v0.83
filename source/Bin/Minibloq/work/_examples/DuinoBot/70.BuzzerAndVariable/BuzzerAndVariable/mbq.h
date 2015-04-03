#ifndef mbq_h
#define mbq_h


#include <Arduino.h>
#include <pins_arduino.h>
#include <stdlib.h>
#include <miniBloq.h>
#include <board.h>
#include <pitches.h>
#include <toneDelay.h>
#include <Mot8833.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include <IRRemote.h>
#include <HCSR0x.h>
#include <Ping.h>
#include <IRRanger.h>


extern Mot8833 motor0;
extern Mot8833 motor1;
extern Servo servo0;
extern Servo servo1;
extern LiquidCrystal textLCD;
extern IRrecv irReceiver;
extern HCSRSensor hcsr;
extern PingSensor ping;
extern IRRanger irRanger20to150;
extern IRRanger irRanger10to80;

#endif
