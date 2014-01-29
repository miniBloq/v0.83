import math
import pygame
from miniSim import MiniSim
from miniSim import MobileRobot
miniSim = MiniSim()
robot = MobileRobot(miniSim)

def go():
	robot.home()
	side=100
	robot.move(side)
	robot.rotate(90)
	robot.move(side)
	robot.rotate(90)
	robot.move(side)
	robot.rotate(90)
	robot.move(side)
	robot.rotate(90)
	robot.move(side)
miniSim.go = go
miniSim.run()
