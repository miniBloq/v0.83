#ifndef __PINS__H_
#define __PINS__H_

//Screen col pins

const static uint8_t COL_1 = 19;
const static uint8_t COL_2 = 6;
const static uint8_t COL_3 = 5;
const static uint8_t COL_4 = 24;
const static uint8_t COL_5 = 7;
const static uint8_t COL_6 = 2;
const static uint8_t COL_7 = 9;
const static uint8_t COL_8 = 10;

//Sensors
//##Ver con Alan estos alias:
//const static uint8_t S0 = 14;
//const static uint8_t S1 = 15;
//const static uint8_t S2 = 16;
//const static uint8_t S3 = 17;
//const static uint8_t S4 = 18;

//Screen row pins (via mux)
const static uint8_t ROW_1 = 13;
const static uint8_t ROW_2 = 11;
const static uint8_t ROW_3 = 12;

//Motors
const static uint8_t M0_EN = 22;
const static uint8_t M0_D0 = 20;
const static uint8_t M0_D1 = 21;

const static uint8_t M1_EN = 3;
const static uint8_t M1_D0 = 4;
const static uint8_t M1_D1 = 8;

//Periphericals
const static uint8_t RUN_SW = 25;
const static uint8_t BUZZER = 23;

#endif
