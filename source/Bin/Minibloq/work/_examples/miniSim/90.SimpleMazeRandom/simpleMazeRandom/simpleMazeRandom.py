import math
import pygame
from miniSim import MiniSim
from miniSim import MobileRobot
miniSim = MiniSim()
robot = miniSim.robot0

def go():
	miniSim.resetRobot(robot)
	rotation = 0
	randomNumber = 0
	import maze
	maze.drawMaze(miniSim, robot)
	while True:
		while robot.readCenterColorSensor() == (0,0,0):
			robot.move(1)
		robot.move(-(1))
		rotation = 90
		randomNumber = robot.random()
		print randomNumber
		if (randomNumber<50):
			rotation = -(rotation)
		robot.rotate(rotation)
miniSim.go = go
miniSim.run()
