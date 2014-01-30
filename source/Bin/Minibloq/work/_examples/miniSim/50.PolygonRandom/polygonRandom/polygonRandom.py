import math
import pygame
from miniSim import MiniSim
from miniSim import MobileRobot
miniSim = MiniSim()
robot = miniSim.robot0

def go():
	miniSim.resetRobot(robot)
	for _i in range(20):
		robot.move(robot.random())
		robot.rotate(robot.random())
miniSim.go = go
miniSim.run()
