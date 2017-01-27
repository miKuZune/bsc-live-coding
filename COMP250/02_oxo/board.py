class Board:
    def __init__(self):
        self.board = [0]*9

    def clone(self):
        """ Return a copy of this board """
        copy = Board()
        copy.board = self.board[:]
        return copy

    def get_square(self, x, y):
        """ Return 0, 1 or 2 depending on the contents of the specified square.
            0 = empty, 1 = noughts, 2 = crosses. """
        return self.board[y*3 + x]

    def set_square(self, x, y, mark):
        """ If the specified square is currently empty (0), fill it with mark and return True.
            If the square is not empty, leave it as-is and return False. """
        if not (0 <= x < 3 and 0 <= y < 3):
            return False

        i = y*3 + x
        if self.board[i] == 0:
            self.board[i] = mark
            return True
        else:
            return False

    def unset_square(self, x, y):
        """ Remove the mark in the specified square. """
        if 0 <= x < 3 and 0 <= y < 3:
            i = y * 3 + x
            self.board[i] = 0

    def is_board_full(self):
        """ If there are still empty squares on the board, return False.
            If there are no empty squares, return True. """
        return 0 not in self.board

    def check_line(self, start, delta):
        p = self.board[start]
        return p if p == self.board[start+delta] == self.board[start+2*delta] else 0

    def check_lines(self):
        for i in xrange(3):
            yield self.check_line(i*3, 1)   # horizontal
            yield self.check_line(i, 3)     # vertical

        # Diagonals
        yield self.check_line(0, 3+1)
        yield self.check_line(2, 3-1)

    def get_winner(self):
        """ If a player has three in a row, return 1 or 2 depending on which player.
            Otherwise, return 0. """
        try:
            return (c for c in self.check_lines() if c != 0).next()
        except StopIteration:
            return 0
