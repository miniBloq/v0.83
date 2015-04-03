# -*- coding: cp1252 -*-
''' Copyright (c) 2014 RobotGroup (http://robotgroup.com.ar)
    Wrote by Julian U. da Silva Gillig. http://minibloq.org. This software is
    distributed under the RMPL license (http://multiplo.com.ar/soft/Mbq/Minibloq.Lic.v1.0.en.pdf)'''

import sys
import serial
from time import sleep


class RG5(object):
    _prevPos = [0 for x in range(10)]
    printOutput = False
    sp = serial.Serial()

    _baseHomePosition = 10000
    _shoulderHomePosition = 10000
    _cubitHomePosition = 10000
    _tiltHomePosition = 10000
    _rollHomePosition = 700

    _gripperMinAngle = 80
    _gripperMaxAngle = 210
    _gripperCurrentAngle = 0 #Warninng: this is not zero unless the user runs a homeMech command.

    _rollCurrentAngle = 0 #Warninng: this is not zero unless the user runs a homeMech command.

    _servoWaitFactor = 0.02

    ########################################################
    # Class interface
    ########################################################

    def __init__(self):
        self.printMsg('RG5 initialized')

    def open(self, port):
        self.sp = serial.Serial(port, 115200, timeout=2)
        sleep(1)

    def close(self):
        self.sp.close()

    def printMsg(self, msg):
        if self.printOutput:
            print msg

    def go(self):
        # This function will be redefined by miniBloq, so doesn't to anything.
        self.printMsg('go')

    ########################################################
    # Auxiliary methods
    ########################################################
    
    def getLowHiBytes(self, value):
        low = int(value) & 0x00ff
        high = (int(value) & 0xff00) >> 8
        return low, high

    def angleToSteps(self, value):
        return int(value/0.033) # Float operations.        

    ########################################################
    # Movements and robot control
    ########################################################

    def areVelocitiesZero(self, printVelocities=True):
        self.sp.flushInput()
        self.sp.write(bytearray.fromhex('69')) # Command to read the state of the robot.
        ret = self.sp.read(21)
        retList = [ord(c) for c in ret]
        retList = retList[12:20]
        if printVelocities:
            self.printMsg(retList)
        return cmp(retList, [0, 0, 0, 0, 0, 0, 0, 0]) == 0

    def hasPositionChangedSingleRead(self, printPositions=False):
        self.sp.flushInput()
        self.sp.write(bytearray.fromhex('69')) # Command to read the state of the robot.
        ret = self.sp.read(21) # The command returns 21 bytes of data.
        retList = [ord(c) for c in ret] # Now we get the hex value inside the returned array.
        retList = retList[1:11] # These are the positions for all the joints (except the gripper).
        if printPositions:
            self.printMsg(retList) # Not necessary, but just to keep the user informed.
        result = cmp(retList, self._prevPos) #Compares the previous position with the current one.
        self._prevPos = retList # Assign the new previous position.
        return result

    def hasPositionChanged(self, readings=3, delay=0.1):
        result = 0
        for i in range(readings):
            result = self.hasPositionChangedSingleRead()
            if result != 0: # Only reads "readings" times the position changes if the position doesn't change even once.
                break
            sleep(delay)
            #print i # Debug.
        return result == 0

    def waitForMovementEnd(self):
        while 1:
            # This means that if the position is the same for 3 readings, the robots has stopped:
            if self.hasPositionChanged(3):
                self.printMsg('Movement finished')
                self.printMsg('')
                break

    def sendJointCmd(self, opcode, steps, precision):
        self.printMsg('Steps: ' + `steps`)
        low, high = self.getLowHiBytes(steps)
        #self.printMsg('LoHi: ' + `low` + ', ' + `high`) # Debug.

        cmd = bytearray.fromhex(opcode) #Command header.
        cmd.append(high)
        cmd.append(low)
        cmd.append(0x00)

        # Type of movement: 1 = precision / 2 = speed:
        if precision:
            cmd.append(0x01)
        else: # speed
            cmd.append(0x02) # 1 = Precision / 2 = Speed.

        self.printMsg('cmd: ' + `hex(cmd[0])` + `hex(cmd[1])` + `hex(cmd[2])` + `hex(cmd[3])` + `hex(cmd[4])`)
        self.printMsg('')

        self.sp.write(cmd)
        self.waitForMovementEnd()

    def homeMech(self):
        self.sp.write(bytearray.fromhex('6767')) # homeMech
        self.waitForMovementEnd()
        _gripperCurrentAngle = 0
        _rollCurrentAngle = 0

    def moveGripper(self, angle, wait=True):
        sleepSeconds = abs(angle - self._gripperCurrentAngle)*self._servoWaitFactor
        self._gripperCurrentAngle = angle
        
        self.printMsg('Gripper angle: ' + `int(angle)`)
        angle += self._gripperMinAngle

        low, high = self.getLowHiBytes(int(angle))
        #self.printMsg('Low: ' + `low`) # Debug.

        if (wait):
            self.printMsg('sleepSeconds: ' + `sleepSeconds`)

        cmd = bytearray.fromhex('66') #Command header.
        cmd.append(low)
        self.printMsg('cmd: ' + `hex(cmd[0])` + `hex(cmd[1])`)
        self.printMsg('')

        self.sp.write(cmd)
        if (wait):
            sleep(sleepSeconds)

    def moveBase(self, angle, precision=True):
        self.printMsg('Base angle: ' + `angle`)
        steps = self.angleToSteps(angle) + self._baseHomePosition
        self.sendJointCmd('6b', steps, precision)

    def moveShoulder(self, angle, precision=True):
        self.printMsg('Shoulder: ' + `angle`)
        steps = self.angleToSteps(angle) + self._shoulderHomePosition
        self.sendJointCmd('6d', steps, precision)

    def moveCubit(self, angle, precision=True):
        self.printMsg('Cubit angle: ' + `angle`)
        steps = self.angleToSteps(angle) + self._cubitHomePosition
        self.sendJointCmd('6e', steps, precision)

    def moveTilt(self, angle, precision=True):
        self.printMsg('Tilt angle: ' + `angle`)
        steps = self.angleToSteps(angle) + self._tiltHomePosition
        self.sendJointCmd('6c', steps, precision)

    def moveRoll(self, angle, wait=True):
        sleepSeconds = abs(angle - self._rollCurrentAngle)*self._servoWaitFactor
        _rollCurrentAngle = angle
        
        self.printMsg('Roll angle: ' + `angle`)
        steps = int(angle/0.082) + self._rollHomePosition

        self.printMsg('Steps: ' + `angle`) # Debug.
        
        self.sendJointCmd('72', steps, True)

        if (wait):
            sleep(sleepSeconds)

    ########################################################
    # Possible future methods (probably not really necessary)
    ########################################################

    '''
    def moveJQ4(self, angle):
       self.moveTilt(angle)
    '''

robot = RG5()
def go():
    # This function will be redefined by miniBloq, so doesn't to anything.
    print 'User''s go function'
    robot.printOutput = True
    robot.open('COM8')

    robot.homeMech()
    robot.moveGripper(30)
    robot.moveRoll(60)
    robot.moveBase(120)
    robot.moveShoulder(30)
    robot.moveCubit(60)
    robot.moveTilt(90)
    robot.moveRoll(120)
    robot.moveGripper(0)
    
    robot.areVelocitiesZero()

    #sleep(5)
    #robot.sp.write(bytearray.fromhex('6c36f00001'))
    
def main():
    robot.go = go
    robot.go()
    robot.close()
    sys.exit()

if __name__ == "__main__": # Necessary to work both with IDLE and with miniBloq.
    main()
