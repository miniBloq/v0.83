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

// defining the MUX pins
#define MUX_ANALOG		A2 // PF5
#define MUX_A  	        A3 // PF4
#define MUX_B      		A4 // PF1
#define MUX_C        	A5 // PF0

// defining the IR line sensor pins (on the multiplexer)
#define IR_EDGE_LEFT      	LOW, HIGH, LOW	// Mux A2     
#define IR_LINE_LEFT        HIGH, LOW, LOW	// Mux A1     
#define IR_LINE_CENTER      LOW, LOW, LOW	// Mux A0      
#define IR_LINE_RIGHT       HIGH, HIGH, LOW	// Mux A3 
#define IR_EDGE_RIGHT       HIGH, LOW, HIGH // Mux A5       

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

class SparkiClass {

public:
  SparkiClass();

  int ping();
  //void servo(int);
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
  
  int readIR(int);

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

  int ping_single();

/*
 * individual motor control (non-blocking)
 * speed range is percent 0-100
*/ 
  void motorRotate( int motor, int direction,  int speed);
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
private:   
  
  byte stepIndex[2];   
  
  
  void setSteps(uint8_t motor, uint32_t steps);  // sets the number of remaining steps, motor stops when this is 0
  uint32_t getSteps(uint8_t motor ); // returns the number of remaining steps
  static void scheduler();
};

extern SparkiClass Sparki;

#endif

