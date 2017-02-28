import pygame
import heapq
import map

def wait_key():
    while True:
        event = pygame.event.wait()
        if event.type == pygame.QUIT:
            raise KeyboardInterrupt()
        elif event.type == pygame.KEYDOWN:
            return

def a_star(the_map, screen):
    closed_set = set()
    open_set = [(0, the_map.start)]
    the_map.start.g = 0
    the_map.start.came_from = None

    while len(open_set) > 0:
        (_, node) = heapq.heappop(open_set)
        if node is the_map.goal:
            path = []
            while node is not None:
                path.insert(0, node)
                node = node.came_from
            return path

        if node in closed_set:
            continue
        closed_set.add(node)

        for edge in node.edges:
            neighbour = edge.to_node
            if neighbour not in closed_set:
                g_tentative = node.g + edge.length
                try:
                    g_current = neighbour.g
                except AttributeError:
                    g_current = 1000000

                if g_tentative < g_current:
                    neighbour.g = g_tentative
                    h = map.euclidean_distance(neighbour.pos, the_map.goal.pos)
                    heapq.heappush(open_set, (neighbour.g + h, neighbour))
                    neighbour.came_from = node

        screen.fill((255, 255, 255))

        the_map.draw(screen)

        for n in the_map.nodes.itervalues():
            try:
                came_from = n.came_from
            except AttributeError:
                continue

            if came_from is not None:
                pygame.draw.line(screen, (0, 200, 0), n.pos, came_from.pos, 4)

        node.draw(screen, (0, 255, 0), 8)

        for (_, n) in open_set:
            n.draw(screen, (0, 255, 255), 4)

        for n in closed_set:
            n.draw(screen, (255, 0, 0), 4)

        pygame.display.flip()
        wait_key()
