import random


def choose_move(board):
    """ Takes a game board, and returns a move to play as a tuple (x,y)
    """

    # This player just chooses a random empty square and plays it
    while True:
        x = random.randrange(3)
        y = random.randrange(3)
        if board.get_square(x, y) == 0:
            return x, y
