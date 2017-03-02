import pygame
import map
import heapq

from a_star import a_star


def reconstruct_path(node):
    path = []
    while node is not None:
        path.insert(0, node)
        node = node.came_from
    return path


def depth_first_search(the_map, screen):
    stack = [the_map.start]
    the_map.start.came_from = None
    visited = set()

    while len(stack) > 0:
        current_node = stack.pop()
        visited.add(current_node)

        if current_node is the_map.goal:
            return reconstruct_path(current_node)

        for edge in current_node.edges:
            if edge.to_node not in stack and edge.to_node not in visited:
                edge.to_node.came_from = current_node
                stack.append(edge.to_node)

        screen.fill((255, 255, 255))
        the_map.draw(screen)

        for node in stack:
            node.draw(screen, (255, 0, 0), 6)

        current_node.draw(screen, (0, 255, 0), 12)

        pygame.display.flip()
        wait_key()


def breadth_first_search(the_map, screen):
    queue = [the_map.start]
    the_map.start.came_from = None
    visited = set()

    while len(queue) > 0:
        current_node = queue.pop(0)
        visited.add(current_node)

        if current_node is the_map.goal:
            return reconstruct_path(current_node)

        for edge in current_node.edges:
            if edge.to_node not in queue and edge.to_node not in visited:
                edge.to_node.came_from = current_node
                queue.append(edge.to_node)

        screen.fill((255, 255, 255))
        the_map.draw(screen)

        for node in queue:
            node.draw(screen, (255, 0, 0), 6)

        current_node.draw(screen, (0, 255, 0), 12)

        pygame.display.flip()
        wait_key()


def greedy_search(the_map, screen):
    queue = [(map.euclidean_distance(the_map.start.pos, the_map.goal.pos), the_map.start)]
    the_map.start.came_from = None
    visited = set()

    while len(queue) > 0:
        (distance, current_node) = heapq.heappop(queue)
        visited.add(current_node)

        if current_node is the_map.goal:
            return reconstruct_path(current_node)

        for edge in current_node.edges:
            if edge.to_node not in [n for (d,n) in queue] and edge.to_node not in visited:
                edge.to_node.came_from = current_node
                distance = map.euclidean_distance(edge.to_node.pos, the_map.goal.pos)
                heapq.heappush(queue, (distance, edge.to_node))

        screen.fill((255, 255, 255))
        the_map.draw(screen)

        for distance, node in queue:
            node.draw(screen, (255, 0, 0), 6)

        current_node.draw(screen, (0, 255, 0), 12)

        pygame.display.flip()
        wait_key()


def a_star_search(the_map, screen):
    queue = [(0 + map.euclidean_distance(the_map.start.pos, the_map.goal.pos), the_map.start)]
    the_map.start.g = 0
    the_map.start.came_from = None
    visited = set()

    while len(queue) > 0:
        (distance, current_node) = heapq.heappop(queue)
        visited.add(current_node)

        if current_node is the_map.goal:
            return reconstruct_path(current_node)

        for edge in current_node.edges:
            if edge.to_node not in visited:
                distance_so_far = current_node.g + edge.length

                try:
                    current_g = edge.to_node.g
                except AttributeError:
                    current_g = 1000000

                if distance_so_far < current_g:
                    edge.to_node.came_from = current_node
                    distance_to_goal = map.euclidean_distance(edge.to_node.pos, the_map.goal.pos)
                    edge.to_node.g = distance_so_far
                    heapq.heappush(queue, (distance_so_far + distance_to_goal, edge.to_node))

        screen.fill((255, 255, 255))
        the_map.draw(screen)

        for distance, node in queue:
            node.draw(screen, (255, 0, 0), 6)

        current_node.draw(screen, (0, 255, 0), 12)

        pygame.display.flip()
        #wait_key()


def wait_key():
    while True:
        event = pygame.event.wait()
        if event.type == pygame.QUIT:
            raise KeyboardInterrupt()
        elif event.type == pygame.KEYDOWN:
            return


def pull_string(the_map, path):
    i = 0
    while i+2 < len(path):
        if the_map.is_unobstructed(path[i].pos, path[i+2].pos):
            del path[i+1]
        else:
            i += 1


def main():
    tile_size = 100
    the_map = map.Map("level3.txt", tile_size, include_diagonals=True)

    # Initialise PyGame
    pygame.init()
    clock = pygame.time.Clock()

    window_width = the_map.width * tile_size
    window_height = the_map.height * tile_size
    window_size = (window_width, window_height)

    # Create the screen
    screen = pygame.display.set_mode(window_size)

    # path = a_star(the_map, screen)
    #path = breadth_first_search(the_map, screen)
    #path = depth_first_search(the_map, screen)
    #path = greedy_search(the_map, screen)
    path = a_star_search(the_map, screen)

    screen.fill((255, 255, 255))

    the_map.draw(screen)
    for i in xrange(1, len(path)):
        pygame.draw.line(screen, (0, 0, 0), path[i - 1].pos, path[i].pos, 6)

    pygame.display.flip()
    wait_key()

    pull_string(the_map, path)

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
