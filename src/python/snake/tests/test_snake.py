from snake import HEIGHT, LEFT, RIGHT, UP, DOWN, WIDTH, SnakeGame


def first_free(n):
    return 0


def last_free(n):
    return n - 1


def make_game(body, direction=RIGHT, random_below=first_free):
    game = SnakeGame(random_below)
    game.body = list(body)
    game.direction = game.next_direction = direction
    return game


def test_new_game_starts_in_the_middle():
    game = SnakeGame(first_free)
    assert game.body == [(WIDTH // 2, HEIGHT // 2)]
    assert game.direction == RIGHT
    assert game.score == 0
    assert not game.game_over


def test_food_is_placed_by_the_random_choice():
    assert SnakeGame(first_free).food == (0, 0)
    assert SnakeGame(last_free).food == (WIDTH - 1, HEIGHT - 1)


def test_food_is_never_on_the_snake():
    game = make_game([(5, 5), (4, 5), (3, 5)])
    game.place_food()
    assert game.food not in game.body


def test_step_moves_the_head():
    game = SnakeGame(first_free)
    game.step()
    assert game.body == [(11, 7)]


def test_turn_at_a_right_angle_is_accepted():
    game = SnakeGame(first_free)
    game.turn(UP)
    game.step()
    assert game.body == [(10, 6)]


def test_cannot_reverse_into_the_neck():
    game = SnakeGame(first_free)
    game.turn(LEFT)
    game.step()
    assert game.direction == RIGHT
    assert game.body == [(11, 7)]


def test_two_turns_in_one_step_cannot_reverse():
    game = SnakeGame(first_free)
    game.turn(UP)
    game.turn(LEFT)  # reverses the last step, so it is ignored
    game.step()
    assert game.body == [(10, 6)]
    assert not game.game_over


def test_hitting_the_wall_ends_the_game():
    game = make_game([(0, 5)], direction=LEFT)
    game.step()
    assert game.game_over


def test_hitting_itself_ends_the_game():
    game = make_game([(5, 5), (4, 5), (4, 6), (5, 6)], direction=DOWN)
    game.step()
    assert game.game_over


def test_eating_food_grows_the_snake_and_scores():
    game = make_game([(5, 5)])
    game.food = (6, 5)
    game.step()
    assert game.body == [(6, 5), (5, 5)]
    assert game.score == 10
    assert game.food not in game.body


def test_moving_without_food_keeps_the_length():
    game = make_game([(5, 5), (4, 5), (3, 5)])
    game.food = (0, 0)
    game.step()
    assert game.body == [(6, 5), (5, 5), (4, 5)]


def test_speed_increases_with_length():
    game = make_game([(5, 5)])
    assert game.delay_ms() == 150
    game.body = [(i, 0) for i in range(10)]
    assert game.delay_ms() == 105
    game.body = [(i, 0) for i in range(40)]
    assert game.delay_ms() == 60


def test_reset_clears_the_game():
    game = make_game([(5, 5), (4, 5)], direction=DOWN)
    game.score = 30
    game.game_over = True
    game.reset()
    assert game.body == [(WIDTH // 2, HEIGHT // 2)]
    assert game.score == 0
    assert not game.game_over
