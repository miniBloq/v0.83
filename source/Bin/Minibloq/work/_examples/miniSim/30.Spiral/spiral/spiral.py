import math
import pygame
from miniSim import MiniSim
from miniSim import MobileRobot
miniSim = MiniSim()
robot = miniSim.robot0

def go():
	miniSim.resetRobot(robot)
	side = 1
	robot.penWidth = 20.1
	robot.penDown()
	robot.penColor = (0,0,255)
	for _i in range(60):
		robot.rotate(8)
		robot.move(side)
		side = (side+1)
miniSim.go = go
miniSim.run()
