/* Tests of the shopping cart rules. Returns 0 when all tests pass. */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "shopping_cart.h"

static void test_catalog(void) {
    assert(CATALOG_SIZE == 8);
    assert(CATALOG[0].price_cents == 349);
    assert(CATALOG[4].price_cents == 2499);
}

static void test_add_and_subtotal(void) {
    Cart cart;

    cart_clear(&cart);
    assert(cart_is_empty(&cart));
    assert(cart_add(&cart, 0, 2) == CART_OK);
    assert(cart_add(&cart, 2, 1) == CART_OK);
    assert(cart_subtotal(&cart) == 2 * 349 + 875);
    assert(!cart_is_empty(&cart));
}

static void test_add_rejects_bad_input(void) {
    Cart cart;

    cart_clear(&cart);
    assert(cart_add(&cart, -1, 1) == CART_BAD_PRODUCT);
    assert(cart_add(&cart, CATALOG_SIZE, 1) == CART_BAD_PRODUCT);
    assert(cart_add(&cart, 0, 0) == CART_BAD_QUANTITY);
    assert(cart_add(&cart, 0, MAX_QUANTITY + 1) == CART_BAD_QUANTITY);
    assert(cart_add(&cart, 0, MAX_QUANTITY) == CART_OK);
    assert(cart_add(&cart, 0, 1) == CART_BAD_QUANTITY);
    assert(cart.quantities[0] == MAX_QUANTITY);
}

static void test_set_quantity_zero_removes(void) {
    Cart cart;

    cart_clear(&cart);
    cart_add(&cart, 1, 3);
    assert(cart_set_quantity(&cart, 1, 0) == CART_OK);
    assert(cart_is_empty(&cart));
    assert(cart_set_quantity(&cart, 1, 100) == CART_BAD_QUANTITY);
    assert(cart_set_quantity(&cart, 9, 1) == CART_BAD_PRODUCT);
}

static void test_percentage_discount(void) {
    Cart cart;

    cart_clear(&cart);
    cart_add(&cart, 4, 1); /* 2499 */
    cart_add(&cart, 7, 1); /* 999 */
    assert(cart_apply_code(&cart, "save10") == CART_OK);
    assert(cart_subtotal(&cart) == 3498);
    assert(cart_discount(&cart) == 350);
    assert(cart_total(&cart) == 3148);
}

static void test_percentage_rounds_to_cent(void) {
    Cart cart;

    cart_clear(&cart);
    cart_add(&cart, 0, 1); /* 349 cents: 10% is 34.9, rounds to 35 */
    cart_apply_code(&cart, "SAVE10");
    assert(cart_discount(&cart) == 35);
    assert(cart_total(&cart) == 314);
}

static void test_fixed_discount_needs_minimum(void) {
    Cart cart;

    cart_clear(&cart);
    cart_add(&cart, 0, 8); /* 2792: below the 30.00 minimum */
    assert(cart_apply_code(&cart, "FLAT5") == CART_MIN_ORDER);
    assert(cart.discount == NULL);

    cart_add(&cart, 4, 1); /* 5291: now above the minimum */
    assert(cart_apply_code(&cart, "FLAT5") == CART_OK);
    assert(cart_discount(&cart) == 500);
    assert(cart_total(&cart) == 5291 - 500);
}

static void test_discount_disappears_below_minimum(void) {
    Cart cart;

    cart_clear(&cart);
    cart_add(&cart, 4, 2); /* 4998 */
    cart_apply_code(&cart, "FLAT5");
    assert(cart_discount(&cart) == 500);
    cart_set_quantity(&cart, 4, 0);
    assert(cart_discount(&cart) == 0);
    assert(cart_total(&cart) == 0);
}

static void test_unknown_code_is_rejected(void) {
    Cart cart;

    cart_clear(&cart);
    cart_add(&cart, 4, 2);
    assert(cart_apply_code(&cart, "FREE") == CART_UNKNOWN_CODE);
    assert(cart_apply_code(&cart, "SAVE1") == CART_UNKNOWN_CODE);
    assert(cart.discount == NULL);
    assert(cart_discount(&cart) == 0);
}

static void test_clear_resets_cart_and_code(void) {
    Cart cart;

    cart_clear(&cart);
    cart_add(&cart, 4, 2);
    cart_apply_code(&cart, "FLAT5");
    cart_clear(&cart);
    assert(cart_is_empty(&cart));
    assert(cart.discount == NULL);
    assert(cart_total(&cart) == 0);
}

static void test_validate_name(void) {
    assert(validate_name("") == CART_BAD_NAME);
    assert(validate_name("A") == CART_BAD_NAME);
    assert(validate_name("1234") == CART_BAD_NAME);
    assert(validate_name("Anna") == CART_OK);
    assert(validate_name("Jan Kowalski") == CART_OK);
}

static void test_validate_address(void) {
    assert(validate_address("Main St") == CART_BAD_ADDRESS);
    assert(validate_address("12") == CART_BAD_ADDRESS);
    assert(validate_address("10 Main Street") == CART_OK);
}

int main(void) {
    test_catalog();
    test_add_and_subtotal();
    test_add_rejects_bad_input();
    test_set_quantity_zero_removes();
    test_percentage_discount();
    test_percentage_rounds_to_cent();
    test_fixed_discount_needs_minimum();
    test_discount_disappears_below_minimum();
    test_unknown_code_is_rejected();
    test_clear_resets_cart_and_code();
    test_validate_name();
    test_validate_address();
    puts("All shopping cart tests passed.");
    return 0;
}
