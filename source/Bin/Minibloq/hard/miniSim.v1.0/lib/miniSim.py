import pygame

pygame.init()

# setup the screen:
width = 640
height = 480
screen = pygame.display.set_mode((width, height))

# create the robot:
robot0 = pygame.sprite.Sprite()
robot0.image = pygame.image.load('robot0.png')
robot0.rect = robot0.image.get_rect()


finish = False
while finish != True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            finish = True
        if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
            finish = True          

pygame.quit()
