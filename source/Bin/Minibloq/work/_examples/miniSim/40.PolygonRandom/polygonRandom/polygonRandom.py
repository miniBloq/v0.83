import math
import pygame
from miniSim import MiniSim
from miniSim import MobileRobot
miniSim = MiniSim()
robot = miniSim.robot0

def go():
	miniSim.resetRobot(robot)
	step = 0
	robot.penDown()
	robot.penColor = (0,0,255)
	for _i in range(20):
		step = robot.random()
		robot.move(step)
		robot.rotate(robot.random())
		print step
miniSim.go = go
miniSim.run()
