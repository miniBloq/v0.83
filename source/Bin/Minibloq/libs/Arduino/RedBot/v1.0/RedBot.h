#ifndef RedBot_h
#define RedBot_h

#include <Arduino.h>

// Pin aliases for the motor controller.
#define    L_CTRL_1   2
#define    L_CTRL_2   4
#define    PWM_L      5

#define    R_CTRL_1   7
#define    R_CTRL_2   8
#define    PWM_R      6

// PCINT functionality aliases. Each PCINT has a value set up when the
//  class member gets created, and the PCINT service routine will handle
//  the choosing the appropriate response to the interrupt.

#define NOT_IN_USE    0
#define WHISKER       1
#define LENCODER      2
#define RENCODER      3
#define SW_SERIAL     4

#define PCINT_A0      0
#define PCINT_A1      1
#define PCINT_A2      2
#define PCINT_A3      3
#define PCINT_A4      4
#define PCINT_A5      5
#define PCINT_3       6
#define PCINT_9       7
#define PCINT_10      8
#define PCINT_11      9

enum WHEEL {LEFT, RIGHT, BOTH}; // Variable for which wheel you're interested in
                  //  when you do things in the encoder class.

const int dcMotMaxPower = 255;
const int dcMotMinPower = -dcMotMaxPower;

// These three functions need to work from within multiple classes, so we keep
//  them separate and add them as friend functions where appropriate.
void setPinChangeInterrupt(int pin, byte role); // The "role" of each pin is
                  //  stored in an array which is accessed in the interrupt
                  //  handler to determine what should be done on a falling edge
                  //  PC interrupt.
void pinFunctionHandler(byte pinIndex); // This is the function which actually
                  //  handles the legwork after the interrupt has identified
                  //  which pin caught the interrupt.
void brake(void); // Globally accessible motor brake. I couldn't figure out how
                  //  to set a function pointer to the RedBotMotor class
                  //  function, and it's a small function, so I just made a
                  //  global in the library.
void PC0Handler(byte PBTemp);
void PC1Handler(byte PCTemp);
void PC2Handler(byte PDTemp);

class RedBotMotor
{
  public:
    RedBotMotor();
    void drive(int speed);
    void pivot(int speed);
    void rightDrive(int speed);
	inline int getRightSpeed() const { return rightSpeed; }
    void leftDrive(int speed);
	inline int getLeftSpeed() const { return leftSpeed; }
    void stop();
    void brake();
    void rightStop();
    void leftStop();
    void leftBrake();
    void rightBrake();
  
  private:
	int leftSpeed, rightSpeed;
	
    void leftFwd(byte speed);
    void leftRev(byte speed);
    void rightFwd(byte speed);
    void rightRev(byte speed);
};

// This is the reflectance sensor used for eg line following and table edge
//  detection. It's pretty crude, but since they're analog sensors, they're
//  kind of hard to work with.
class RedBotSensor
{
  public:
    RedBotSensor(int pin);  // Configure a pin as a sensor.
    int read();             // Return the current value of the pin.
    boolean check();        // In theory, this will be true if a deviation from
                            //  detectLevel is found; false otherwise.
    int setBGLevel();       // You can calibrate the sensor to detect a deviation
    int setDetectLevel();   //  from detectLevel; these functions allow for that.
    boolean calStatus();    // Have both calibrated levels been set yet?
  private:
    int _pin;
    int _BGLevel;
    int _detectLevel;
};


// This handles the physical wire-whisker type bumper.
class RedBotBumper
{
  public:
    RedBotBumper(int pin); // Simple constructor; when the bumper gets hit, the
                           //  motors will stop.
    RedBotBumper(int pin, void(*functionPointer)(void)); // If the user wishes
                           //  to do something other than stop on a whisker,
                           //  bump, they can write a function to do so, and
                           //  use this constructor.
  private:
    void setBumpFunction(int pin, void(*functionPointer)(void));
};


// Handler for encoder sensors. Assume only one of this class is present.
//  When a negative going edge happens on an encoder pin, a counter is
//  incremented (or decremented), depending on the direction last determined
//  by one of the motor direction commands.
class RedBotEncoder
{
  // We declare a couple of friends, so they can have access to the private
  //  members of this class.
  friend class RedBotMotor;  // Needs access to lDir and rDir.
  friend void pinFunctionHandler(byte pinIndex); // Called from within the
                             //  ISRs, this function increments the counts
                             //  by calling wheelTick().
  public:
    RedBotEncoder(int lPin, int rPin); // Constructor. Assigns pins, pin
                             //  functions, zeroes counters, and adds a
                             //  reference to the new encoder object for other
                             //  library members to access.
    void clearEnc(WHEEL wheel); // Zaps the encoder count for a given wheel (or
                             //  for both wheels).
    long getTicks(WHEEL wheel); // Returns the encoder count for a wheel.
  private:
    void wheelTick(WHEEL wheel); // Increment or decrement a wheel's counts,
                             //  depending on which way the motor is turning.
    long lCounts;            // Holds the number of ticks for that wheel's
    long rCounts;            //  encoder.
    char lDir;               // Direction is set by the motor class, according
    char rDir;               //  to what the most recent motion direction for
                             //  the given wheel was.
};


