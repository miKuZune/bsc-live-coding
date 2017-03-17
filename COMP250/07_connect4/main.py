import pygame

import board
import ai_player


def main():
    # Initialise PyGame
    pygame.init()
    clock = pygame.time.Clock()

    # Create the window
    window_width = board.WIDTH * board.TILE_SIZE
    window_height = board.HEIGHT * board.TILE_SIZE
    window_size = (window_width, window_height)
    screen = pygame.display.set_mode(window_size)

    # Create the game board
    the_board = board.Board()
    game_over = False

    # Main loop
    while not game_over:
        # Draw the board
        screen.fill((255, 255, 255))
        the_board.draw(screen)
        pygame.display.flip()
        
        if the_board.current_player == 1:
            # Handle events, waiting for player to play a move
            played_move = False
            while not played_move:
                event = pygame.event.wait()
                if event.type == pygame.QUIT:
                    raise KeyboardInterrupt()
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    mouse_x, mouse_y = event.pos
                    square_x = mouse_x / board.TILE_SIZE
                    if the_board.play_move(square_x):
                        played_move = True
                    else:
                        print "Illegal move"
        else:
            # Draw text in the centre of the screen
            font = pygame.font.Font(None, 60)
            text_surface = font.render("I'm thinking...", True, (0, 0, 0))
            x = (window_width - text_surface.get_width()) * 0.5
            y = (window_height - text_surface.get_height()) * 0.5
            screen.blit(text_surface, (x, y))
            pygame.display.flip()
            
            # Let the AI play a move
            move = ai_player.choose_move(the_board)
            the_board.play_move(move)
        
        # Check if game is over
        result = the_board.get_game_result(1)
        if result is not None:
            game_over = True # Causes us to break out of the main loop

            if result == 0:
                game_over_text = "It's a draw!"
            elif result == 1:
                game_over_text = "Human wins!"
            else:
                game_over_text = "Human loses!"

            # Draw the board
            screen.fill((255, 255, 255))
            the_board.draw(screen)

            # Draw the winning line
            if the_board.winning_line is not None:
                x, y, dx, dy = the_board.winning_line
                pygame.draw.line(screen, (100, 100, 200),
                                 ((x + 0.5) * board.TILE_SIZE, (y + 0.5) * board.TILE_SIZE),
                                 ((x + 0.5 + dx*3) * board.TILE_SIZE, (y + 0.5 + dy*3) * board.TILE_SIZE),
                                 5)

            # Draw text in the centre of the screen
            font = pygame.font.Font(None, 60)
            text_surface = font.render(game_over_text, True, (0, 0, 0))
            x = (window_width - text_surface.get_width()) * 0.5
            y = (window_height - text_surface.get_height()) * 0.5
            screen.blit(text_surface, (x, y))

            pygame.display.flip()

    # Game is over -- wait for user to close the window
    while True:
        event = pygame.event.wait()
        if event.type == pygame.QUIT:
            return

if __name__ == '__main__':
    main()
