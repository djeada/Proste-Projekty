#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// Forward declarations for internal helpers
static double parse_expr(const char **s, int *error);
static double parse_term(const char **s, int *error);
static double parse_factor(const char **s, int *error);
static double parse_power(const char **s, int *error);
static double parse_unary(const char **s, int *error);

static void skip_spaces(const char **s) {
    while (isspace(**s)) (*s)++;
}

double parse_and_eval(const char *expr, int *error) {
    *error = 0;
    const char *s = expr;
    double result = parse_expr(&s, error);
    skip_spaces(&s);
    if (*s != '\0') *error = 1;
    return result;
}

static double parse_expr(const char **s, int *error) {
    double val = parse_term(s, error);
    while (!*error) {
        skip_spaces(s);
        if (**s == '+') {
            (*s)++;
            val += parse_term(s, error);
        } else if (**s == '-') {
            (*s)++;
            val -= parse_term(s, error);
        } else {
            break;
        }
    }
    return val;
}

static double parse_unary(const char **s, int *error) {
    skip_spaces(s);
    if (**s == '-') {
        (*s)++;
        return -parse_power(s, error);
    }
    return parse_power(s, error);
}

static double parse_power(const char **s, int *error) {
    double base = parse_factor(s, error);
    while (!*error) {
        skip_spaces(s);
        if (**s == '^') {
            (*s)++;
            double exp = parse_unary(s, error); // right-associative
            base = pow(base, exp);
        } else {
            break;
        }
    }
    return base;
}

static double parse_factor(const char **s, int *error) {
    skip_spaces(s);
    if (**s == '(') {
        (*s)++;
        double val = parse_expr(s, error);
        skip_spaces(s);
        if (**s == ')') {
            (*s)++;
        } else {
            *error = 3;
        }
        return val;
    } else if (isdigit(**s) || **s == '.') {
        char *end;
        double val = strtod(*s, &end);
        *s = end;
        return val;
    } else {
        *error = 4;
        return 0;
    }
}

// Update parse_term to use parse_unary
static double parse_term(const char **s, int *error) {
    double val = parse_unary(s, error);
    while (!*error) {
        skip_spaces(s);
        if (**s == '*') {
            (*s)++;
            val *= parse_unary(s, error);
        } else if (**s == '/') {
            (*s)++;
            double denom = parse_unary(s, error);
            if (denom == 0) { *error = 2; return 0; }
            val /= denom;
        } else {
            break;
        }
    }
    return val;
}
