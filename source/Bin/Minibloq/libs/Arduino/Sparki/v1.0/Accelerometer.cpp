#include "Accelerometer.h"

Accelerometer::Accelerometer()
{

}

void Accelerometer::begin()
{
  
  pinMode(INT1_PIN, INPUT);
  digitalWrite(INT1_PIN, LOW);
  pinMode(INT2_PIN, INPUT);
  digitalWrite(INT2_PIN, LOW);
  
  uint8_t c;
  
  /* Read the WHO_AM_I register, this is a good test of communication */
  c = readRegister(0x0D);  // Read WHO_AM_I register
  if (c == 0x2A) // WHO_AM_I should always be 0x2A
  {  
    initMMA8452(SCALE, DATA_RATE);  // init the accelerometer if communication is OK
    Serial.println("MMA8452Q is online...");
  }
  else
  {
    Serial.print("Could not connect to MMA8452Q: 0x");
    Serial.println(c, HEX);
    while (1)  // Loop forever if communication doesn't happen
      ;
  }
}

void Accelerometer::readGs(float * accelG)
{
    int accelCount[3]; 
    readAccelData(accelCount);
    for (int i=0; i<3; i++){
      accelG[i] = (float) accelCount[i]/((1<<12)/(2*SCALE));  // get actual g value, this depends on scale being set
    }
}

void Accelerometer::readRegisters(uint8_t address, uint32_t i, uint8_t * dest)
{
  i2cSendStart();
  i2cWaitForComplete();
  
  i2cSendByte((MMA8452_ADDRESS<<1));    // write 0xB4
  i2cWaitForComplete();
  
  i2cSendByte(address); // write register address
  i2cWaitForComplete();
  
  i2cSendStart();
  i2cSendByte((MMA8452_ADDRESS<<1)|0x01);   // write 0xB5
  i2cWaitForComplete();
  for (int j=0; j<i; j++)
  {
    i2cReceiveByte(TRUE);
    i2cWaitForComplete();
    dest[j] = i2cGetReceivedByte(); // Get MSB result
  }
  i2cWaitForComplete();
  i2cSendStop();
  
  cbi(TWCR, TWEN);  // Disable TWI
  sbi(TWCR, TWEN);  // Enable TWI
}

void Accelerometer::readAccelData(int * destination)
{
  byte rawData[6];  // x/y/z accel register data stored here

  readRegisters(0x01, 6, &rawData[0]);  // Read the six raw data registers into data array
  
  /* loop to calculate 12-bit ADC and g value for each axis */
  for (int i=0; i<6; i+=2)
  {
    destination[i/2] = ((rawData[i] << 8) | rawData[i+1]) >> 4;  // Turn the MSB and LSB into a 12-bit value
    if (rawData[i] > 0x7F)
    {  // If the number is negative, we have to make it so manually (no 12-bit data type)
      destination[i/2] = ~destination[i/2] + 1;
      destination[i/2] *= -1;  // Transform into negative 2's complement #
    }
  }
}

/* This function will read the status of the tap source register.
   And print if there's been a single or double tap, and on what
   axis. */
void Accelerometer::tapHandler()
{
  byte source = readRegister(0x22);  // Reads the PULSE_SRC register
  
  if ((source & 0x10)==0x10)  // If AxX bit is set
  {
    if ((source & 0x08)==0x08)  // If DPE (double puls) bit is set
      Serial.print("    Double Tap (2) on X");  // tabbing here for visibility
    else
      Serial.print("Single (1) tap on X");
      
    if ((source & 0x01)==0x01)  // If PoIX is set
      Serial.println(" +");
    else
      Serial.println(" -");
  }
  if ((source & 0x20)==0x20)  // If AxY bit is set
  {
    if ((source & 0x08)==0x08)  // If DPE (double puls) bit is set
      Serial.print("    Double Tap (2) on Y");
    else
      Serial.print("Single (1) tap on Y");
      
    if ((source & 0x02)==0x02)  // If PoIY is set
      Serial.println(" +");
    else
      Serial.println(" -");
  }
  if ((source & 0x40)==0x40)  // If AxZ bit is set
  {
    if ((source & 0x08)==0x08)  // If DPE (double puls) bit is set
      Serial.print("    Double Tap (2) on Z");
    else
      Serial.print("Single (1) tap on Z");
    if ((source & 0x04)==0x04)  // If PoIZ is set
      Serial.println(" +");
    else
      Serial.println(" -");
  }
}

/* This function will read the p/l source register and
   print what direction the sensor is now facing */
