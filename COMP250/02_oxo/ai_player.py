import random


def check_line(board, squares):
    empty_square = None
    other_squares = []

    for x,y in squares:
        c = board.get_square(x,y)
        if c == 0: # Empty
            if empty_square is None:
                empty_square = (x,y)
            else: # Already got an empty
                return None
        else:
            other_squares.append(c)

    # No empty squares
    if empty_square is None:
        return None

    if other_squares == [1, 1]:
        return empty_square

    if other_squares == [2, 2]:
        return empty_square

    return None


def choose_move(board):
    """ Takes a game board, and returns a move to play as a tuple (x,y)
    """

    for i in xrange(3):
        # Horizontal
        result = check_line(board, [(0,i), (1,i), (2,i)])
        if result is not None:
            return result

        # Vertical
        result = check_line(board, [(i, 0), (i, 1), (i, 2)])
        if result is not None:
            return result

    # Diagonals
    result = check_line(board, [(0,0), (1,1), (2,2)])
    if result is not None:
        return result

    result = check_line(board, [(2,0), (1,1), (0,2)])
    if result is not None:
        return result

    # Try to play in the centre
    if board.get_square(1, 1) == 0:
        return 1,1

    # Choose a random empty square and play it
    while True:
        x = random.randrange(3)
        y = random.randrange(3)
        if board.get_square(x, y) == 0:
            return x, y
