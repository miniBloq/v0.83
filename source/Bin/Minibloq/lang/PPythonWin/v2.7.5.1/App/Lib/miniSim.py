import os
import pygame


def miniSimAskToQuit():
    # Application end events (both from keyboard or from the window close button):
    for event in pygame.event.get():
        # Application end:
        if event.type == pygame.QUIT:
            return True
        if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
            return True    
    return False


def miniSimLoop(robot, step):
    return miniSimAskToQuit()


def miniSimRun():
    pygame.init()

    # Setup the screen:
    width = 800
    height = 600
    screen = pygame.display.set_mode((width, height))

    # Create the robot:
    robot0 = pygame.sprite.Sprite()
    dname = ''
    if __name__ != "__main__": # Necessary to work both with IDLE and with miniBloq.
        dname = os.path.dirname(os.path.abspath(__file__)) + '/'
    robot0.image = pygame.image.load(dname + 'robot1.png')
    
    robot0.rect = robot0.image.get_rect()
    robot0_group = pygame.sprite.GroupSingle(robot0)

    robot0.rect.top = height/2 - robot0.rect.height/2
    robot0.rect.left = width/2 - robot0.rect.width/2

    # Canvas:
    tileSize = robot0.rect.width
    numTilesWidth = width / tileSize
    numTilesHeight = height / tileSize

    # Main application loop:
    finish = False
    while finish != True:
        # Background painting:
        screen.fill((192, 192, 192))

        # robot0 painting:
        robot0_group.draw(screen)
        pygame.display.update()

        # Movements:
        finish = miniSimLoop(robot0, tileSize)

    # Program end:
    pygame.quit()


if __name__ == "__main__": # Necessary to work both with IDLE and with miniBloq.
   miniSimRun()