void Accelerometer::portraitLandscapeHandler()
{
  byte pl = readRegister(0x10);  // Reads the PL_STATUS register
  switch((pl&0x06)>>1)  // Check on the LAPO[1:0] bits
  {
    case 0:
      Serial.print("Portrait up, ");
      break;
    case 1:
      Serial.print("Portrait Down, ");
      break;
    case 2:
      Serial.print("Landscape Right, ");
      break;
    case 3:
      Serial.print("Landscape Left, ");
      break;
  }
  if (pl&0x01)  // Check the BAFRO bit
    Serial.print("Back");
  else
    Serial.print("Front");
  if (pl&0x40)  // Check the LO bit
    Serial.print(", Z-tilt!");
  Serial.println();
}

/* Initialize the MMA8452 registers 
   See the many application notes for more info on setting 
   all of these registers:
   http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=MMA8452Q
   
   Feel free to modify any values, these are settings that work well for me.
*/
void Accelerometer::initMMA8452(uint8_t fsr, uint8_t dataRate)
{
  MMA8452Standby();  // Must be in standby to change registers
  
  /* Set up the full scale range to 2, 4, or 8g. */
  if ((fsr==2)||(fsr==4)||(fsr==8))
    writeRegister(0x0E, fsr >> 2);  
  else
    writeRegister(0x0E, 0);
    
  /* Setup the 3 data rate bits, from 0 to 7 */
  writeRegister(0x2A, readRegister(0x2A) & ~(0x38));
  if (dataRate <= 7)
    writeRegister(0x2A, readRegister(0x2A) | (dataRate << 3));  
    
  /* Set up portrait/landscap registers - 4 steps:
     1. Enable P/L
     2. Set the back/front angle trigger points (z-lock)
     3. Set the threshold/hysteresis angle
     4. Set the debouce rate
  // For more info check out this app note: http://cache.freescale.com/files/sensors/doc/app_note/AN4068.pdf */
  writeRegister(0x11, 0x40);  // 1. Enable P/L
  writeRegister(0x13, 0x44);  // 2. 29deg z-lock (don't think this register is actually writable)
  writeRegister(0x14, 0x84);  // 3. 45deg thresh, 14deg hyst (don't think this register is writable either)
  writeRegister(0x12, 0x50);  // 4. debounce counter at 100ms (at 800 hz)
  
  /* Set up single and double tap - 5 steps:
     1. Set up single and/or double tap detection on each axis individually.
     2. Set the threshold - minimum required acceleration to cause a tap.
     3. Set the time limit - the maximum time that a tap can be above the threshold
     4. Set the pulse latency - the minimum required time between one pulse and the next
     5. Set the second pulse window - maximum allowed time between end of latency and start of second pulse
     for more info check out this app note: http://cache.freescale.com/files/sensors/doc/app_note/AN4072.pdf */
  writeRegister(0x21, 0x7F);  // 1. enable single/double taps on all axes
  // writeRegister(0x21, 0x55);  // 1. single taps only on all axes
  // writeRegister(0x21, 0x6A);  // 1. double taps only on all axes
  writeRegister(0x23, 0x20);  // 2. x thresh at 2g, multiply the value by 0.0625g/LSB to get the threshold
  writeRegister(0x24, 0x20);  // 2. y thresh at 2g, multiply the value by 0.0625g/LSB to get the threshold
  writeRegister(0x25, 0x08);  // 2. z thresh at .5g, multiply the value by 0.0625g/LSB to get the threshold
  writeRegister(0x26, 0x30);  // 3. 30ms time limit at 800Hz odr, this is very dependent on data rate, see the app note
  writeRegister(0x27, 0xA0);  // 4. 200ms (at 800Hz odr) between taps min, this also depends on the data rate
  writeRegister(0x28, 0xFF);  // 5. 318ms (max value) between taps max
  
  /* Set up interrupt 1 and 2 */
  writeRegister(0x2C, 0x02);  // Active high, push-pull interrupts
  writeRegister(0x2D, 0x19);  // DRDY, P/L and tap ints enabled
  writeRegister(0x2E, 0x01);  // DRDY on INT1, P/L and taps on INT2
  
  MMA8452Active();  // Set to active to start reading
}

/* Sets the MMA8452 to standby mode.
   It must be in standby to change most register settings */
void Accelerometer::MMA8452Standby()
{
  byte c = readRegister(0x2A);
  writeRegister(0x2A, c & ~(0x01));
}

/* Sets the MMA8452 to active mode.
   Needs to be in this mode to output data */
void Accelerometer::MMA8452Active()
{
  byte c = readRegister(0x2A);
  writeRegister(0x2A, c | 0x01);
}

