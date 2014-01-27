import os
import pygame

def runMiniSim():
    pygame.init()

    # Setup the screen:
    width = 800
    height = 600
    screen = pygame.display.set_mode((width, height))

    # Create the robot:
    robot0 = pygame.sprite.Sprite()
    abspath = os.path.abspath(__file__)
    dname = os.path.dirname(abspath)
    robot0.image = pygame.image.load(dname + '/robot1.png')
    robot0.rect = robot0.image.get_rect()
    robot0_group = pygame.sprite.GroupSingle(robot0)

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

        # Events:
        for event in pygame.event.get():
            # Application end:
            if event.type == pygame.QUIT:
                finish = True
            if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
                finish = True

            # Movements:
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_UP:
                    robot0.rect.top = tileSize
                elif event.key == pygame.K_DOWN:
                    robot0.rect.top += tileSize
                elif event.key == pygame.K_RIGHT:
                    robot0.rect.right += tileSize
                elif event.key == pygame.K_LEFT:
                    robot0.rect.right = tileSize

    # Program end:
    pygame.quit()


runMiniSim()
