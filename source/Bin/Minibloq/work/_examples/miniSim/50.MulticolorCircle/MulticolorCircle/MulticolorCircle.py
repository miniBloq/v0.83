import math
import pygame
from miniSim import MiniSim
from miniSim import MobileRobot
miniSim = MiniSim()
robot = miniSim.robot0

def go():
	miniSim.resetRobot(robot)
	colorFactor = 2.5
	robot.penDown()
	robot.penWidth = 10
	for _i in range(180):
		robot.rotate(2)
		robot.move(1)
		robot.penColor = ((robot.random()*colorFactor),(robot.random()*colorFactor),(robot.random()*colorFactor))
miniSim.go = go
miniSim.run()
