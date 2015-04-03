/*******************************
 * Softservo.ino
 * software servo control without using timers
 * note that these functions block until complete
 *******************************/
#include "SoftServo.h"

int servoPin;
int lastAngle = 200; // a value that is outside normal range

void softServoAttach(int pin)
{
  servoPin = pin;
  pinMode(pin, OUTPUT); 
}

// writes given angle to servo for given delay in milliseconds
void softServoWrite(int angle, long servoDelay)
{
  if( angle != lastAngle)
  {
    int pulsewidth = map(angle, 0, 180, 544, 2400); // width in microseconds
	Serial.print("servo pw= "); Serial.print(pulsewidth); Serial.print(", dur= "); Serial.println(servoDelay);
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(pulsewidth);
    digitalWrite(servoPin, LOW);
    delay(servoDelay); // wait 
    lastAngle = angle;
  }
}

// writes given angle to servo for given delay in milliseconds
void softServoWriteX(int angle, long servoDelay)
{
  if( angle != lastAngle)
  {
    int pulsewidth = map(angle, 0, 180, 544, 2400); // width in microseconds
    do {
      digitalWrite(servoPin, HIGH);
      delayMicroseconds(pulsewidth);
      digitalWrite(servoPin, LOW);
      delay(20); // wait for 20 milliseconds
      servoDelay -= 20; 
    }
    while(servoDelay >=0);
    lastAngle = angle;
  }
}