#define XL_ADDR     0x1D // I2C address of the MMA8452Q accelerometer
#define I2C_READ    0x01 // I2C read bit set
// Some values we'll load into TWCR a lot
#define START_COND  0xA4 // (1<<TWINT) | (1<<TWSTA) | (1<<TWEN)
#define STOP_COND   0x94 // (1<<TWINT) | (1<<TWSTO) | (1<<TWEN)
#define CLEAR_TWINT 0x84 // (1<<TWINT) | (1<<TWEN)
#define NEXT_BYTE   0xC4 // (1<<TWINT) | (1<<TWEA) | (1<<TWEN)
class RedBotAccel
{
  public:
    RedBotAccel();
    void read();
    void enableBump();
    boolean checkBump();
    void setBumpThresh(int xThresh);
    int x;
    int y;
    int z;
	inline int readX() { read(); return x; }
	inline int readY() { read(); return y; }
	inline int readZ() { read(); return z; }
  private:
    void xlWriteBytes(byte addr, byte *buffer, byte len);
    void xlReadBytes(byte addr, byte *buffer, byte len);
};

// This is lifted from the SoftwareSerial version that shipped with Arduino
//  v1.0.5. I needed to rework that rather than use the existing SoftwareSerial
//  library because I need to share the pin change ISRs with other classes in
//  this library. Also, because I'm constraining the environment, I can make
//  some optimizations to the code. I'm leaving in the full history, for
//  citation's sake. People should know I didn't do all this.

/*
SoftwareSerial.h (formerly NewSoftSerial.h) - 
Multi-instance software serial library for Arduino/Wiring
-- Interrupt-driven receive and other improvements by ladyada
   (http://ladyada.net)
-- Tuning, circular buffer, derivation from class Print/Stream,
   multi-instance support, porting to 8MHz processors,
   various optimizations, PROGMEM delay tables, inverse logic and 
   direct port writing by Mikal Hart (http://www.arduiniana.org)
-- Pin change interrupt macros by Paul Stoffregen (http://www.pjrc.com)
-- 20MHz processor support by Garrett Mace (http://www.macetech.com)
-- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

The latest version of this library can always be found at
http://arduiniana.org.
*/

#include <inttypes.h>
#include <Stream.h>

/******************************************************************************
* Definitions
******************************************************************************/

#define _SS_MAX_RX_BUFF 64 // RX buffer size
#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

class RedBotSoftwareSerial : public Stream
{
  friend void pinFunctionHandler(byte pinIndex);
  
  public:
    // public methods
    RedBotSoftwareSerial();
    ~RedBotSoftwareSerial();
    void begin(long speed);
    void end();
    bool overflow() { bool ret = _buffer_overflow; _buffer_overflow = false; return ret; }
    int peek();

    virtual size_t write(uint8_t byte);
    virtual int read();
    virtual int available();
    virtual void flush();
    
    using Print::write;
    
  private:
    // per object data
    uint8_t _receivePin;
    uint8_t _receiveBitMask;
    volatile uint8_t *_receivePortRegister;
    uint8_t _transmitBitMask;
    volatile uint8_t *_transmitPortRegister;

    uint16_t _rx_delay_centering;
    uint16_t _rx_delay_intrabit;
    uint16_t _rx_delay_stopbit;
    uint16_t _tx_delay;

    uint16_t _buffer_overflow:1;

    // static data
    static char _receive_buffer[_SS_MAX_RX_BUFF]; 
    static volatile uint8_t _receive_buffer_tail;
    static volatile uint8_t _receive_buffer_head;
    static RedBotSoftwareSerial *active_object;

    // private methods
    void recv();
    uint8_t rx_pin_read();
    void tx_pin_write(uint8_t pin_state);
    void setTX(uint8_t transmitPin);
    void setRX(uint8_t receivePin);

    // private static method for timing
    static inline void tunedDelay(uint16_t delay);

};

// We're going to create a special class now, to interface with the onboard
//  XBee header. Since we've got the option of either software or hardware serial,
//  I'm going to allow the user to choose between modes.

class RedBotRadio
{
  public:
    RedBotRadio();      // Constructor.
  private:
  
};

#endif RedBot_h
