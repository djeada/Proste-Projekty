/* Tests of the expression logic. Returns 0 when all tests pass. */
#include "calculator.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static void check_value(const char *text, double expected)
{
    char error[128];
    double result = 0;

    assert(calc_evaluate(text, &result, error, sizeof error) == 0);
    assert(fabs(result - expected) < 1e-9);
}

static void check_error(const char *text, const char *message)
{
    char error[128];
    double result = 0;

    assert(calc_evaluate(text, &result, error, sizeof error) == -1);
    assert(strcmp(error, message) == 0);
}

static void test_precedence(void)
{
    check_value("2 + 3 * 4", 14);
    check_value("2 + 3 * (4 - 1)", 11);
    check_value("10 - 4 - 3", 3);
    check_value("8 / 4 / 2", 1);
    check_value("2 * 3 + 4 / 2", 8);
}

static void test_parentheses(void)
{
    check_value("(1 + 2) * (3 + 4)", 21);
    check_value("((2))", 2);
    check_value("-(2 + 3)", -5);
}

static void test_unary_minus(void)
{
    check_value("-5 + 2", -3);
    check_value("2 * -3", -6);
    check_value("--4", 4);
    check_value("2 - -3", 5);
}

static void test_decimals(void)
{
    check_value("0.1 + 0.2", 0.3);
    check_value(".5 * 4", 2);
    check_value("3. + 1", 4);
    check_value("7.5 / 2.5", 3);
}

static void test_whitespace(void)
{
    check_value("  1+2\t", 3);
}

static void test_errors(void)
{
    char huge[400];

    check_error("", "Empty expression");
    check_error("   ", "Empty expression");
    check_error("1 / 0", "Division by zero");
    check_error("1 / (2 - 2)", "Division by zero");
    check_error("(1 + 2", "Unbalanced parentheses");
    check_error("(1 + (2)", "Unbalanced parentheses");
    check_error("1 + 2)", "Unbalanced parentheses");
    check_error("2 $ 3", "Unexpected character '$' at position 3");
    check_error("1e5", "Unexpected character 'e' at position 2");
    check_error("2 +", "Unexpected end of expression");
    check_error("2 3", "Unexpected '3' at position 3");
    check_error("* 2", "Unexpected '*' at position 1");
    check_error("()", "Unexpected ')' at position 2");
    check_error("1.2.3", "Unexpected '.' at position 4");

    memset(huge, '9', sizeof huge - 1);
    huge[sizeof huge - 1] = '\0';
    check_error(huge, "Result is out of range");
}

int main(void)
{
    test_precedence();
    test_parentheses();
    test_unary_minus();
    test_decimals();
    test_whitespace();
    test_errors();
    printf("All calculator tests passed.\n");
    return 0;
}
