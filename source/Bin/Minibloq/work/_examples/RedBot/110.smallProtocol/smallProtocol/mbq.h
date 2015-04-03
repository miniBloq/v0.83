#ifndef mbq_h
#define mbq_h


#include <Arduino.h>
#include <pins_arduino.h>
#include <stdlib.h>
#include <miniBloq.h>
#include <board.h>
#include <pitches.h>
#include <toneDelay.h>
#include <Servo.h>
#include <RedBot.h>
#include <mbqRedBot.h>
#include <IRRemote.h>
#include <HCSR0x.h>
#include <Ping.h>
#include <IRRanger.h>
#include <RedBot.h>
#include <mbqRedBot.h>


extern Servo servo0;
extern Servo servo1;
extern Servo servo2;
extern Servo servo3;
extern RedBot robot;
extern IRrecv irReceiver;
extern HCSRSensor hcsr;
extern PingSensor ping;
extern IRRanger irRanger20to150;
extern IRRanger irRanger10to80;
extern RedBotAccel accel;

#endif
