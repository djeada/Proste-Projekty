#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "../src/parser.h"

void test_add() {
    int error;
    assert(fabs(parse_and_eval("2+3", &error) - 5.0) < 1e-6 && error == 0);
}

void test_subtract() {
    int error;
    assert(fabs(parse_and_eval("5-2", &error) - 3.0) < 1e-6 && error == 0);
}

void test_multiply() {
    int error;
    assert(fabs(parse_and_eval("2*3", &error) - 6.0) < 1e-6 && error == 0);
}

void test_divide() {
    int error;
    assert(fabs(parse_and_eval("6/2", &error) - 3.0) < 1e-6 && error == 0);
    parse_and_eval("1/0", &error);
    assert(error == 2); // Division by zero error
}

void test_invalid_operator() {
    int error;
    // Test invalid characters/expressions
    parse_and_eval("2@3", &error);
    assert(error != 0);
}

void test_parser_basic() {
    int error;
    assert(fabs(parse_and_eval("2+3", &error) - 5.0) < 1e-6 && error == 0);
    assert(fabs(parse_and_eval("2*3+4", &error) - 10.0) < 1e-6 && error == 0);
    assert(fabs(parse_and_eval("2*(3+4)", &error) - 14.0) < 1e-6 && error == 0);
    assert(fabs(parse_and_eval("6/2", &error) - 3.0) < 1e-6 && error == 0);
    assert(fabs(parse_and_eval("6/0", &error)) < 1e-6 && error == 2);
}

void test_parser_unary_and_power() {
    int error;
    assert(fabs(parse_and_eval("-5+2", &error) - (-3.0)) < 1e-6 && error == 0);
    assert(fabs(parse_and_eval("2^3", &error) - 8.0) < 1e-6 && error == 0);
    assert(fabs(parse_and_eval("-2^2", &error) - (-4.0)) < 1e-6 && error == 0); // if implemented as -(2^2)
}

void test_parser_parentheses() {
    int error;
    assert(fabs(parse_and_eval("(2+3)*4", &error) - 20.0) < 1e-6 && error == 0);
    assert(fabs(parse_and_eval("2+(3*4)", &error) - 14.0) < 1e-6 && error == 0);
}

void test_parser_errors() {
    int error;
    parse_and_eval("2+", &error);
    assert(error != 0);
    parse_and_eval("abc", &error);
    assert(error != 0);
    parse_and_eval("2/(1-1)", &error);
    assert(error == 2);
}

int main() {
    test_add();
    test_subtract();
    test_multiply();
    test_divide();
    test_invalid_operator();
    test_parser_basic();
    test_parser_unary_and_power();
    test_parser_parentheses();
    test_parser_errors();
    printf("All tests passed!\n");
    return 0;
}
