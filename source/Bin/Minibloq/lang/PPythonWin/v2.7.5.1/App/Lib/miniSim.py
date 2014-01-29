import os
import pygame

        
class MiniSim(object):
    def __init__(self):
        pygame.init()

        # Setup the screen:
        self.width = 800
        self.height = 600
        self.screen = pygame.display.set_mode((self.width, self.height))

        # Create the robot:
        self.robot0 = pygame.sprite.Sprite()
        dname = ''
        if __name__ != "__main__": # Necessary to work both with IDLE and with miniBloq.
            dname = os.path.dirname(os.path.abspath(__file__)) + '/'
        self.robot0.image = pygame.image.load(dname + 'robot1.png')
        
        self.robot0.rect = self.robot0.image.get_rect()
        self.robot0_group = pygame.sprite.GroupSingle(self.robot0)

        self.centerSprite(self.robot0)

        # Canvas:
        self.tileSize = self.robot0.rect.width
        self.numTilesWidth = self.width / self.tileSize
        self.numTilesHeight = self.height / self.tileSize

    def centerSprite(self, sprite):
        sprite.rect.top = self.height/2 - sprite.rect.height/2
        sprite.rect.left = self.width/2 - sprite.rect.width/2

    def update(self):
        # Background painting:
        self.screen.fill((192, 192, 192))

        # robot0 painting:
        self.robot0_group.draw(self.screen)
        pygame.display.update()

        #Program end?
        self.askToQuit()

    def run(self):
        # Centers the robot again:
        self.centerSprite(self.robot0)
        
        # Robot movements:
        self.go()

        # Main application loop:
        self.finish = False
        while self.finish != True:
            self.askToQuit()

        # Program end:
        pygame.quit()

    def go(self):
        #This function will be redefined by the simulator user (see main() below):
        return self.askToQuit()
        
    def askToQuit(self):
        # Application end events (both from keyboard or from the window close button):
        for event in pygame.event.get():
            # Application end:
            if event.type == pygame.QUIT:
                self.finish = True
                pygame.quit()
                return
            if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
                self.finish = True
                pygame.quit()
                return
            if event.type == pygame.KEYDOWN and event.key == pygame.K_SPACE:
                # Run the simulation (go function) again:
                self.run()
                return            
        self.finish = False


class MobileRobot(object):
    def __init__(self, simulator):
        self.speed = 1 #[ms]
        self.simulator = simulator

    def wait(self, time_ms):
        pygame.time.wait(time_ms)

    def move(self, distance):
        if (distance >= 0):
            self.forward(distance)
        else:
            self.reverse(-distance)

    def forward(self, distance):
        for i in range(distance):
            self.simulator.robot0.rect.top -= 1
            self.wait(self.speed);
            self.simulator.update()

    def reverse(self, distance):
        for i in range(distance):
            self.simulator.robot0.rect.top += 1
            self.wait(self.speed);
            self.simulator.update()


miniSim = MiniSim()
robot = MobileRobot(miniSim)

def go():
    robot.forward(50)
    robot.wait(300)
    robot.reverse(150)

def main():
    # Main user program here:
    miniSim.go = go
    miniSim.run()

if __name__ == "__main__": # Necessary to work both with IDLE and with miniBloq.
    main()
