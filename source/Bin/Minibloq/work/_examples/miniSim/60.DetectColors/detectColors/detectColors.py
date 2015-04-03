import math
import pygame
from miniSim import MiniSim
from miniSim import MobileRobot
miniSim = MiniSim()
robot = miniSim.robot0

def go():
	miniSim.resetRobot(robot)
	angle = 90
	side = 200
	#Draw a multicolor square:
	robot.penWidth = 30
	robot.penDown()
	robot.penColor = (255,0,0)
	robot.move(side)
	robot.rotate(angle)
	robot.penColor = (0,0,255)
	robot.move(side)
	robot.rotate(angle)
	robot.penColor = (0,0,0)
	robot.move(side)
	robot.rotate(angle)
	robot.penColor = (0,255,0)
	robot.move(side)
	robot.rotate(angle)
	#Position the robot to go to the square:
	robot.penUp()
	robot.move((side+50))
	robot.rotate(angle)
	robot.move((side/2))
	robot.rotate(angle)
	for _i in range(20):
		if robot.readCenterColorSensor() == (0,255,0):
			print 'Green color detected!'
		if robot.readCenterColorSensor() == (0,0,255):
			print 'Blue color detected!'
		robot.move(5)
miniSim.go = go
miniSim.run()
