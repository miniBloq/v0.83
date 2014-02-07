#define darkLevel  200  // You'll need to adjust these for your surface.

// Speed levels for looking for line and following once it's found. Go
//  too fast and you're liable to lose the line.
#define seekSpd    150
#define fwdSpd     200

void followNoCal()
{
  static boolean centerOnLine = false;
  static boolean onLine = false;
  
  int rLevel, lLevel, cLevel; // Sensor levels.
  rLevel = rSen.read();
  lLevel = lSen.read();
  cLevel = cSen.read();
  
  // Okay, logic time! We're going to try and keep the center sensor on
  //  the line.
  
  // Start with the "no line detected" case: drive forward, slowly, until
  //  we see a line on the center sensor.
  if (cLevel < darkLevel && 
      centerOnLine == false) motor.drive(seekSpd);
  
  if (cLevel >= darkLevel) centerOnLine = true;
  
  // Okay, now we've found a line. Let's swivel to the right until our
  //  right sensor locks onto the line, then drive forward.
  if (centerOnLine &&
      onLine == false)
  {
    motor.rightBrake();
    while (rLevel < darkLevel) rLevel = rSen.read();
    motor.drive(fwdSpd);
    onLine = true;
  }
  
  // Okay, now we're "locked" to the line. We want to correct- if the
  //  center sensor is off the line but the right isn't, stop the right
  //  motor until center is back on. Vice versa for left.
  
  if (cLevel < darkLevel &&
      onLine == true)
  {
    if (rLevel >= darkLevel)
    {
      motor.rightBrake();
      while (cLevel < darkLevel)
      {
        cLevel = cSen.read();
      }
      motor.drive(fwdSpd);
    }
    else if (lLevel >= darkLevel)
    {
      motor.leftBrake();
      while (cLevel < darkLevel)
      {
        cLevel = cSen.read();
      }
      motor.drive(fwdSpd);
    }
  }
}

