"""Fifteen puzzle rules: moves, shuffle, solved and solvable checks."""

SIZE = 4
SHUFFLE_MOVES = 500
DIRECTIONS = {"up": (-1, 0), "down": (1, 0), "left": (0, -1), "right": (0, 1)}


def solved_board():
    return list(range(1, SIZE * SIZE)) + [0]


def gap_index(board):
    return board.index(0)


def slide(board, cell):
    """The tile in `cell` slides into the gap if they are next to each other."""
    gap = gap_index(board)
    if not 0 <= cell < len(board) or cell == gap:
        return False
    distance = abs(cell // SIZE - gap // SIZE) + abs(cell % SIZE - gap % SIZE)
    if distance != 1:
        return False
    board[gap], board[cell] = board[cell], 0
    return True


def move_gap(board, direction):
    """The gap moves in `direction`: the tile on that side slides into it."""
    drow, dcol = DIRECTIONS[direction]
    gap = gap_index(board)
    row, col = gap // SIZE + drow, gap % SIZE + dcol
    if not (0 <= row < SIZE and 0 <= col < SIZE):
        return False
    return slide(board, row * SIZE + col)


def shuffle(board, rng):
    """Random legal moves from the solved board, so the result is always solvable."""
    board[:] = solved_board()
    moves = 0
    while moves < SHUFFLE_MOVES:
        if move_gap(board, rng.choice(list(DIRECTIONS))):
            moves += 1
    if is_solved(board):
        shuffle(board, rng)


def is_solved(board):
    return board == solved_board()


def is_solvable(board):
    """On a board with an even width: solvable when inversions + the gap's row (from the top, 0-based) is odd."""
    inversions = 0
    for i in range(len(board)):
        for j in range(i + 1, len(board)):
            if board[i] and board[j] and board[i] > board[j]:
                inversions += 1
    return (inversions + gap_index(board) // SIZE) % 2 == 1
