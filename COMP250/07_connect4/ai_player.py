import random


def minimax(board):
    result = board.get_game_result(2)
    if result is not None:
        return result
    else:
        if board.current_player == 1:
            best_value = 1000
        else:
            best_value = -1000
        
        for move in board.get_available_moves():
            if board.play_move(move):
                value = minimax(board)
                board.unplay_move(move)
                if board.current_player == 1:
                    best_value = min(value, best_value)
                    if best_value == -1:
                        return best_value
                else:
                    best_value = max(value, best_value)
                    if best_value == 1:
                        return best_value
        
        return best_value


def choose_move(board):
    return random.choice(board.get_available_moves())
    
    """
    scored_moves = []
    
    for move in board.get_available_moves():
        board.play_move(move)
        value = minimax(board)
        board.unplay_move(move)
        scored_moves.append((value, move))
    
    print scored_moves
    
    best_score = max(score for (score, move) in scored_moves)
    best_moves = [move for (score, move) in scored_moves if score == best_score]
    
    return random.choice(best_moves)
    """
    
