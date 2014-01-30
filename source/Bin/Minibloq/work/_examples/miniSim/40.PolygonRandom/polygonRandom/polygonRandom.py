import math
import pygame
from miniSim import MiniSim
from miniSim import MobileRobot
miniSim = MiniSim()
robot = miniSim.robot0

def go():
	miniSim.resetRobot(robot)
	avance = 0
	robot.penDown()
	robot.penColor = (0,255,0)
	for _i in range(20):
		avance = robot.random()
		robot.move(avance)
		robot.rotate(robot.random())
		print avance
miniSim.go = go
miniSim.run()
