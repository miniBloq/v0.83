import math
import pygame
from miniSim import MiniSim
from miniSim import MobileRobot
miniSim = MiniSim()
robot = miniSim.robot0

def go():
	miniSim.resetRobot(robot)
	#First, draw the perimeter:
	robot.penColor = (0,0,0)
	robot.penWidth = 20
	robot.penDown()
	for _i in range(180):
		robot.rotate(2)
		robot.move(1)
	#Move inside the perimeter (with the pen disabled):
	robot.penUp()
	robot.rotate(90)
	robot.move(60)
	robot.penWidth = 30
	#Draw a small red object:
	robot.penColor = (255,0,0)
	robot.penDown()
	robot.move(30)
	#Now, keep inside it with the color sensor:
	robot.penUp()
	while True:
		while robot.readCenterColorSensor() != (0,0,0):
			robot.move(2)
			if robot.readCenterColorSensor() == (255,0,0):
				print ":)"
		robot.move(-(10))
		#Rotates a random angle between 0 and 180 degrees:
		robot.rotate((robot.random()*1.8))
miniSim.go = go
miniSim.run()
