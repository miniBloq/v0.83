// SoftServo.h

#ifndef SOFTSERVO_H
#define SOFTSERVO_H

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

void softServoAttach(int pin);

// writes given angle to servo for given delay in milliseconds
void softServoWrite(int angle, long servoDelay);

#endif
