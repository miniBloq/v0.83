import math
import pygame
from miniSim import MiniSim
from miniSim import MobileRobot
miniSim = MiniSim()
robot = MobileRobot(miniSim)

def go():
	robot.home()
	side = 1
	for _i in range(60):
		robot.rotate(8)
		robot.move(side)
		side = (side+1)
miniSim.go = go
miniSim.run()
