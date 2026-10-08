/* Terminal interface for Yahtzee. Commands are typed one per line. */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "yahtzee.h"

#define LABEL_WIDTH 24
#define CELL_WIDTH 10

static int roll_face(void *context) {
    (void)context;
    return rand() % 6 + 1;
}

static bool read_line(char *line, size_t size) {
    if (fgets(line, (int)size, stdin) == NULL) {
        return false;
    }
    line[strcspn(line, "\n")] = '\0';
    return true;
}

static void print_dice(const Game *g) {
    printf("Dice:  ");
    for (int i = 0; i < NUM_DICE; i++) {
        printf("%d:", i + 1);
        if (g->rolls == 0) {
            printf(" -    ");
        } else if (g->held[i]) {
            printf("[%d]  ", g->dice[i]);
        } else {
            printf(" %d    ", g->dice[i]);
        }
    }
    printf("\n");
}

/* Filled cells show the score. The current player's empty cells show what the dice would score. */
static void print_cell(const Game *g, int player, int category) {
    char text[16];
    int score = g->cards[player].scores[category];
    if (score != UNUSED) {
        snprintf(text, sizeof text, "%d", score);
    } else if (player == g->current && g->rolls > 0) {
        snprintf(text, sizeof text, "(%d)", score_for(g->dice, (Category)category));
    } else {
        snprintf(text, sizeof text, "-");
    }
    printf("%*s", CELL_WIDTH, text);
}

static void show_screen(const Game *g, const char *message) {
    printf("\033[H\033[2J");
    if (game_is_over(g)) {
        printf("Game over!\n\n");
    } else {
        printf("Round %d of %d - Player %d to play\n\n", g->round, NUM_ROUNDS, g->current + 1);
    }
    print_dice(g);
    printf("Rolls used: %d of %d\n\n", g->rolls, MAX_ROLLS);

    printf("%-*s", LABEL_WIDTH, "");
    for (int p = 0; p < g->num_players; p++) {
        char label[32];
        snprintf(label, sizeof label, "Player %d", p + 1);
        printf("%*s", CELL_WIDTH, label);
    }
    printf("\n");

    for (int c = 0; c < NUM_CATEGORIES; c++) {
        printf("%2d. %-*s", c + 1, LABEL_WIDTH - 4, category_name((Category)c));
        for (int p = 0; p < g->num_players; p++) {
            print_cell(g, p, c);
        }
        printf("\n");
        if (c == SIXES) {
            printf("    %-*s", LABEL_WIDTH - 4, "Upper bonus (63+)");
            for (int p = 0; p < g->num_players; p++) {
                printf("%*d", CELL_WIDTH, card_upper_bonus(&g->cards[p]));
            }
            printf("\n");
        }
    }
    printf("    %-*s", LABEL_WIDTH - 4, "TOTAL");
    for (int p = 0; p < g->num_players; p++) {
        printf("%*d", CELL_WIDTH, card_total(&g->cards[p]));
    }
    printf("\n\n%s\n", message);
}

static int ask_number_of_players(void) {
    char line[64];
    while (true) {
        printf("How many players (1-4)? ");
        if (!read_line(line, sizeof line)) {
            return 0;
        }
        long n = strtol(line, NULL, 10);
        if (n >= 1 && n <= MAX_PLAYERS) {
            return (int)n;
        }
    }
}

/* Runs one command line. Returns false when the player quits. */
static bool run_command(Game *g, const char *line, char *message, size_t size) {
    snprintf(message, size, "%s", "");
    switch (line[0]) {
        case 'r':
            if (!game_roll(g, roll_face, NULL)) {
                snprintf(message, size, "%s", "No rolls left. Choose a category with s <number>.");
            }
            return true;
        case 'h': {
            const char *cursor = line + 1;
            while (true) {
                char *end;
                long die = strtol(cursor, &end, 10);
                if (end == cursor) {
                    break;
                }
                if (die < 1 || die > NUM_DICE) {
                    snprintf(message, size, "%s", "Dice are numbered 1 to 5.");
                    break;
                }
                if (!game_toggle_hold(g, (int)die - 1)) {
                    snprintf(message, size, "%s", "Roll the dice first.");
                    break;
                }
                cursor = end;
            }
            return true;
        }
        case 's': {
            long category = strtol(line + 1, NULL, 10);
            if (category < 1 || category > NUM_CATEGORIES) {
                snprintf(message, size, "%s", "Categories are numbered 1 to 13.");
            } else if (!game_choose(g, (Category)(category - 1))) {
                snprintf(message, size, "%s",
                         g->rolls == 0 ? "Roll the dice first." : "That category is already used.");
            }
            return true;
        }
        case 'q':
            return false;
        default:
            snprintf(message, size, "%s", "Unknown command. Use r, h <dice>, s <category> or q.");
            return true;
    }
}

int main(void) {
    char line[128];
    char message[128];

    srand((unsigned)time(NULL));
    int players = ask_number_of_players();
    if (players == 0) {
        return 0;
    }

    Game game;
    game_init(&game, players);
    snprintf(message, sizeof message, "%s",
             "Commands: r = roll, h 1 3 = hold dice, s 7 = score, q = quit");
    while (!game_is_over(&game)) {
        show_screen(&game, message);
        printf("> ");
        if (!read_line(line, sizeof line)) {
            break;
        }
        if (!run_command(&game, line, message, sizeof message)) {
            break;
        }
    }
    if (game_is_over(&game)) {
        show_screen(&game, "");
        int winner = game_winner(&game);
        printf("Player %d wins with %d points!\n", winner + 1, card_total(&game.cards[winner]));
    }
    return 0;
}
