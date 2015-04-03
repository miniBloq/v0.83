''' Moves the robot according the captured movements (with the camera) of a blue object. '''

## This software contains code derived from the work of Abid Rahman and Jay Edry.
## The original work is available from: https://github.com/abidrahmank (specifically
## from https://github.com/abidrahmank/MyRoughWork).
## Please the license file here:
## https://github.com/abidrahmank/MyRoughWork/blob/master/license.txt

import sys
import serial
import cv2
import numpy as np


##Settings:
gridWidth = 2
gridColor = [0,255,0]

zeroZoneX = 10
zeroZoneY = 10

minBlue = 100
maxBlue = 120

minRed = 100
maxRed = 120


##Robot functions:
def sendCommand(command):
    print command
    sp.write(command + '\n')


##Color tracking functions:
def getThresholdedImg(hsv):
    #yellow = cv2.inRange(hsv,np.array((20,100,100)),np.array((30,255,255)))
    #blue = cv2.inRange(hsv,np.array((minBlue,100,100)),np.array((maxBlue,255,255)))
    blue = cv2.inRange(hsv,np.array((minBlue, 100, 100)),np.array((maxBlue,255,255)))
    #both = cv2.add(yellow,blue)
    return blue


##Main program:
##sp = serial.Serial('COM69', 115200) ##KC21 BT module
##sp = serial.Serial('COM64', 9600) ##HC-07 BT module
sp = serial.Serial('COM76', 38400) ##SparkFun's XBee module (RedBot)

##Initializes video capturing:
c = cv2.VideoCapture(0)
width,height = c.get(3),c.get(4)
print "Frame width and height: ", width, height

##Main application cycle:
while(1):
    ##Captures the image:
    _,f = c.read()

    ##Process the image:
    f = cv2.flip(f,1)
    blur = cv2.medianBlur(f,5)

    if f is not None:
        hsv = cv2.cvtColor(f,cv2.COLOR_BGR2HSV)
        both = getThresholdedImg(hsv)
        erode = cv2.erode(both,None,iterations = 3)
        dilate = cv2.dilate(erode,None,iterations = 10)
        contours,hierarchy = cv2.findContours(dilate,cv2.RETR_LIST,cv2.CHAIN_APPROX_SIMPLE)

        width, height = cv2.cv.GetSize(cv2.cv.fromarray(f))
        centerX = width/2;
        centerY = height/2;

        for cnt in contours:
            x,y,w,h = cv2.boundingRect(cnt)
            cx,cy = x+w/2, y+h/2

            ##Draw tracking rectangles:
            #if 20 < hsv.item(cy,cx,0) < 30:
            #    ##Yellow:
            #    cv2.rectangle(f,(x,y),(x+w,y+h),[0,255,255],2)
                
            if minBlue < hsv.item(cy,cx,0) < maxBlue:
                ##Blue:
                cv2.rectangle(f,(x,y),(x+w,y+h),[255,0,0],2)
                cv2.line(f,(x,cy),(x+w,cy),[0,0,0],2)
                cv2.line(f,(cx,y),(cx,y+h),[0,0,0],2)

                ##After this, cx and cy are the centers of the current tracking blue rectangle:
        
                ##Robot commands:
                powerY = (height/2 - cy)*200/height ## =(height/2 - y)*100/(height/2)
                powerX = (width/2 - cx)*200/width
                if abs(powerY) > zeroZoneY: ##The ZeroZoneY determines if the robot is stopped.
                    ##This is the splitted transform function between screen and motor power spaces:
                    if powerX  > zeroZoneX: 
                        command = 'motors ' + str(powerY) + ' ' + str(powerY+(powerX*powerY/100))
                    elif powerX < -zeroZoneX:
                        command = 'motors ' + str(powerY-(powerX*powerY/100)) + ' ' + str(powerY)
                    else:
                        command = 'motors ' + str(powerY) + ' ' + str(powerY)
                else:
                    command = 'motors'
                sendCommand(command)

        ##Zero zone x:
        zeroZoneXScreen = zeroZoneX*(width/2)/100
        cv2.line(f,(centerX-zeroZoneX,0),(centerX-zeroZoneX,height),[0,255,255], 1)
        cv2.line(f,(centerX+zeroZoneX,0),(centerX+zeroZoneX,height),[0,255,255], 1)

        ##Zero zone y:
        zeroZoneYScreen = zeroZoneY*(height/2)/100
        cv2.line(f,(0,centerY-zeroZoneYScreen),(width,centerY-zeroZoneYScreen),[0,255,255], 1)
        cv2.line(f,(0,centerY+zeroZoneYScreen),(width,centerY+zeroZoneYScreen),[0,255,255], 1)

        ##Grid:
        cv2.line(f,(0,centerY),(width,centerY),gridColor, gridWidth)
        cv2.line(f,(centerX,0),(centerX,height),gridColor, gridWidth)

        cv2.imshow('img',f)

    ##Press ESC to end the program:
    if cv2.waitKey(25) == 27:
        break

##Releases OpenCV's resources:
cv2.destroyAllWindows()
c.release()

##Releases robot's resources:
sendCommand('stop')
sp.close()
