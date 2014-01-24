#ifndef Sparki_h
#define Sparki_h

#include "Accelerometer.h"

#define SHIFTREG_LATCH      TXLED0   // PD5
#define STATUS_LED          13        
#define BUZZER              11       // PB7
#define ULTRASONIC_ECHO     5        // PC6
#define ULTRASONIC_TRIG     10       // PB6
#define IR_RECEIVE          7        // PE6
#define IR_SEND             6        // PD7

#define SERVO               9        // 
#define SERVO_LEFT          -90
#define SERVO_CENTER        0
#define SERVO_RIGHT         90

// defining the MUX pins
#define MUX_ANALOG		A2 // PF5
#define MUX_A  	        A3 // PF4
#define MUX_B      		A4 // PF1
#define MUX_C        	A5 // PF0

// defining the IR line sensor pins (on the multiplexer)
#define IR_EDGE_RIGHT      LOW, HIGH, LOW	// Mux A2     
#define IR_LINE_RIGHT      HIGH, LOW, LOW	// Mux A1     
#define IR_LINE_CENTER     LOW, LOW, LOW	// Mux A0      
#define IR_LINE_LEFT       HIGH, HIGH, LOW	// Mux A3 
#define IR_EDGE_LEFT       HIGH, LOW, HIGH // Mux A5       

// defining the light sensors
#define LIGHT_RIGHT         HIGH, HIGH, HIGH // Mux A7
#define LIGHT_CENTER        LOW, HIGH, HIGH	 // Mux A6
#define LIGHT_LEFT          LOW, LOW, HIGH	 // Mux A4

// defining battery monitor constants
#define BATTERY_MONITOR     A10       // PB6
#define ADC_REFERENCE       2.56
#define VOLTAGE_DIVIDER     4.7  	 // using 47k and 10k resistors
#define MAX_BAT_V          (VOLTAGE_DIVIDER * ADC_REFERENCE) 

// defining the buttons on the shipped-with remote control
#define REMOTE_ERROR    -2
#define REMOTE_REPEAT   -3
#define REMOTE_POWER    69
#define REMOTE_MENU     71
#define REMOTE_TEST     68
#define REMOTE_PLUS     64
#define REMOTE_BACK     67
#define REMOTE_REWIND   7
#define REMOTE_PLAY     21
#define REMOTE_FF       9
#define REMOTE_0        22
#define REMOTE_MINUS    25
#define REMOTE_C        13
#define REMOTE_1        12
#define REMOTE_2        24
#define REMOTE_3        94
#define REMOTE_4        8
#define REMOTE_5        28
#define REMOTE_6        90
#define REMOTE_7        66
#define REMOTE_8        82
#define REMOTE_9        74

#define STEP_DELAY      1000

// define the shift registers pin output values for the RGB arrays
#define RGB_R 0x01 // pin value of the Red LED on the RGB on the shift register
#define RGB_G 0x02 // pin value of the Green LED on the RGB on the shift register
#define RGB_B 0x04 // pin value of the Blue LED on the RGB on the shift register
#define RGB_SHIFT 1 // which shift register the RGB is on (starts at 0)

#define RED 255,0,0
#define GREEN 0,255,0
#define BLUE 0,0,255

//Forwared declaration; could not exist and it will be ok (since this function only exist in components with text-based code files:
void go();

// properties about the robot in cm
const float WHEEL_DIAMETER_CM     = 5.15;
const float WHEEL_CIRCUMFERENCE_CM = WHEEL_DIAMETER_CM * PI;
const float TRACK_WIDTH            = 11.1;              //tyre seperation in cm  
//const float WHEEL_TRAVEL_PER_360   = WHEEL_TRACK * PI;  // cm per ROBOT revolution 

const int   STEPS_PER_REV      = 8192; // steps for wheels to revolve 360 degrees


const float STEPS_PER_CM       = STEPS_PER_REV / WHEEL_CIRCUMFERENCE_CM; // linear movement
const float STEPS_PER_ROTATION = (TRACK_WIDTH / WHEEL_DIAMETER_CM) * STEPS_PER_REV ;  // robot rotation
const float STEPS_PER_DEGREE   = STEPS_PER_ROTATION / 360.0;         // robot rotation
const float CM_PER_DEGREE      = WHEEL_CIRCUMFERENCE_CM / 360.0;     // wheel movement per degree rotation of robot 

// circumference = 16.18


// defines for left and right motors
#define MOTOR_LEFT  0
#define MOTOR_RIGHT 1
#define MOTOR_GRIPPER 2

// defines for direction
#define DIR_CCW -1
#define DIR_CW   1


//includes for the LCD 

#define swap(a, b) { uint8_t t = a; a = b; b = t; }

#define BLACK 1
#define WHITE 0

#define LCDWIDTH 128
#define LCDHEIGHT 64

#define CMD_DISPLAY_OFF   0xAE
#define CMD_DISPLAY_ON    0xAF

#define CMD_SET_DISP_START_LINE  0x60
#define CMD_SET_PAGE  0xB0

#define CMD_SET_COLUMN_UPPER  0x10
#define CMD_SET_COLUMN_LOWER  0x00

#define CMD_SET_ADC_NORMAL  0xA0
#define CMD_SET_ADC_REVERSE 0xA1

