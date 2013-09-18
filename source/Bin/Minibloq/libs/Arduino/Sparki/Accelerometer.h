#ifndef Accelerometer_h
#define Accelerometer_h

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "Arduino.h"

#ifndef WIN32
    // true/false defines
    #define FALSE   0
    #define TRUE    -1
#endif

// Code compatibility to new AVR-libc
// outb(), inb(), inw(), outw(), BV(), sbi(), cbi(), sei(), cli()
#ifndef outb
    #define outb(addr, data)    addr = (data)
#endif
#ifndef inb
    #define inb(addr)           (addr)
#endif
#ifndef BV
    #define BV(bit)         (1<<(bit))
#endif

// TWSR values (not bits)
// (taken from avr-libc twi.h - thank you Marek Michalkiewicz)
// Master
#define TW_START                    0x08
#define TW_REP_START                0x10
// Master Transmitter
#define TW_MT_SLA_ACK               0x18
#define TW_MT_SLA_NACK              0x20
#define TW_MT_DATA_ACK              0x28
#define TW_MT_DATA_NACK             0x30
#define TW_MT_ARB_LOST              0x38
// Master Receiver
#define TW_MR_ARB_LOST              0x38
#define TW_MR_SLA_ACK               0x40
#define TW_MR_SLA_NACK              0x48
#define TW_MR_DATA_ACK              0x50
#define TW_MR_DATA_NACK             0x58
// Slave Transmitter
#define TW_ST_SLA_ACK               0xA8
#define TW_ST_ARB_LOST_SLA_ACK      0xB0
#define TW_ST_DATA_ACK              0xB8
#define TW_ST_DATA_NACK             0xC0
#define TW_ST_LAST_DATA             0xC8
// Slave Receiver
#define TW_SR_SLA_ACK               0x60
#define TW_SR_ARB_LOST_SLA_ACK      0x68
#define TW_SR_GCALL_ACK             0x70
#define TW_SR_ARB_LOST_GCALL_ACK    0x78
#define TW_SR_DATA_ACK              0x80
#define TW_SR_DATA_NACK             0x88
#define TW_SR_GCALL_DATA_ACK        0x90
#define TW_SR_GCALL_DATA_NACK       0x98
#define TW_SR_STOP                  0xA0
// Misc
#define TW_NO_INFO                  0xF8
#define TW_BUS_ERROR                0x00

// defines and constants
#define TWCR_CMD_MASK       0x0F
#define TWSR_STATUS_MASK    0xF8

// return values
#define I2C_OK              0x00
#define I2C_ERROR_NODEV     0x01

#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))

#define WRITE_sda() DDRC = DDRC | 0b00010000 //SDA must be output when writing
#define READ_sda()  DDRC = DDRC & 0b11101111 //SDA must be input when reading - don't forget the resistor on SDA!!


// the SparkFun breakout board defaults to 1, set to 0 if SA0 jumper on the bottom of the board is set
#define SA0 1
#if SA0
  #define MMA8452_ADDRESS 0x1D  // SA0 is high, 0x1C if low
#else
  #define MMA8452_ADDRESS 0x1C
#endif

/* Set the scale below either 2, 4 or 8*/
// Sets full-scale range to +/-2, 4, or 8g. Used to calc real g values.
#define SCALE 2

#define SENSITIVITY_2GS 2
#define SENSITIVITY_2GS 4
#define SENSITIVITY_2GS 8

/* Set the output data rate below. Value should be between 0 and 7*/
#define DATA_RATE 0  // 0=800Hz, 1=400, 2=200, 3=100, 4=50, 5=12.5, 6=6.25, 7=1.56

/* Pin definitions */
#define INT1_PIN    2
#define INT2_PIN    3

class Accelerometer {

public:
    Accelerometer();
    
    void begin();
    
    void readGs(float *);
    
    void readRegisters(uint8_t , uint32_t , uint8_t *);
    void readAccelData(int *);
    void tapHandler();
    void portraitLandscapeHandler();
    void initMMA8452(uint8_t, uint8_t);
    void MMA8452Standby();
    void MMA8452Active();
    uint8_t readRegister(uint8_t);
    void writeRegister(uint8_t, uint8_t);

    uint8_t c;
  
  
private:
    void i2cInit(void);
    void i2cSetBitrate(uint16_t bitrateKHz);
    void i2cSendStart(void);
    void i2cSendStop(void);
    void i2cWaitForComplete(void);
    void i2cSendByte(unsigned char data);
    void i2cReceiveByte(unsigned char ackFlag);
    unsigned char i2cGetReceivedByte(void);
    unsigned char i2cGetStatus(void);
    void delay_ms(uint16_t x);

};

#endif


