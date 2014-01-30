# Copyright (C) 2014 Julian U. da Silva Gillig - http://minibloq.org
#
# miniSim is distributed under the RobotGroup-Multiplo Pacifist License (RMPL),
# either version 1.0 of the licence, or (at your option) any later version.
# You should have received a copy of the RMPL along with this software. If not,
# you can download it from http://minibloq.org.

import sys
import os
import pygame
import math
import random

        
class MiniSim(object):
    def __init__(self):
        os.environ['SDL_VIDEO_CENTERED'] = '1' # Centers the main window on the screen.
        pygame.init()

        # Setup the screen:
        self.width = 800
        self.height = 600
        self.screenColor = (255, 255, 146)
        self.screen = pygame.display.set_mode((self.width, self.height))
        pygame.display.set_caption("miniSim.v0.1")

        # Create the robot:
        self.robot0 = MobileRobot(self)
        self.allRobots = pygame.sprite.GroupSingle(self.robot0)
        self.resetRobot(self.robot0)

        # Canvas:
        self.tileSize = self.robot0.rect.width
        self.numTilesWidth = self.width / self.tileSize
        self.numTilesHeight = self.height / self.tileSize

    def resetRobot(self, robot):
        robot.rect.top = self.height/2 - robot.rect.height/2
        robot.rect.left = self.width/2 - robot.rect.width/2
        robot.resetHeading()
        robot.penColor = (0,0,0) #Default value.
        robot.penWidth = 2.0
        robot.penUp()
        robot.penPoints = [ ([robot.rect.centerx, robot.rect.centery], robot.isPenDown()) ]
                
    def updateLines(self):
        points = [self.robot0.penPoints[0]] # Draw all the lines, so it starts with from the very first point.
        prevPoint = self.robot0.penPoints[0][0]
        for point in self.robot0.penPoints:
            points = [prevPoint]
            if point[1]:
                points.append(point[0])
                if len(points) > 1: # Security check.
                    pygame.draw.lines(self.screen, self.robot0.penColor, False, points, int(self.robot0.penWidth))
                    if self.robot0.penWidth > 4: #Small antialiasing:
                        pygame.draw.circle(self.screen, self.robot0.penColor, point[0], int(self.robot0.penWidth/2))
            prevPoint = point[0]
        
    def update(self):
        # Background painting:
        self.screen.fill(self.screenColor)

        # Robot's pen:
        self.updateLines()
        #self.robot0.readCenterColorSensor() #debug.

        # robot0 painting:
        self.allRobots.draw(self.screen)

        pygame.display.update()

        #Program end?
        self.askToQuit()

    def run(self):
        # Centers the robot again:
        #self.resetRobot(self.robot0)
        
        # Robot movements:
        self.go()

        # Main application loop:
        self.finish = False
        while self.finish != True:
            self.askToQuit()

        # Program end:
        #pygame.quit()

    def go(self):
        #This function will be redefined by the simulator user (see main() below):
        return self.askToQuit()
        
    def askToQuit(self):
        # Application end events (both from keyboard or from the window close button):
        for event in pygame.event.get():
            # Application end:
            if event.type == pygame.QUIT:
                self.finish = True
                pygame.quit()
                return
            if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
                self.finish = True
                pygame.quit()
                sys.exit()
                return
            if event.type == pygame.KEYDOWN and event.key == pygame.K_SPACE:
                # Run the simulation (go function) again:
                self.run()
                return            
        self.finish = False


