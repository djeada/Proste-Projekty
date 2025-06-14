#include <assert.h>
#include <stdio.h>
#include "../src/calculator.h"

void test_add() {
    int error;
    assert(calculate('+', 2, 3, &error) == 5);
    assert(error == 0);
}

void test_subtract() {
    int error;
    assert(calculate('-', 5, 2, &error) == 3);
    assert(error == 0);
}

void test_multiply() {
    int error;
    assert(calculate('*', 2, 3, &error) == 6);
    assert(error == 0);
}

void test_divide() {
    int error;
    assert(calculate('/', 6, 2, &error) == 3);
    assert(error == 0);
    calculate('/', 1, 0, &error);
    assert(error == 1);
}

void test_invalid_operator() {
    int error;
    calculate('^', 1, 2, &error);
    assert(error == 2);
}

int main() {
    test_add();
    test_subtract();
    test_multiply();
    test_divide();
    test_invalid_operator();
    printf("All tests passed!\n");
    return 0;
}
