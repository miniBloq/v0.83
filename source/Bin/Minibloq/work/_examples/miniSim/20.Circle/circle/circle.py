import math
import pygame
from miniSim import MiniSim
from miniSim import MobileRobot
miniSim = MiniSim()
robot = MobileRobot(miniSim)

def go():
	robot.home()
	for _i in range(180):
		robot.move(1)
		robot.rotate(2)
miniSim.go = go
miniSim.run()