/* Read i registers sequentially, starting at address 
   into the dest byte arra */


/* read a single byte from address and return it as a byte */
uint8_t Accelerometer::readRegister(uint8_t address)
{
  byte data;
  
  i2cSendStart();
  i2cWaitForComplete();
  
  i2cSendByte((MMA8452_ADDRESS<<1));    // write 0xB4
  i2cWaitForComplete();
  
  i2cSendByte(address); // write register address
  i2cWaitForComplete();
  
  i2cSendStart();
  
  i2cSendByte((MMA8452_ADDRESS<<1)|0x01);   // write 0xB5
  i2cWaitForComplete();
  i2cReceiveByte(TRUE);
  i2cWaitForComplete();
  
  data = i2cGetReceivedByte();  // Get MSB result
  i2cWaitForComplete();
  i2cSendStop();
  
  cbi(TWCR, TWEN);  // Disable TWI
  sbi(TWCR, TWEN);  // Enable TWI
  
  return data;
}

/* Writes a single byte (data) into address */
void Accelerometer::writeRegister(uint8_t address, uint8_t data)
{
  i2cSendStart();
  i2cWaitForComplete();
  
  i2cSendByte((MMA8452_ADDRESS<<1));// write 0xB4
  i2cWaitForComplete();
  
  i2cSendByte(address); // write register address
  i2cWaitForComplete();
  
  i2cSendByte(data);
  i2cWaitForComplete();
  
  i2cSendStop();
}

// ######### modified i2c commands #########

//! Initialize I2C (TWI) interface
void Accelerometer::i2cInit(void)
{
    // set i2c bit rate to 40KHz
    i2cSetBitrate(100);
    // enable TWI (two-wire interface)
    sbi(TWCR, TWEN);    // Enable TWI
}

//! Set the I2C transaction bitrate (in KHz)
void Accelerometer::i2cSetBitrate(uint16_t bitrateKHz)
{
    uint8_t bitrate_div;
    // set i2c bitrate
    // SCL freq = F_CPU/(16+2*TWBR))
    cbi(TWSR, TWPS0);
    cbi(TWSR, TWPS1);
    
    //calculate bitrate division    
    bitrate_div = ((F_CPU/4000l)/bitrateKHz);
    if(bitrate_div >= 16)
        bitrate_div = (bitrate_div-16)/2;
    outb(TWBR, bitrate_div);
}

// ###### Low-level I2C transaction commands ###### 

//! Send an I2C start condition in Master mode
void Accelerometer::i2cSendStart(void)
{
    WRITE_sda();
    // send start condition
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
}

//! Send an I2C stop condition in Master mode
void Accelerometer::i2cSendStop(void)
{
    // transmit stop condition
        TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

//! Wait for current I2C operation to complete
void Accelerometer::i2cWaitForComplete(void)
{
    int i = 0;      //time out variable
    
    // wait for i2c interface to complete operation
    while ((!(TWCR & (1<<TWINT))) && (i < 90))
        i++;
}

//! Send an (address|R/W) combination or a data byte over I2C
void Accelerometer::i2cSendByte(uint8_t data)
{
    delay_ms(1);
    //printf("sending 0x%x\n", data);
    WRITE_sda();
    // save data to the TWDR
    TWDR = data;
    // begin send
    TWCR = (1<<TWINT)|(1<<TWEN);
}

//! Receive a data byte over I2C  
// ackFlag = TRUE if recevied data should be ACK'ed
// ackFlag = FALSE if recevied data should be NACK'ed
void Accelerometer::i2cReceiveByte(uint8_t ackFlag)
{
    // begin receive over i2c
    if( ackFlag )
    {
        // ackFlag = TRUE: ACK the recevied data
        outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA));
    }
    else
    {
        // ackFlag = FALSE: NACK the recevied data
        outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT));
    }
}

//! Pick up the data that was received with i2cReceiveByte()
uint8_t Accelerometer::i2cGetReceivedByte(void)
{
    // retieve received data byte from i2c TWDR
    return( inb(TWDR) );
}

//! Get current I2c bus status from TWSR
uint8_t Accelerometer::i2cGetStatus(void)
{
    // retieve current i2c status from i2c TWSR
    return( inb(TWSR) );
}

void Accelerometer::delay_ms(uint16_t x)
{
  uint8_t y, z;
  for ( ; x > 0 ; x--){
    for ( y = 0 ; y < 90 ; y++){
      for ( z = 0 ; z < 6 ; z++){
        asm volatile ("nop");
      }
    }
  }
}