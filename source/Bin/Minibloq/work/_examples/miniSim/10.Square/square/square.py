import math
import pygame
from miniSim import MiniSim
from miniSim import MobileRobot
miniSim = MiniSim()
robot = MobileRobot(miniSim)

def go():
	robot.home()
	for _i in range(4):
		robot.move(200)
		robot.rotate(90)
miniSim.go = go
miniSim.run()
