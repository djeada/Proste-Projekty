/* The chat protocol and the rules of the server: no sockets, no printing. */
#ifndef MESSENGER_H
#define MESSENGER_H

#include <stddef.h>

#define MAX_LINE 512 /* longest line, without the newline */
#define NICK_MAX 16
#define MAX_CLIENTS 16
#define DEFAULT_PORT 8888

typedef enum { LINE_EMPTY, LINE_CHAT, LINE_NICK, LINE_LIST, LINE_QUIT, LINE_UNKNOWN } LineKind;

/* Users by slot: nick[i] is "" when slot i is free or has no nickname yet. */
typedef struct {
    char nick[MAX_CLIENTS][NICK_MAX + 1];
} Roster;

/* Classifies a line from a user. arg gets the text after the command, or the whole line for chat.
 */
LineKind parse_line(const char *line, char *arg, size_t arg_size);

int valid_nick(const char *nick);

/* Returns the port number, or -1 if text is not a number from 1 to 65535. */
int parse_port(const char *text);

/* Moves the first line out of buf (holding *len bytes) into line. Returns 0 if no full line yet.
 * A line of MAX_LINE bytes without a newline is returned as it is. */
int next_line(char *buf, size_t *len, char *line);

void roster_init(Roster *roster);
int roster_has_nick(const Roster *roster, const char *nick);
void roster_set(Roster *roster, int slot, const char *nick);
const char *roster_get(const Roster *roster, int slot);

void format_chat(char *out, size_t size, const char *nick, const char *text);
void format_join(char *out, size_t size, const char *nick);
void format_leave(char *out, size_t size, const char *nick);
void format_rename(char *out, size_t size, const char *old_nick, const char *new_nick);
void format_list(const Roster *roster, char *out, size_t size);

#endif
