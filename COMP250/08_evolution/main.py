import pygame
import random

import map
import pathfinding


TILE_SIZE = 30
MAP_WIDTH = 20
MAP_HEIGHT = 20


def wait_key():
    while True:
        event = pygame.event.wait()
        if event.type == pygame.QUIT:
            raise KeyboardInterrupt()
        elif event.type == pygame.KEYDOWN:
            return


def main():
    tiles = [' '] * (MAP_WIDTH * MAP_HEIGHT)
    tiles[0] = 'S'
    tiles[-1] = 'G'

    the_map = map.Map((MAP_WIDTH, MAP_HEIGHT), tiles, TILE_SIZE, include_diagonals=True)

    # Initialise PyGame
    pygame.init()
    clock = pygame.time.Clock()

    window_width = the_map.width * TILE_SIZE
    window_height = the_map.height * TILE_SIZE
    window_size = (window_width, window_height)

    # Create the screen
    screen = pygame.display.set_mode(window_size)

    path = pathfinding.a_star_search(the_map, screen)
    if path is None:
        path = []

    pathfinding.pull_string(the_map, path)

    print "Path length:", pathfinding.get_path_length(path)

    screen.fill((255, 255, 255))

    the_map.draw(screen)
    for i in xrange(1, len(path)):
        pygame.draw.line(screen, (0, 0, 0), path[i - 1].pos, path[i].pos, 6)

    pygame.display.flip()

    try:
        wait_key()
    except KeyboardInterrupt:
        pass

if __name__ == '__main__':
    main()
