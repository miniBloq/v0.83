import math
import pygame
from miniSim import MiniSim
from miniSim import MobileRobot
miniSim = MiniSim()
robot = miniSim.robot0

def go():
	miniSim.resetRobot(robot)
	steps = 72
	for _i in range(steps):
		robot.move(1)
		robot.rotate((360/steps))
miniSim.go = go
miniSim.run()
