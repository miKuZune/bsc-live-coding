import pygame

import board
import ai_player


# Initialise PyGame
pygame.init()
clock = pygame.time.Clock()

# Constants
black = (0, 0, 0)
white = (255, 255, 255)
blue = (100, 100, 255)
red = (255, 100, 100)
purple = (200, 0, 255)

window_width = 600
window_height = 600
window_size = (window_width, window_height)

grid_width = 3
grid_height = 3

square_width = window_width / grid_width
square_height = window_height / grid_height

# Create the screen
screen = pygame.display.set_mode(window_size)

# Create the game board
game_board = board.Board()

# If the game is over, game_over_text will be a pygame surface containing the game over text
# Otherwise it will be None
game_over_text = None


def check_game_over():
    """ Check if the game has ended; if so, set game_over_text.
        Return True if the game is over, otherwise False. """
    global game_over_text

    winner = game_board.get_winner()
    if winner != 0:
        font = pygame.font.Font(None, 60)
        game_over_text = font.render("Player %i wins!" % winner, True, purple, white)
        return True
    elif game_board.is_board_full():
        font = pygame.font.Font(None, 60)
        game_over_text = font.render("It's a draw!", True, purple, white)
        return True
    else:
        return False


# Main loop
running = True
while running:
    # Handle events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif game_over_text is None and event.type == pygame.MOUSEBUTTONDOWN:
            mouse_x, mouse_y = event.pos
            square_x = mouse_x / square_width
            square_y = mouse_y / square_height

            # Play the move
            if game_board.set_square(square_x, square_y, 1):
                if not check_game_over():
                    # Have the AI player play a move
                    opponent_x, opponent_y = ai_player.choose_move(game_board)
                    if not game_board.set_square(opponent_x, opponent_y, 2):
                        raise RuntimeError("AI player chose an invalid move (%i, %i)" % (opponent_x, opponent_y))
                    check_game_over()

    # Clear the screen
    screen.fill(white)

    # Draw the grid lines
    for x in xrange(1, grid_width):
        pygame.draw.line(screen, black, (x * square_width, 0), (x * square_width, window_height), 3)
    for y in xrange(1, grid_height):
        pygame.draw.line(screen, black, (0, y * square_height), (window_width, y * square_height), 3)

    # Draw the board
    for x in xrange(grid_width):
        for y in xrange(grid_height):
            square_contents = game_board.get_square(x, y)
            rect = pygame.Rect((x + 0.2) * square_width, (y + 0.2) * square_height, 0.6 * square_width, 0.6 * square_height)
            if square_contents == 1:
                pygame.draw.ellipse(screen, blue, rect, 3)
            elif square_contents == 2:
                pygame.draw.line(screen, red, rect.topleft, rect.bottomright, 3)
                pygame.draw.line(screen, red, rect.topright, rect.bottomleft, 3)

    # Display game over text
    if game_over_text is not None:
        # Text is centred on the screen
        x = (window_width - game_over_text.get_width()) * 0.5
        y = (window_height - game_over_text.get_height()) * 0.5
        screen.blit(game_over_text, (x, y))

    # Flip the display and regulate the frame rate
    pygame.display.flip()
    clock.tick(60)
