import pygame
import random

import map
import pathfinding


TILE_SIZE = 25
MAP_WIDTH = 20
MAP_HEIGHT = 20


def wait_key():
    while True:
        event = pygame.event.wait()
        if event.type == pygame.QUIT:
            raise KeyboardInterrupt()
        elif event.type == pygame.KEYDOWN:
            return


def draw_map_and_path(screen, the_map, path):
    screen.fill((255, 255, 255))

    the_map.draw(screen)
    for i in xrange(1, len(path)):
        pygame.draw.line(screen, (0, 0, 0), path[i - 1].pos, path[i].pos, 6)

    pygame.display.flip()


def get_path(the_map):
    path = pathfinding.a_star_search(the_map, None)
    if path is None:
        path = []

    pathfinding.pull_string(the_map, path)

    return path


def calculate_fitness(the_map):
    path = get_path(the_map)
    return pathfinding.get_path_length(path)


def main():
    tiles = [' '] * (MAP_WIDTH * MAP_HEIGHT)
    tiles[0] = 'S'
    tiles[-1] = 'G'

    the_map = map.Map((MAP_WIDTH, MAP_HEIGHT), tiles, TILE_SIZE, include_diagonals=True)
    current_fitness = calculate_fitness(the_map)

    # Initialise PyGame
    pygame.init()
    clock = pygame.time.Clock()

    window_width = the_map.width * TILE_SIZE
    window_height = the_map.height * TILE_SIZE
    window_size = (window_width, window_height)

    # Create the screen
    screen = pygame.display.set_mode(window_size)

    index = 0

    while True:
        index += 1
        if index >= len(tiles):
            index = 0

        the_map = map.Map((MAP_WIDTH, MAP_HEIGHT), tiles, TILE_SIZE, include_diagonals=True)
        print "Current fitness:", current_fitness
        path = get_path(the_map)
        draw_map_and_path(screen, the_map, path)

        new_tiles = tiles[:]
        #index = random.randrange(len(new_tiles))
        if new_tiles[index] == ' ':
            new_tiles[index] = '*'

            new_map = map.Map((MAP_WIDTH, MAP_HEIGHT), new_tiles, TILE_SIZE, include_diagonals=True)
            new_fitness = calculate_fitness(new_map)

            if new_fitness > current_fitness:
                tiles = new_tiles
                current_fitness = new_fitness

        # wait_key()

if __name__ == '__main__':
    main()
