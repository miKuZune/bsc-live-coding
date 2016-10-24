import pygame
import random


class Ball:
    def __init__(self):
        # Choose a random position
        self.pos_x = random.randrange(0, window_width)
        self.pos_y = random.randrange(0, window_height)

        # Choose a random speed
        self.speed_x = random.randrange(-10, 10)
        self.speed_y = random.randrange(-10, 10)

        # Choose a random size
        self.radius = random.randrange(2, 20)

        # Choose a random colour
        red = random.randrange(50, 255)
        green = random.randrange(50, 255)
        blue = random.randrange(50, 255)
        self.colour = (red, green, blue)

    def update(self):
        self.pos_x += self.speed_x
        self.pos_y += self.speed_y

        # Bounce off the walls
        if self.pos_x < 0 or self.pos_x > window_width:
            self.speed_x = -self.speed_x

        if self.pos_y < 0 or self.pos_y > window_height:
            self.speed_y = -self.speed_y
    
    def draw(self):
        print "This should never be called"


class SquareBall(Ball):
    def draw(self):
        pos_x = self.pos_x
        pos_y = self.pos_y
        radius = self.radius
        rect = (pos_x - radius, pos_y - radius, 2*radius, 2*radius)
        pygame.draw.rect(screen, self.colour, rect)


class CircleBall(Ball):
    def draw(self):
        pos_x = self.pos_x
        pos_y = self.pos_y
        radius = self.radius
        pygame.draw.circle(screen, self.colour, (pos_x, pos_y), radius)


class TriangleBall(Ball):
    def draw(self):
        pos_x = self.pos_x
        pos_y = self.pos_y
        radius = self.radius
        points = [(pos_x - radius, pos_y + radius), (pos_x, pos_y - radius), (pos_x + radius, pos_y + radius)]
        pygame.draw.polygon(screen, self.colour, points)


# Initialise PyGame
pygame.init()
clock = pygame.time.Clock()

window_width = 800
window_height = 600
window_size = (window_width, window_height)

screen = pygame.display.set_mode(window_size)

# Initialise lists for ball data
balls = []

# Create balls
num_balls = 100
for ball_index in xrange(num_balls):
    shape = random.choice([SquareBall, TriangleBall, CircleBall])
    new_ball = shape()
    balls.append(new_ball)

# Main loop
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Clear the screen to black
    screen.fill((0, 0, 0))

    for ball in balls:
        ball.update()
        ball.draw()

    # Flip the display and regulate the frame rate
    pygame.display.flip()
    clock.tick(60)
