def drawMaze(robot):
	robot.penWidth = 30

	robot.penUp()
	robot.rotate(-90)
	robot.move(300-robot.penWidth)
	robot.rotate(90)
	robot.penDown()
	robot.move(400-robot.penWidth)