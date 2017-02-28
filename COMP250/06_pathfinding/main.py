import pygame
import map

from a_star import a_star


def wait_key():
    while True:
        event = pygame.event.wait()
        if event.type == pygame.QUIT:
            raise KeyboardInterrupt()
        elif event.type == pygame.KEYDOWN:
            return


def main():
    tile_size = 50
    the_map = map.Map("level1.txt", tile_size, include_diagonals=True)

    # Initialise PyGame
    pygame.init()
    clock = pygame.time.Clock()

    window_width = the_map.width * tile_size
    window_height = the_map.height * tile_size
    window_size = (window_width, window_height)

    # Create the screen
    screen = pygame.display.set_mode(window_size)

    path = a_star(the_map, screen)

    screen.fill((255, 255, 255))

    the_map.draw(screen)
    for i in xrange(1, len(path)):
        pygame.draw.line(screen, (0, 0, 0), path[i-1].pos, path[i].pos, 6)
    
    pygame.display.flip()

    try:
        wait_key()
    except KeyboardInterrupt:
        pass

if __name__ == '__main__':
    main()
