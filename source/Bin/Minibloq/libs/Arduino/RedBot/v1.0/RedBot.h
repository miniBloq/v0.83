#ifndef RedBot_h
#define RedBot_h

#include <Arduino.h>

#define    L_CTRL_1   2
#define    L_CTRL_2   4
#define    PWM_L      5

#define    R_CTRL_1   7
#define    R_CTRL_2   8
#define    PWM_R      6


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

class RedBotSensor
{
  public:
    RedBotSensor(int pin);
    int read();
    boolean check();
    int setBGLevel();
    int setDetectLevel();
  private:
    int _pin;
    int _BGLevel;
    int _BGSD;
    int _detectLevel;
    int _detectSD;
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
  private:
    void xlWriteBytes(byte addr, byte *buffer, byte len);
    void xlReadBytes(byte addr, byte *buffer, byte len);
};

#endif RedBot_h
