import math
import pygame
from miniSim import MiniSim
from miniSim import MobileRobot
miniSim = MiniSim()
robot = MobileRobot(miniSim)

def go():
	robot.home()
	steps = 72
	for _i in range(steps):
		robot.move(1)
		robot.rotate((360/steps))
miniSim.go = go
miniSim.run()
