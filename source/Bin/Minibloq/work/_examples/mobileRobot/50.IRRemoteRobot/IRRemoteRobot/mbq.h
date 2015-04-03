#ifndef mbq_h
#define mbq_h


#include <Arduino.h>
#include <pins_arduino.h>
#include <stdlib.h>
#include <miniBloq.h>
#include <board.h>
#include <pitches.h>
#include <toneDelay.h>
#include <DCMotorWrap.h>
#include <AFMotor.h>
#include <Servo.h>
#include <IRRemote.h>
#include <HCSR0x.h>
#include <Ping.h>
#include <IRRanger.h>


extern AF_DCMotor afMotor0;
extern AF_DCMotor afMotor1;
extern AF_DCMotor afMotor2;
extern AF_DCMotor afMotor3;
extern DCMotorWrap motor0;
extern DCMotorWrap motor1;
extern DCMotorWrap motor2;
extern DCMotorWrap motor3;
extern Servo servo0;
extern Servo servo1;
extern IRrecv irReceiver;
extern HCSRSensor hcsr;
extern PingSensor ping;
extern IRRanger irRanger20to150;
extern IRRanger irRanger10to80;

#endif
