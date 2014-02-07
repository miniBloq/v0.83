#include "RedBot.h"
#include <Arduino.h>

RedBotSensor::RedBotSensor(int pin)
{
  _pin = pin;
}

int RedBotSensor::read()
{
  return analogRead(_pin);
}

// An attempt at a decent single-call, quick return line detection function.
//  Takes 3 samples and applies a voting algorithm to them. If two of them
//  agree that the current reading is a positive for detection (which, for us,
//  means 2 standard deviations to the background side of the detect level),
//  it returns a true. False, otherwise. This is to reduce the impact of specks
//  and crumbs on the surface.
// A better way would be to poll regularly and look for sudden changes; we
//  can't really rely on being able to do that because of the idiosyncracies of
//  the Arduino environment.
boolean RedBotSensor::check()
{
  // Are we looking for something that is darker than our normal level (say,
  //  a table edge, or a black stripe on a white surface) or something that
  //  is brighter than our normal level (say, a piece of copper tape?)
  boolean lightOnDark;
  if (_BGLevel < _detectLevel) lightOnDark = true;
  else                         lightOnDark = false;

  // We can set the number of samples we want to take (NUM_SAMPS) and the
  //  number of samples that must pass the threshold to count as a positive
  //  detection.
  #define NUM_SAMPS 3
  #define VOTE_PASS 2
  byte resultCounter = 0;
  
  int level[NUM_SAMPS];
  
  // First up, collect a few samples. Give a tiny break between them, so the
  //  sensor has time to move a bit.
  for (byte i = 0; i<NUM_SAMPS; i++)
  {
    level[i] = analogRead(_pin);
    delay(5);
  }
  
  // Now we need to consider whether we're going to call a threshold or not.
  //  We'll do that by seeing if we have more than VOTE_PASS samples that are
  //  in the detection range.
  for (byte i = 1; i<NUM_SAMPS; i++)
  {
    // For the lightOnDark case, we're looking for an increase in the voltage,
    //  corresponding to a higher current in the receiver caused by more light
    //  being reflected.
    if (lightOnDark)
    {
      // As the reflectance of the surface increases, level will rise. We'll
      //  call a successful detection _detectLevel - 2*_detectSD; a single
      //  left shift is a cheap way to do a multiply-by-two. For each detection
      //  we increase the counter; at the end we'll compare resultCounter to
      //  VOTE_PASS to make our decision.
      if (level[i]>(_detectLevel-(_detectSD<<1))) resultCounter++;
    }
    else // dark-on-light condition
    {
      if (level[i]<(_detectLevel+(_detectSD<<1))) resultCounter++;
    }
  }
  // We've now looked at all of the samples in our array; if resultCounter
  //  is greater than or equal to VOTE_PASS, we'll call it a detection.
  if (resultCounter >= VOTE_PASS) return true;
  else                            return false;
}

// setBGLevel() is used to calibrate the level that we expect to see when we
//  aren't seeing something interesting. It takes some number of samples,
//  averages them, computes the standard deviation, and stores the results
//  as _BGLevel and _BGSD.

int RedBotSensor::setBGLevel()
{
  // To calculate our average and standard deviation, we need three values:
  //  - a running mean value (stored in _BGLevel)
  //  - a temporary last mean value (stored in lastMean)
  //  - the most current read value (level)
  //  - a running value that will yield SD at the end (BGQ)
  // All those values need to be initialized to zero. I'm going to try and stick
  //  to integer math; float sucks.
  _BGLevel = 0;
  int level = 0;
  int BGQ = 0;
  int lastMean = 0;
  
  for (byte i = 1; i <= 16; i++)
  {
    // First, we need to capture a data point from each sensor.
    level = analogRead(_pin);
    // Store the current mean value, then calculate the current running mean.
    lastMean = _BGLevel;
    _BGLevel = _BGLevel + ((level-_BGLevel)/i);
    // BGQ is an intermediate variable. To find the SD at any step along the 
    //  way, we'd do this:
    //    SD = sqrt(BGQ/(i-1))
    // For more information on why this is the case, see the wikipedia article 
    //  on standard deviation. In the meantime, we *don't* need the SD, so we 
    //  can skip the computationally expensive sqrt() operation until the end.
    BGQ = BGQ + ((level-lastMean)*(level-_BGLevel));
    // The delay here is completely arbitrary. I want to be collecting data 
    //  for a goodly amount of time, to get a nice distribution. Might think
    //  about randomizing this to improve the data set.
    delay(250);
  }
  
  // Remember up above, where I described how to get SD from BGQ? Here it is:
  _BGSD = sqrt(BGQ/15);
  
  // This is a complete sham. I don't want the SD to be *too* small, so I
  //  picked a nice number for a lower bound. No surface is perfect!
  if (_BGSD<25) _BGSD = 25;
  
  return _BGLevel;
}

// setDetectLevel() works exactly the same as setBGLevel(), but with different
//  variables. I should really abstract out the lion's share of these two
//  functions, but I feel like they're small enough to not be worth the time.
int RedBotSensor::setDetectLevel()
{
  // To calculate our average and standard deviation, we need three values:
  //  - a running mean value (stored in _detectLevel)
  //  - a temporary last mean value (stored in lastMean)
  //  - the most current read value (level)
  //  - a running value that will yield SD at the end (DQ)
  // All those values need to be initialized to zero. I'm going to try and stick
  //  to integer math; float sucks.
  _detectLevel = 0;
  int level = 0;
  int DQ = 0;
  int lastMean = 0;
  
  for (byte i = 1; i <= 16; i++)
  {
    // First, we need to capture a data point from each sensor.
    level = analogRead(_pin);
    // Store the current mean value, then calculate the current running mean.
    lastMean = _detectLevel;
    _detectLevel = _detectLevel + ((level-_detectLevel)/i);
    // DQ is an intermediate variable. To find the SD at any step along the 
    //  way, we'd do this:
    //    SD = sqrt(DQ/(i-1))
    // For more information on why this is the case, see the wikipedia article 
    //  on standard deviation. In the meantime, we *don't* need the SD, so we 
    //  can skip the computationally expensive sqrt() operation until the end.
    DQ = DQ + ((level-lastMean)*(level-_detectLevel));
    // The delay here is completely arbitrary. I want to be collecting data 
    //  for a goodly amount of time, to get a nice distribution. Might think
    //  about randomizing this to improve the data set.
    delay(250);
  }
  
  // Remember up above, where I described how to get SD from DQ? Here it is:
  _detectSD = sqrt(DQ/15);
  
  // This is a complete sham. I don't want the SD to be *too* small, so I
  //  picked a nice number for a lower bound. No surface is perfect!
  if (_detectSD<25) _detectSD = 25;
  
  return _detectLevel;
  
}