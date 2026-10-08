#include "messenger.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Copies src to dst without the spaces at both ends. */
static void copy_trimmed(char *dst, size_t size, const char *src) {
    while (*src == ' ') {
        src++;
    }
    size_t len = strlen(src);
    while (len > 0 && src[len - 1] == ' ') {
        len--;
    }
    if (len >= size) {
        len = size - 1;
    }
    memcpy(dst, src, len);
    dst[len] = '\0';
}

static int is_word(const char *start, size_t len, const char *word) {
    return len == strlen(word) && strncmp(start, word, len) == 0;
}

LineKind parse_line(const char *line, char *arg, size_t arg_size) {
    if (line[0] != '/') {
        copy_trimmed(arg, arg_size, line);
        return arg[0] == '\0' ? LINE_EMPTY : LINE_CHAT;
    }

    const char *space = strchr(line, ' ');
    size_t name_len = space ? (size_t)(space - line) : strlen(line);
    copy_trimmed(arg, arg_size, line + name_len);

    if (is_word(line, name_len, "/nick")) {
        return LINE_NICK;
    }
    if (is_word(line, name_len, "/list")) {
        return LINE_LIST;
    }
    if (is_word(line, name_len, "/quit")) {
        return LINE_QUIT;
    }
    return LINE_UNKNOWN;
}

int valid_nick(const char *nick) {
    size_t len = strlen(nick);
    if (len == 0 || len > NICK_MAX) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)nick[i]) && nick[i] != '_' && nick[i] != '-') {
            return 0;
        }
    }
    return 1;
}

int parse_port(const char *text) {
    char *end = NULL;
    long port = strtol(text, &end, 10);
    if (end == text || *end != '\0' || port < 1 || port > 65535) {
        return -1;
    }
    return (int)port;
}

int next_line(char *buf, size_t *len, char *line) {
    size_t take = 0;
    size_t skip = 0;
    int found = 0;

    for (size_t i = 0; i < *len; i++) {
        if (buf[i] == '\n') {
            take = i;
            skip = i + 1;
            found = 1;
            break;
        }
    }
    if (!found) {
        if (*len < MAX_LINE) {
            return 0;
        }
        take = MAX_LINE;
        skip = MAX_LINE;
    }

    memcpy(line, buf, take);
    if (take > 0 && line[take - 1] == '\r') {
        take--;
    }
    line[take] = '\0';

    memmove(buf, buf + skip, *len - skip);
    *len -= skip;
    return 1;
}

void roster_init(Roster *roster) { memset(roster, 0, sizeof *roster); }

int roster_has_nick(const Roster *roster, const char *nick) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (strcmp(roster->nick[i], nick) == 0 && nick[0] != '\0') {
            return 1;
        }
    }
    return 0;
}

void roster_set(Roster *roster, int slot, const char *nick) {
    snprintf(roster->nick[slot], sizeof roster->nick[slot], "%s", nick);
}

const char *roster_get(const Roster *roster, int slot) { return roster->nick[slot]; }

void format_chat(char *out, size_t size, const char *nick, const char *text) {
    /* The whole line must fit in MAX_LINE bytes together with its newline. */
    if (size > MAX_LINE) {
        size = MAX_LINE;
    }
    snprintf(out, size, "<%s> %s", nick, text);
}

void format_join(char *out, size_t size, const char *nick) {
    snprintf(out, size, "* %s joined", nick);
}

void format_leave(char *out, size_t size, const char *nick) {
    snprintf(out, size, "* %s left", nick);
}

void format_rename(char *out, size_t size, const char *old_nick, const char *new_nick) {
    snprintf(out, size, "* %s is now known as %s", old_nick, new_nick);
}

void format_list(const Roster *roster, char *out, size_t size) {
    int count = 0;
    snprintf(out, size, "* Online:");
    for (int i = 0; i < MAX_CLIENTS; i++) {
        const char *nick = roster->nick[i];
        if (nick[0] == '\0') {
            continue;
        }
        size_t used = strlen(out);
        snprintf(out + used, size - used, "%s %s", count == 0 ? "" : ",", nick);
        count++;
    }
    if (count == 0) {
        snprintf(out, size, "* Nobody is online");
    }
}
