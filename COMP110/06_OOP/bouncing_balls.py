import pygame
import random

# Initialise PyGame
pygame.init()
clock = pygame.time.Clock()

window_width = 800
window_height = 600
window_size = (window_width, window_height)

screen = pygame.display.set_mode(window_size)

# Initialise lists for ball data
ball_pos_x = []
ball_pos_y = []
ball_speed_x = []
ball_speed_y = []
ball_radius = []
ball_colour = []
ball_shape = []

# Create balls
num_balls = 100
for ball_index in xrange(num_balls):
    # Choose a random position
    ball_pos_x.append(random.randrange(0, window_width))
    ball_pos_y.append(random.randrange(0, window_height))

    # Choose a random speed
    ball_speed_x.append(random.randrange(-10, 10))
    ball_speed_y.append(random.randrange(-10, 10))

    # Choose a random size
    ball_radius.append(random.randrange(2, 20))

    # Choose a random colour
    red = random.randrange(50, 255)
    green = random.randrange(50, 255)
    blue = random.randrange(50, 255)
    ball_colour.append((red, green, blue))

    # Choose a random shape
    shape = random.choice(["square", "triangle", "circle"])
    ball_shape.append(shape)

# Main loop
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Clear the screen to black
    screen.fill((0, 0, 0))

    # Loop through the balls
    for ball_index in xrange(num_balls):
        # Update position
        ball_pos_x[ball_index] += ball_speed_x[ball_index]
        ball_pos_y[ball_index] += ball_speed_y[ball_index]

        # Bounce off the walls
        if ball_pos_x[ball_index] < 0 or ball_pos_x[ball_index] > window_width:
            ball_speed_x[ball_index] = -ball_speed_x[ball_index]

        if ball_pos_y[ball_index] < 0 or ball_pos_y[ball_index] > window_height:
            ball_speed_y[ball_index] = -ball_speed_y[ball_index]

        # Draw the ball
        pos_x = ball_pos_x[ball_index]
        pos_y = ball_pos_y[ball_index]
        radius = ball_radius[ball_index]
        if ball_shape[ball_index] == "square":
            rect = (pos_x - radius, pos_y - radius, 2*radius, 2*radius)
            pygame.draw.rect(screen, ball_colour[ball_index], rect)
        elif ball_shape[ball_index] == "triangle":
            points = [(pos_x - radius, pos_y + radius), (pos_x, pos_y - radius), (pos_x + radius, pos_y + radius)]
            pygame.draw.polygon(screen, ball_colour[ball_index], points)
        elif ball_shape[ball_index] == "circle":
            pygame.draw.circle(screen, ball_colour[ball_index], (pos_x, pos_y), radius)

    # Flip the display and regulate the frame rate
    pygame.display.flip()
    clock.tick(60)
