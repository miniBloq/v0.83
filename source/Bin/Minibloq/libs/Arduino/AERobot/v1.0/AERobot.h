#ifndef AERobot_h
#define AERobot_h

//Copyright (C) 2014-2015 Wyss Institute (http://wyss.harvard.edu)
//This file is licensed under a Creative Commons Attribution 4.0 International License. (CC BY 4.0)

#define F_CPU 12000000UL

#define ee_POWER_STATE 0x00
#define ee_move_f_a 0x02
#define ee_move_f_b 0x04
#define ee_move_b_a 0x06
#define ee_move_b_b 0x08
#define ee_rotate_ccw_a 0x0a
#define ee_rotate_ccw_b 0x0c
#define ee_rotate_cw_a 0x0e
#define ee_rotate_cw_b 0x10
#define ee_line_dark 0x12
#define ee_line_light 0x14
 
#include <avr/eeprom.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <math.h>

const int objectThreshold = 500;
const int DELAY_LIM = 5000;
enum MoveDirection {STOP, FORWARD, BACKWARD, TURN_LEFT, TURN_RIGHT};
enum SensorNum {LEFT = 800, CENTER = 600, RIGHT = 150, NONE = 50}; //NONE is a trick for lineFollowing usage.
enum LineReturn{N, L, C, R};
enum Color{BLACK, RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET, WHITE};
enum Bump{NOBUMP, ISBUMP};

void setLED(unsigned char red, unsigned char green, unsigned char blue);
void ledControl(Color color);
void move_forward();
void move_ccw();
void rotate_ccw();
void move_cw();
void rotate_cw();
void move_backward();
void move_stop();
void move(int direction);
void load_calibration();
void initAERobot();
int lightSens(SensorNum num);
int	distSens(SensorNum num);
bool bumpSens(SensorNum num);
LineReturn lineSens();
void Delay(float seconds);

//##Added 2015.03.26-2015.03.30 for miniBloq.v0.82:
void colorLED(Color color);
void colorLED(float R, float G, float B);
bool lineSens(SensorNum sensor);

#endif
