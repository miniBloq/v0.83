#ifndef mbq_h
#define mbq_h


#include <Arduino.h>
#include <pins_arduino.h>
#include <stdlib.h>
#include <miniBloq.h>
#include <board.h>
#include <pitches.h>
#include <toneDelay.h>
#include <DCMotor.h>
#include <Servo.h>
#include <IRRemote.h>
#include <Ping.h>
#include <IRRanger.h>


extern DCMotor motor0;
extern DCMotor motor1;
extern Servo servo0;
extern Servo servo1;
extern Servo servo2;
extern IRrecv irReceiver;
extern PingSensor ping;
extern IRRanger irRanger20to150;
extern IRRanger irRanger10to80;

#endif