class MobileRobot(pygame.sprite.Sprite):
    def __init__(self, simulator):
        pygame.sprite.Sprite.__init__(self)

        dirName = ''
        if __name__ != "__main__": # Necessary to work both with IDLE and with miniBloq.
            dirName = os.path.dirname(os.path.abspath(__file__)) + '/'
        self.image = pygame.image.load(dirName + 'miniSim/robot0.png')
        self.rect = self.image.get_rect()

        self.__pen = False
        self.penWidth = 2.0 #Default value: Float to protect from run time errors. ##Future: create a setter and a getter.
        self.penColor = (0,0,0)
        self.penPoints = [ ([0,0], self.isPenDown()) ]
        
        self.delay = 10 # Unit: [ms]

        self.simulator = simulator
        self.originalImage = self.image

        self.heading = 0

        self.centerSenX = 30.0
        self.centerSenY = 0.0
        self.centerSenH = math.sqrt(self.centerSenX*self.centerSenX + self.centerSenY*self.centerSenY)
        self.centerSenHeading = math.degrees(math.acos(self.centerSenX/self.centerSenH))

    def resetHeading(self):
        self.rotate(-self.heading)        

    def wait(self, time_ms):
        pygame.time.wait(abs(int(time_ms)))

    def move(self, distance):
        step = 5;
        distance = int(distance)
        if (math.fabs(distance) > step):
            distanceRange = distance/step
        else:
            distanceRange = distance
        if distance <= 0:
            distanceRange = -distanceRange
            step = -step
        for i in range(distanceRange):
            self.wait(self.delay)
            dx = step*math.cos(math.radians(self.heading))
            dy = -step*math.sin(math.radians(self.heading))
            self.rect = self.rect.move(dx, dy)
            self.penPoints.append( ([self.rect.centerx, self.rect.centery], self.isPenDown()) )
            self.simulator.update()

    def rotate(self, angle):
        angle = int(angle) #Security
        if (angle >= 0):
            for i in range(angle+1):
                oldCenter = self.rect.center
                self.image = pygame.transform.rotate(self.originalImage, self.heading + i)
                self.rect = self.image.get_rect()
                self.rect.center = oldCenter
                self.simulator.update()
        else:
            for i in range(-angle+1):
                oldCenter = self.rect.center
                self.image = pygame.transform.rotate(self.originalImage, self.heading + 360 -i)
                self.rect = self.image.get_rect()
                self.rect.center = oldCenter
                self.simulator.update()
        self.heading += angle

    def penDown(self):
        self.__pen = True
        
    def penUp(self):
        self.__pen = False

    def isPenDown(self):
        return self.__pen

    def random(self):
        return int(random.uniform(0,100))

    def readCenterColorSensor(self):
        pxArray = pygame.PixelArray(self.simulator.screen)
        return pxArray[self.rect.centerx, self.rect.centery]

## Future, not working yet:
##    def readCenterColorSensor(self):
##        pxArray = pygame.PixelArray(self.simulator.screen)
##        
##        angle = self.heading + self.centerSenHeading
##        senX = self.centerSenH*math.cos(math.radians(angle)) + self.rect.centerx
##        senY = self.centerSenH*math.sin(math.radians(angle)) + self.rect.centery
##        
##        pixelColor =  pxArray[int(senX), int(senY)]
##        pygame.draw.circle(self.simulator.screen, (0,0,255), [int(senX), int(senY)], 10)
##        return pixelColor
        

miniSim = MiniSim()
robot = miniSim.robot0

def go():
    # User program here:

	miniSim.resetRobot(robot)
	#First, draw the perimeter:
	robot.penColor = (0,0,0)
	robot.penWidth = 20
	robot.penDown()
	for _i in range(180):
		robot.rotate(2)
		robot.move(1)
	#Move inside the perimeter (with the pen disabled):
	robot.penUp()
	robot.move(-10)
	robot.rotate(90)
	robot.move(50)
	#Now, keep inside it with the color sensor:
	while True:
		while robot.readCenterColorSensor() != miniSim.screen.map_rgb((0,0,0)):
			robot.move(2)
		robot.move(-(10))
		#Rotates a random angle between 0 and 180 degrees:
		robot.rotate((robot.random()*1.8))

##    # Zone keeping:
##    robot.penDown()
##    robot.penWidth = 20
##    for i in range(180):
##        robot.rotate(2)
##        robot.move(1)
##    robot.penUp()
##    robot.move(-10)
##    robot.rotate(90)
##    robot.move(20)
##    while True:
##        while robot.readCenterColorSensor() != miniSim.screen.map_rgb((0, 0, 0)):
##            robot.move(2)
##        robot.move(-10)
##        robot.rotate(robot.random()*1.8)
        #robot.rotate(30)

##    robot.wait(1000)
##    miniSim.resetRobot(robot)
##    robot.wait(1000)
##    robot.rotate(30)
##    robot.move(50)
##    robot.wait(1000)
##    robot.rotate(30)
##    robot.move(50)


##    robot.penDown()
##    robot.rotate(30)
##    robot.move(100)
##    robot.rotate(90)
##
##    robot.penUp()
##    robot.move(100)
##    robot.rotate(90)
##    robot.move(100)
##    robot.rotate(90)
##
##    robot.penDown()
##    robot.move(100)
##    
##    
##    robot.move(100)
##    robot.wait(250)
##    robot.rotate(30)
##    robot.wait(300)
##    robot.move(100)
##    robot.wait(500)
##    robot.rotate(30)
##    robot.wait(300)
##    robot.move(-50)
##    robot.wait(300)
##    robot.rotate(-30)
##    robot.wait(300)
##    robot.move(100)
##    robot.wait(300)
##    robot.rotate(-45)
##    robot.move(50)    

def main():
    miniSim.go = go
    miniSim.run()
    sys.exit()

if __name__ == "__main__": # Necessary to work both with IDLE and with miniBloq.
    main()
