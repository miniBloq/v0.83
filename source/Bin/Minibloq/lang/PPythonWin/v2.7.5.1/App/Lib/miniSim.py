# Copyright (C) 2014 Julian U. da Silva Gillig - http://minibloq.org
#
# miniSim is distributed under the RobotGroup-Multiplo Pacifist License (RMPL),
# either version 1.0 of the licence, or (at your option) any later version.
# You should have received a copy of the RMPL along with this software. If not,
# you can download it from http://minibloq.org.

import os
import pygame
import math

        
class MiniSim(object):
    def __init__(self):
        pygame.init()

        # Setup the screen:
        self.width = 800
        self.height = 600
        self.screen = pygame.display.set_mode((self.width, self.height))
        pygame.display.set_caption("miniSim.v0.1")

        # Create the robot:
        self.robot0 = pygame.sprite.Sprite()
        dname = ''
        if __name__ != "__main__": # Necessary to work both with IDLE and with miniBloq.
            dname = os.path.dirname(os.path.abspath(__file__)) + '/'
        self.robot0.image = pygame.image.load(dname + 'robot1.png')
        
        self.robot0.rect = self.robot0.image.get_rect()
        self.allRobots = pygame.sprite.GroupSingle(self.robot0)

        self.resetSprite(self.robot0)

        # Canvas:
        self.tileSize = self.robot0.rect.width
        self.numTilesWidth = self.width / self.tileSize
        self.numTilesHeight = self.height / self.tileSize

    def resetSprite(self, sprite):
        sprite.rect.top = self.height/2 - sprite.rect.height/2
        sprite.rect.left = self.width/2 - sprite.rect.width/2

    def update(self):
        # Background painting:
        self.screen.fill((192, 192, 192))

        # robot0 painting:
        self.allRobots.draw(self.screen)
        pygame.display.update()

        #Program end?
        self.askToQuit()

    def run(self):
        # Centers the robot again:
        self.resetSprite(self.robot0)
        
        # Robot movements:
        self.go()

        # Main application loop:
        self.finish = False
        while self.finish != True:
            self.askToQuit()

        # Program end:
        pygame.quit()

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
                return
            if event.type == pygame.KEYDOWN and event.key == pygame.K_SPACE:
                # Run the simulation (go function) again:
                self.run()
                return            
        self.finish = False


class MobileRobot(object):
    def __init__(self, simulator):
        self.speed = 1 # [ms]
        self.simulator = simulator
        self.sprite = simulator.robot0
        self.group = simulator.allRobots
        self.originalImage = simulator.robot0.image
        self.home()

    def home(self):
        self.heading = 0

    def wait(self, time_ms):
        pygame.time.wait(time_ms)

##    def move(self, distance):
##        if (distance >= 0):
##            self.forward(distance)
##        else:
##            self.reverse(-distance)

    def move(self, distance):
        #for i in range(distance):
            self.wait(self.speed)
            dx = distance*math.cos(math.radians(self.heading))
            dy = -distance*math.sin(math.radians(self.heading))
            self.sprite.rect = self.sprite.rect.move(dx, dy)
            self.simulator.update()

##    def reverse(self, distance):
##            self.wait(self.speed)
##            dx = distance*math.cos(math.radians(self.heading))
##            dy = -distance*math.sin(math.radians(self.heading))
##            self.sprite.rect = self.sprite.rect.move(dx, dy)
##            self.simulator.update()
##
##"""        for i in range(distance):
##            self.sprite.rect.top += 1
##            self.wait(self.speed);
##            self.simulator.update()"""

    def rotate(self, angle):
        if (angle >= 0):
            for i in range(angle+1):
                oldCenter = self.sprite.rect.center
                self.sprite.image = pygame.transform.rotate(self.originalImage, self.heading + i)
                self.sprite.rect = self.sprite.image.get_rect()
                self.sprite.rect.center = oldCenter
                self.simulator.update()
        else:
            for i in range(-angle+1):
                oldCenter = self.sprite.rect.center
                self.sprite.image = pygame.transform.rotate(self.originalImage, self.heading + 360 -i)
                self.sprite.rect = self.sprite.image.get_rect()
                self.sprite.rect.center = oldCenter
                self.simulator.update()
        self.heading += angle


miniSim = MiniSim()
robot = MobileRobot(miniSim)

def go():
    # User program here:
    robot.home()
    robot.wait(500)
    robot.rotate(30)
    robot.wait(300)
    robot.move(100)
    robot.wait(300)
    robot.rotate(-30)
    robot.wait(300)
    robot.move(100)
    robot.wait(300)
    robot.rotate(-45)
    robot.move(50)    

def main():
    miniSim.go = go
    miniSim.run()

if __name__ == "__main__": # Necessary to work both with IDLE and with miniBloq.
    main()