#define CMD_SET_DISP_NORMAL 0xA6
#define CMD_SET_DISP_REVERSE 0xA7

#define CMD_SET_ALLPTS_NORMAL 0xA4
#define CMD_SET_ALLPTS_ON  0xA5
#define CMD_SET_BIAS_9 0xA2 
#define CMD_SET_BIAS_7 0xA3

#define CMD_RMW  0xE0
#define CMD_RMW_CLEAR 0xEE
#define CMD_INTERNAL_RESET  0xE2
#define CMD_SET_COM_NORMAL  0xC0
#define CMD_SET_COM_REVERSE  0xC8
#define CMD_SET_POWER_CONTROL  0x28
#define CMD_SET_RESISTOR_RATIO  0x20
#define CMD_SET_VOLUME_FIRST  0x81
#define  CMD_SET_VOLUME_SECOND  0
#define CMD_SET_STATIC_OFF  0xAC
#define  CMD_SET_STATIC_ON  0xAD
#define CMD_SET_STATIC_REG  0x0
#define CMD_SET_BOOSTER_FIRST  0xF8
#define CMD_SET_BOOSTER_234  0
#define  CMD_SET_BOOSTER_5  1
#define  CMD_SET_BOOSTER_6  3
#define CMD_NOP  0xE3
#define CMD_TEST  0xF0

#define LCD_A0 4
#define LCD_RST 12
#define LCD_CS 17

// SPI definitions
#define SPI_MODE_MASK 0x0C  // CPOL = bit 3, CPHA = bit 2 on SPCR
#define SPI_CLOCK_MASK 0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01  // SPI2X = bit 0 on SPSR
#define SPI_CLOCK_DIV2 0x04

class SparkiClass {

public:
  SparkiClass();
  int ping_single();
  int ping();
  void begin();
  void beep();
  void RGB(uint8_t,uint8_t,uint8_t);

  void setMux(uint8_t, uint8_t, uint8_t);
/*
* Light level sensors
*/
  int lightRight();
  int lightCenter();
  int lightLeft();

/*
* Infrared line sensors
*/
  int edgeRight();
  int lineRight();
  int lineCenter();
  int lineLeft();  
  int edgeLeft();
  
  int readSensorIR(int);

/*
* Infrared Remote sensor
*/
  int readIR();

/*
* Infrared Remote sensor
*/
  int writeIR();

/*
* Servo Functions
*/
void startServoTimer();
void writeServo(int);

/*
 * high-level move functions
*/ 
  void moveForward(float);
  void moveForward();
  
  void moveBackward(float);
  void moveBackward();
  
  void moveLeft(float);
  void moveLeft();
  
  void moveRight(float);
  void moveRight();
  
  void moveStop();
   
  void gripOpen();
  void gripClose();
  void gripStop();

/*
 * individual motor control (non-blocking)
 * speed range is percent 0-100
*/ 
  inline void setMotorInvertedSpeedSign(bool value) { motorInvertedSpeedSign = value; } //miniBloq.
  inline bool getMotorInvertedSpeedSign() const { return motorInvertedSpeedSign; } //miniBloq.
  void motorRotate( int motor, int direction,  int speed);
  void motorRotate( int motor, int speed); //miniBloq.
  void motorStop(int motor);


/*
 * combined motor control using step count
 * this function blocks by default but returns after starting motors if wait = false
 */
  void motorsRotateSteps( int leftDir, int rightDir,  int speed, uint32_t steps, bool wait= true);
 
/*
 * returns true if one or both motors a still stepping
 */
 bool areMotorsRunning();
   
 void onGrabIR();
 void offGrabIR();
   
 void onIR();
 void offIR();



// Display Functions
  void st7565_init(void);
  void beginDisplay();
  void st7565_command(uint8_t c);
  void st7565_data(uint8_t c);
  void st7565_set_brightness(uint8_t val);
  void clear_display(void);
  void clear();
  void display();

  void setpixel(uint8_t x, uint8_t y, uint8_t color);
  uint8_t getpixel(uint8_t x, uint8_t y);
  void fillcircle(uint8_t x0, uint8_t y0, uint8_t r, 
		  uint8_t color);
  void drawcircle(uint8_t x0, uint8_t y0, uint8_t r, 
		  uint8_t color);
  void drawrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
		uint8_t color);
  void fillrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, 
		uint8_t color);
  void drawline(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, 
		uint8_t color);
  void drawchar(uint8_t x, uint8_t line, char c);
  void drawstring(uint8_t x, uint8_t line, char *c);
  void drawstring_P(uint8_t x, uint8_t line, const char *c);

  void drawbitmap(uint8_t x, uint8_t y, 
		  const uint8_t *bitmap, uint8_t w, uint8_t h,
		  uint8_t color);


private:   
  
  byte stepIndex[2];
  bool motorInvertedSpeedSign; //miniBloq.
  
  
  void setSteps(uint8_t motor, uint32_t steps);  // sets the number of remaining steps, motor stops when this is 0
  uint32_t getSteps(uint8_t motor ); // returns the number of remaining steps
  static void scheduler();

// Display Functions
  int8_t sid, sclk, a0, rst, cs;
  void startSPI();
  void spiwrite(uint8_t c);
  void my_setpixel(uint8_t x, uint8_t y, uint8_t color);
};

extern SparkiClass Sparki;

#endif

