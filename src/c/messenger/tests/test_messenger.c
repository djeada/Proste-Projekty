/* Tests of the chat rules: parsing, nicknames, the roster and line splitting. */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "messenger.h"

static void test_parse_chat(void) {
    char arg[MAX_LINE];
    assert(parse_line("hello   there ", arg, sizeof arg) == LINE_CHAT);
    assert(strcmp(arg, "hello   there") == 0);
    assert(parse_line("  ", arg, sizeof arg) == LINE_EMPTY);
    assert(parse_line("", arg, sizeof arg) == LINE_EMPTY);
}

static void test_parse_commands(void) {
    char arg[MAX_LINE];
    assert(parse_line("/nick  Alice ", arg, sizeof arg) == LINE_NICK);
    assert(strcmp(arg, "Alice") == 0);
    assert(parse_line("/nick", arg, sizeof arg) == LINE_NICK);
    assert(arg[0] == '\0');
    assert(parse_line("/list", arg, sizeof arg) == LINE_LIST);
    assert(parse_line("/quit", arg, sizeof arg) == LINE_QUIT);
    assert(parse_line("/nickname Bob", arg, sizeof arg) == LINE_UNKNOWN);
    assert(parse_line("/help", arg, sizeof arg) == LINE_UNKNOWN);
}

static void test_valid_nick(void) {
    assert(valid_nick("Alice"));
    assert(valid_nick("a_b-1"));
    assert(valid_nick("0123456789abcdef")); /* exactly NICK_MAX characters */
    assert(!valid_nick(""));
    assert(!valid_nick("a b"));
    assert(!valid_nick("0123456789abcdefg"));
    assert(!valid_nick("<script>"));
}

static void test_parse_port(void) {
    assert(parse_port("8888") == 8888);
    assert(parse_port("65535") == 65535);
    assert(parse_port("0") == -1);
    assert(parse_port("65536") == -1);
    assert(parse_port("80a") == -1);
    assert(parse_port("") == -1);
}

static void test_next_line(void) {
    char buf[MAX_LINE] = "one\r\ntwo\npart";
    size_t len = strlen(buf);
    char line[MAX_LINE + 1];

    assert(next_line(buf, &len, line) == 1);
    assert(strcmp(line, "one") == 0);
    assert(next_line(buf, &len, line) == 1);
    assert(strcmp(line, "two") == 0);
    assert(next_line(buf, &len, line) == 0); /* "part" is not finished yet */
    assert(len == 4);
}

static void test_next_line_too_long(void) {
    char buf[MAX_LINE];
    char line[MAX_LINE + 1];
    size_t len = MAX_LINE;
    memset(buf, 'x', MAX_LINE);

    assert(next_line(buf, &len, line) == 1);
    assert(strlen(line) == MAX_LINE);
    assert(len == 0);
}

static void test_roster(void) {
    Roster roster;
    char text[MAX_LINE];
    roster_init(&roster);
    assert(!roster_has_nick(&roster, "Alice"));

    roster_set(&roster, 2, "Alice");
    roster_set(&roster, 5, "Bob");
    assert(roster_has_nick(&roster, "Alice"));
    assert(strcmp(roster_get(&roster, 5), "Bob") == 0);

    format_list(&roster, text, sizeof text);
    assert(strcmp(text, "* Online: Alice, Bob") == 0);

    roster_set(&roster, 2, "");
    roster_set(&roster, 5, "");
    format_list(&roster, text, sizeof text);
    assert(strcmp(text, "* Nobody is online") == 0);
}

static void test_formatting(void) {
    char text[MAX_LINE];
    format_chat(text, sizeof text, "Alice", "hi");
    assert(strcmp(text, "<Alice> hi") == 0);

    format_join(text, sizeof text, "Bob");
    assert(strcmp(text, "* Bob joined") == 0);
    format_leave(text, sizeof text, "Bob");
    assert(strcmp(text, "* Bob left") == 0);
    format_rename(text, sizeof text, "Bob", "Carl");
    assert(strcmp(text, "* Bob is now known as Carl") == 0);
}

static void test_chat_is_cut_to_line_limit(void) {
    char text[MAX_LINE];
    char big[MAX_LINE];
    memset(big, 'a', MAX_LINE - 1);
    big[MAX_LINE - 1] = '\0';
    format_chat(text, sizeof text, "Alice", big);
    assert(strlen(text) == MAX_LINE - 1);
}

int main(void) {
    test_parse_chat();
    test_parse_commands();
    test_valid_nick();
    test_parse_port();
    test_next_line();
    test_next_line_too_long();
    test_roster();
    test_formatting();
    test_chat_is_cut_to_line_limit();
    printf("All messenger tests passed\n");
    return 0;
}
