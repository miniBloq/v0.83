from miniSim import MiniSim

import maze

miniSim = MiniSim()
robot = miniSim.robot0


def go():
    miniSim.resetRobot(robot)
    maze.drawMaze(robot)


miniSim.go = go
miniSim.run()
