/* Expression logic: tokenizer, recursive-descent parser and evaluator. */
#include "calculator.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TOK_NUMBER,
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_END
} TokenKind;

typedef struct {
    TokenKind kind;
    double number;
    size_t pos; /* index of the first character of the token in the text */
} Token;

typedef struct {
    const char *text;
    Token *tokens;
    size_t index; /* the token the parser is looking at */
    char *error;
    size_t error_size;
} Parser;

static int fail(Parser *p, const char *message)
{
    snprintf(p->error, p->error_size, "%s", message);
    return -1;
}

static int fail_unexpected(Parser *p, const Token *tok)
{
    if (tok->kind == TOK_END) {
        return fail(p, "Unexpected end of expression");
    }
    snprintf(p->error, p->error_size, "Unexpected '%c' at position %zu", p->text[tok->pos],
             tok->pos + 1);
    return -1;
}

static int is_digit(char c)
{
    return c >= '0' && c <= '9';
}

static void add_token(Parser *p, size_t *count, TokenKind kind, double number, size_t pos)
{
    p->tokens[*count].kind = kind;
    p->tokens[*count].number = number;
    p->tokens[*count].pos = pos;
    (*count)++;
}

/* Splits the text into tokens. The array must have room for strlen(text) + 1 tokens. */
static int tokenize(Parser *p)
{
    const char *s = p->text;
    size_t i = 0;
    size_t count = 0;

    while (s[i] != '\0') {
        char c = s[i];
        if (c == ' ' || c == '\t') {
            i++;
        } else if (is_digit(c) || (c == '.' && is_digit(s[i + 1]))) {
            size_t end = i;
            while (is_digit(s[end])) {
                end++;
            }
            if (s[end] == '.') {
                end++;
                while (is_digit(s[end])) {
                    end++;
                }
            }
            char lexeme[end - i + 1];
            memcpy(lexeme, s + i, end - i);
            lexeme[end - i] = '\0';
            add_token(p, &count, TOK_NUMBER, strtod(lexeme, NULL), i);
            i = end;
        } else {
            TokenKind kind;
            switch (c) {
            case '+': kind = TOK_PLUS; break;
            case '-': kind = TOK_MINUS; break;
            case '*': kind = TOK_STAR; break;
            case '/': kind = TOK_SLASH; break;
            case '(': kind = TOK_LPAREN; break;
            case ')': kind = TOK_RPAREN; break;
            default:
                snprintf(p->error, p->error_size, "Unexpected character '%c' at position %zu", c,
                         i + 1);
                return -1;
            }
            add_token(p, &count, kind, 0, i);
            i++;
        }
    }
    add_token(p, &count, TOK_END, 0, i);
    return 0;
}

static const Token *peek(const Parser *p)
{
    return &p->tokens[p->index];
}

static void advance(Parser *p)
{
    p->index++;
}

static int parse_expression(Parser *p, double *value);

/* factor := '-' factor | number | '(' expression ')' */
static int parse_factor(Parser *p, double *value)
{
    const Token *tok = peek(p);

    switch (tok->kind) {
    case TOK_NUMBER:
        *value = tok->number;
        advance(p);
        return 0;
    case TOK_MINUS:
        advance(p);
        if (parse_factor(p, value) != 0) {
            return -1;
        }
        *value = -*value;
        return 0;
    case TOK_LPAREN:
        advance(p);
        if (parse_expression(p, value) != 0) {
            return -1;
        }
        if (peek(p)->kind == TOK_END) {
            return fail(p, "Unbalanced parentheses");
        }
        if (peek(p)->kind != TOK_RPAREN) {
            return fail_unexpected(p, peek(p));
        }
        advance(p);
        return 0;
    default:
        return fail_unexpected(p, tok);
    }
}

/* term := factor (('*' | '/') factor)* */
static int parse_term(Parser *p, double *value)
{
    double right;

    if (parse_factor(p, value) != 0) {
        return -1;
    }
    while (peek(p)->kind == TOK_STAR || peek(p)->kind == TOK_SLASH) {
        TokenKind op = peek(p)->kind;
        advance(p);
        if (parse_factor(p, &right) != 0) {
            return -1;
        }
        if (op == TOK_STAR) {
            *value *= right;
        } else if (right == 0) {
            return fail(p, "Division by zero");
        } else {
            *value /= right;
        }
    }
    return 0;
}

/* expression := term (('+' | '-') term)* */
static int parse_expression(Parser *p, double *value)
{
    double right;

    if (parse_term(p, value) != 0) {
        return -1;
    }
    while (peek(p)->kind == TOK_PLUS || peek(p)->kind == TOK_MINUS) {
        TokenKind op = peek(p)->kind;
        advance(p);
        if (parse_term(p, &right) != 0) {
            return -1;
        }
        if (op == TOK_PLUS) {
            *value += right;
        } else {
            *value -= right;
        }
    }
    return 0;
}

int calc_evaluate(const char *text, double *result, char *error, size_t error_size)
{
    Token tokens[strlen(text) + 1];
    Parser p = {text, tokens, 0, error, error_size};
    double value = 0;

    if (tokenize(&p) != 0) {
        return -1;
    }
    if (peek(&p)->kind == TOK_END) {
        return fail(&p, "Empty expression");
    }
    if (parse_expression(&p, &value) != 0) {
        return -1;
    }
    if (peek(&p)->kind == TOK_RPAREN) {
        return fail(&p, "Unbalanced parentheses");
    }
    if (peek(&p)->kind != TOK_END) {
        return fail_unexpected(&p, peek(&p));
    }
    if (!isfinite(value)) {
        return fail(&p, "Result is out of range");
    }
    *result = value + 0.0; /* turns -0.0 into 0.0 so it is never printed as "-0" */
    return 0;
}
