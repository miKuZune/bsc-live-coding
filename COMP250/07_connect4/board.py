import pygame

WIDTH = 7
HEIGHT = 6
TILE_SIZE = 100

COLOURS = [None, (200, 200, 100), (200, 100, 100)]


class Board:
    def __init__(self, copy_from = None):
        """ No arguments: initialise a new empty board
            With argument: initialise a clone of the given board """
        if copy_from is None:
            self.squares = [0] * (WIDTH * HEIGHT)
            self.current_player = 1
            self.winning_line = None
        else:
            self.squares = copy_from.squares[:]
            self.current_player = copy_from.current_player
            self.winning_line = copy_from.winning_line

    def get_square(self, x, y):
        """ Return 0 for an empty square, 1 or 2 for a player's counter """
        return self.squares[y * WIDTH + x]

    def get_available_moves(self):
        """ Return a list of available moves (column numbers) """
        moves = []
        for x in xrange(WIDTH):
            if self.get_square(x, 0) == 0:
                moves.append(x)
        return moves

    def play_move(self, x):
        """ Given a move (a column number), play that move for the current player.
            If the move is legal, flip the current player and return True.
            If not, return False. """
        for y in xrange(HEIGHT-1, -1, -1): # bottom to top, to find the first available space
            i = y * WIDTH + x
            if self.squares[i] == 0:
                self.squares[i] = self.current_player
                self.current_player = 3 - self.current_player # Flip current player
                return True
        return False
    
    def unplay_move(self, x):
        """ Remove the top counter from column x, and flip the current player. """
        for y in xrange(HEIGHT): # top to bottom, to find the first occupied space
            i = y * WIDTH + x
            if self.squares[i] != 0:
                self.squares[i] = 0
                self.current_player = 3 - self.current_player # Flip current player
                return True
        return False

    def is_board_full(self):
        """ Return True if the board is completely full, False otherwise """
        return len(self.get_available_moves()) == 0

    def check_line(self, x, y, dx, dy):
        """ Check if a player has a line starting at x,y in the direction dx,dy.
            Return 1 or 2 for which player has a line, or None if there is no line. """
        if x + 3*dx >= WIDTH or y + 3*dy >= HEIGHT:
            return None

        owner = self.get_square(x, y)
        if owner == 0:
            return None

        for i in xrange(1, 4):
            if self.get_square(x + i*dx, y + i*dy) != owner:
                return None
        return owner

    def get_game_result(self, player):
        """ If this is a terminal state, return its value for the given player:
            +1 for a win, -1 for a loss, 0 for a draw.
            If this is a nonterminal state, return None. """
        if self.is_board_full():
            return 0

        for y in xrange(HEIGHT):
            for x in xrange(WIDTH):
                for dx, dy in [(1, 0), (0, 1), (1, 1), (1, -1)]:
                    winner = self.check_line(x, y, dx, dy)
                    if winner is not None:
                        self.winning_line = (x, y, dx, dy)
                        if winner == player:
                            return 1
                        else:
                            return -1

        return None

    def draw(self, screen):
        """ Draw the board """
        for x in xrange(WIDTH):
            if x % 2 == 1:
                rect = pygame.Rect(x * TILE_SIZE, 0, TILE_SIZE, HEIGHT * TILE_SIZE)
                pygame.draw.rect(screen, (245, 245, 245), rect)

            for y in xrange(HEIGHT):
                c = self.get_square(x, y)
                colour = COLOURS[c]
                if colour is not None:
                    rect = pygame.Rect(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE)
                    pygame.draw.ellipse(screen, colour, rect)
                    pygame.draw.ellipse(screen, (0,0,0), rect, 2)
