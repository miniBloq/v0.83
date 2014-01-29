import math
import pygame
from miniSim import MiniSim
from miniSim import MobileRobot
miniSim = MiniSim()
robot = MobileRobot(miniSim)

def go():
	robot.home()
	robot.move(-50)
	robot.rotate(-30)
	for _i in range(360):
		robot.move(1)
		robot.rotate(1)
miniSim.go = go
miniSim.run()
