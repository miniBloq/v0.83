from miniSim import MiniSim

miniSim = MiniSim()
robot = miniSim.robot0


def go():
    miniSim.resetRobot(robot)
    robot.penWidth = 30
    robot.penDown()

    for _i in range(4):
        robot.move(200)
        robot.rotate(90)


miniSim.go = go
miniSim.run()
