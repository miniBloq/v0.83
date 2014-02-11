#include "RedBot.h"
#include <Arduino.h>


RedBotMotor::RedBotMotor() : 	leftSpeed(0),
								rightSpeed(0)
{
  pinMode(R_CTRL_1, OUTPUT);
  pinMode(R_CTRL_2, OUTPUT);
  pinMode(L_CTRL_1, OUTPUT);
  pinMode(L_CTRL_2, OUTPUT);
}

void RedBotMotor::stop()
{
  leftStop();
  rightStop(); 
}

void RedBotMotor::brake()
{
  leftBrake();
  rightBrake(); 
}

void RedBotMotor::drive(int speed)
{
  int power = constrain(speed, dcMotMinPower, dcMotMaxPower);
  if (power >= 0)
  {
    leftFwd((byte)(power));
    rightFwd((byte)(power));
  }
  else
  {
    leftRev((byte)(-power));
    rightRev((byte)(-power));
  }
  leftSpeed = power;
  rightSpeed = power;
}

void RedBotMotor::pivot(int speed)
{
  int power = constrain(speed, dcMotMinPower, dcMotMaxPower);
  if (power >= 0)
  {
    leftRev((byte)(power));
    rightFwd((byte)(power));
    leftSpeed = -power;
    rightSpeed = power;  
  }
  else
  {
    leftFwd((byte)(-power));
    rightRev((byte)(-power));
    leftSpeed = power;
    rightSpeed = -power;  
  }
}

void RedBotMotor::rightDrive(int speed)
{
  int power = constrain(speed, dcMotMinPower, dcMotMaxPower);
  if (power >= 0)
  {
    rightFwd((byte)(power));
  }
  else
  {
    rightRev((byte)(-power));
  }
  rightSpeed = power;
}

void RedBotMotor::leftDrive(int speed)
{
  int power = constrain(speed, dcMotMinPower, dcMotMaxPower);
  if (power >= 0)
  {
    leftFwd((byte)(power));
  }
  else
  {
    leftRev((byte)(-power));
  }
  leftSpeed = power;
}

void RedBotMotor::leftBrake()
{
  digitalWrite(L_CTRL_1, HIGH);
  digitalWrite(L_CTRL_2, HIGH);
  analogWrite(PWM_L, 0);
  leftSpeed = 0;
}

void RedBotMotor::rightBrake()
{
  digitalWrite(R_CTRL_1, HIGH);
  digitalWrite(R_CTRL_2, HIGH);
  analogWrite(PWM_R, 0);
  rightSpeed = 0;
}

void RedBotMotor::leftStop()
{
  digitalWrite(L_CTRL_1, LOW);
  digitalWrite(L_CTRL_2, LOW);
  analogWrite(PWM_L, 0);
  leftSpeed = 0;
}

void RedBotMotor::rightStop()
{
  digitalWrite(R_CTRL_1, LOW);
  digitalWrite(R_CTRL_2, LOW);
  analogWrite(PWM_R, 0);
  rightSpeed = 0;
}

/******************************************************************************
Private functions for RedBotMotor
******************************************************************************/

void RedBotMotor::leftFwd(byte spd)
{
  digitalWrite(L_CTRL_1, HIGH);
  digitalWrite(L_CTRL_2, LOW);
  analogWrite(PWM_L, spd);
}

void RedBotMotor::leftRev(byte spd)
{
  digitalWrite(L_CTRL_1, LOW);
  digitalWrite(L_CTRL_2, HIGH);
  analogWrite(PWM_L, spd);
}

void RedBotMotor::rightFwd(byte spd)
{
  digitalWrite(R_CTRL_1, HIGH);
  digitalWrite(R_CTRL_2, LOW);
  analogWrite(PWM_R, spd);
}

void RedBotMotor::rightRev(byte spd)
{
  digitalWrite(R_CTRL_1, LOW);
  digitalWrite(R_CTRL_2, HIGH);
  analogWrite(PWM_R, spd);
}
