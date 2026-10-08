"""Rules of tic-tac-toe and the minimax search for the computer player."""

EMPTY = " "
LINES = (
    (0, 1, 2), (3, 4, 5), (6, 7, 8),
    (0, 3, 6), (1, 4, 7), (2, 5, 8),
    (0, 4, 8), (2, 4, 6),
)


def new_board():
    return [EMPTY] * 9


def other_mark(mark):
    return "O" if mark == "X" else "X"


def legal_moves(board):
    return [cell for cell in range(9) if board[cell] == EMPTY]


def play(board, cell, mark):
    """Return a new board with mark on cell. Raises ValueError if the cell is taken."""
    if cell not in legal_moves(board):
        raise ValueError(f"cell {cell} is not empty")
    result = board[:]
    result[cell] = mark
    return result


def winning_line(board):
    """Return the three cells of a complete line, or None."""
    for line in LINES:
        first = board[line[0]]
        if first != EMPTY and first == board[line[1]] == board[line[2]]:
            return line
    return None


def winner(board):
    """Return "X", "O" or None."""
    line = winning_line(board)
    return board[line[0]] if line else None


def is_full(board):
    return EMPTY not in board


def is_over(board):
    return winning_line(board) is not None or is_full(board)


def is_draw(board):
    return winning_line(board) is None and is_full(board)


def _score(board, turn, me, depth):
    """Score for `me` when `turn` is to move: +10 for a win, -10 for a loss, 0 for a draw.

    The depth is subtracted from a win, so the quickest win scores highest and the slowest
    loss scores least badly.
    """
    line = winning_line(board)
    if line:
        return 10 - depth if board[line[0]] == me else depth - 10
    moves = legal_moves(board)
    if not moves:
        return 0
    scores = [_score(play(board, cell, turn), other_mark(turn), me, depth + 1) for cell in moves]
    return max(scores) if turn == me else min(scores)


def best_move(board, mark):
    """Return the cell that gives mark the best result with minimax."""
    return max(legal_moves(board), key=lambda cell: _score(play(board, cell, mark), other_mark(mark), mark, 1))
