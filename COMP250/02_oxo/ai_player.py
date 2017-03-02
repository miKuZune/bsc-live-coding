import random


def minimax(board, current_player):
    winner = board.get_winner()
    if winner == 1:
        return -1
    elif winner == 2:
        return 1
    elif board.is_board_full():
        return 0
    else:
        if current_player == 1:
            best_value = 1000
        else:
            best_value = -1000

        for x in xrange(3):
            for y in xrange(3):
                if board.set_square(x, y, current_player):
                    value = minimax(board, 3 - current_player)
                    board.unset_square(x, y)
                    if current_player == 1:
                        best_value = min(value, best_value)
                        if best_value == -1:
                            return best_value
                    else:
                        best_value = max(value, best_value)
                        if best_value == 1:
                            return best_value

        return best_value


def choose_move(board):
    """ Takes a game board, and returns a move to play as a tuple (x,y)
    """

    best_move = None
    best_value = -1000

    for y in xrange(3):
        for x in xrange(3):
            if board.set_square(x, y, 2):
                value = minimax(board, 1)
                board.unset_square(x, y)
                print x, y, value
                if value > best_value:
                    best_move = (x,y)
                    best_value = value

    print
    return best_move
