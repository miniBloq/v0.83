#ifndef board_h
#define board_h

#define serial0 Serial
#define serial1 Serial1

#define BuzzerPin 22

#define sensor0  0
#define sensor1  1
#define sensor2  2
#define sensor3  3
#define sensor4  4
#define sensor5  5
#define BatteryPin       6
#define TemperaturePin   7
		
#define PWM9    9
#define PWM10  10
#define PWM12  12
#define PWM13  13
#define PWM27  27
#define PWM28  28
		
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
#define D19 19
#define D20 20
#define D21_BiLED 21
#define D22_Speaker 22
#define D23 23
#define D24 24
#define D25 25
#define D26 26
#define D27 27
#define D28 28
#define D29_RunButton 29

void initBoard();

#endif
