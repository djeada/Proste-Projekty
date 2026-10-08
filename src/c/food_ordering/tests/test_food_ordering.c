/* Tests of the food ordering rules. Returns 0 when every test passes. */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "food_ordering.h"

static void test_menu(void) {
    assert(MENU_SIZE == 10);
    assert(strcmp(dish_find(3)->name, "Margherita Pizza") == 0);
    assert(dish_find(3)->price_cents == 1200);
    assert(dish_find(0) == NULL);
    assert(dish_find(11) == NULL);
    assert(strcmp(payment_name(PAYMENT_CASH), "Cash on delivery") == 0);
}

static void test_add_set_and_remove(void) {
    Order order;
    order_init(&order);
    assert(order_add_dish(&order, 1, 2));
    assert(order_add_dish(&order, 1, 1));
    assert(order_quantity(&order, 1) == 3);
    assert(order_set_quantity(&order, 1, 1));
    assert(order_quantity(&order, 1) == 1);
    assert(order_set_quantity(&order, 1, 0));
    assert(order.line_count == 0);
    assert(order_quantity(&order, 1) == 0);
}

static void test_limits_and_unknown_dishes(void) {
    Order order;
    order_init(&order);
    assert(!order_add_dish(&order, 99, 1));
    assert(!order_add_dish(&order, 1, 0));
    assert(!order_set_quantity(&order, 1, MAX_QUANTITY + 1));
    assert(!order_set_quantity(&order, 1, -1));
    assert(order_set_quantity(&order, 1, MAX_QUANTITY));
    assert(!order_add_dish(&order, 1, 1));
    assert(order_quantity(&order, 1) == MAX_QUANTITY);
}

static void test_total_in_cents(void) {
    Order order;
    order_init(&order);
    assert(order_total_cents(&order) == 0);
    assert(order_add_dish(&order, 9, 2));    /* 2 x 2.90 */
    assert(order_add_dish(&order, 6, 1));    /* 1 x 18.90 */
    assert(order_total_cents(&order) == 2 * 290 + 1890);
}

static void test_address_validation(void) {
    Order order;
    order_init(&order);
    assert(order_set_address(&order, "  Main Street 12  "));
    assert(strcmp(order.address, "Main Street 12") == 0);
    assert(!order_set_address(&order, "Main St"));     /* no house number */
    assert(!order_set_address(&order, "12345"));       /* no street name */
    assert(!order_set_address(&order, "1 A"));         /* too short */
}

static void test_phone_validation(void) {
    Order order;
    order_init(&order);
    assert(order_set_phone(&order, "+48 (123) 456-789"));
    assert(strcmp(order.phone, "+48123456789") == 0);
    assert(!order_set_phone(&order, "12345"));            /* too short */
    assert(!order_set_phone(&order, "1234567890123456")); /* 16 digits */
    assert(!order_set_phone(&order, "123abc4567"));
}

static void test_luhn(void) {
    assert(luhn_is_valid("4111 1111 1111 1111"));
    assert(luhn_is_valid("4539-1488-0343-6467"));
    assert(luhn_is_valid("378282246310005"));
    assert(!luhn_is_valid("4111 1111 1111 1112"));
    assert(!luhn_is_valid("1234"));
    assert(!luhn_is_valid("4111 1111 1111 abcd"));
}

static void test_card_payment_keeps_last_four(void) {
    Order order;
    order_init(&order);
    assert(!order_set_card(&order, "4111 1111 1111 1112"));
    assert(order.payment == PAYMENT_NONE);
    assert(order_set_card(&order, "4111 1111 1111 1111"));
    assert(order.payment == PAYMENT_CARD);
    assert(strcmp(order.card_last4, "1111") == 0);
    assert(order_set_payment(&order, PAYMENT_PAYPAL));
    assert(order.card_last4[0] == '\0');
    assert(!order_set_payment(&order, PAYMENT_CARD));
}

static void test_place_order_rules(void) {
    Order order;
    order_init(&order);
    assert(order_place(&order, 100) != NULL);            /* empty order */
    assert(order_add_dish(&order, 3, 1));
    assert(order_place(&order, 100) != NULL);            /* no address */
    assert(order_set_address(&order, "Main Street 12"));
    assert(order_place(&order, 100) != NULL);            /* no phone */
    assert(order_set_phone(&order, "123456789"));
    assert(order_place(&order, 100) != NULL);            /* no payment */
    assert(order_set_payment(&order, PAYMENT_CASH));
    assert(order_place(&order, 100) == NULL);
    assert(order.placed && order.placed_at == 100);
    assert(order_place(&order, 200) != NULL);            /* placed twice */
    assert(!order_add_dish(&order, 1, 1));                /* no changes after placing */
    assert(!order_set_address(&order, "Other Road 5"));
}

static void test_status_follows_elapsed_time(void) {
    assert(order_status_at(0) == STATUS_RECEIVED);
    assert(order_status_at(STATUS_STEP_SECONDS - 1) == STATUS_RECEIVED);
    assert(order_status_at(STATUS_STEP_SECONDS) == STATUS_PREPARING);
    assert(order_status_at(2 * STATUS_STEP_SECONDS) == STATUS_ON_THE_WAY);
    assert(order_status_at(3 * STATUS_STEP_SECONDS - 1) == STATUS_ON_THE_WAY);
    assert(order_status_at(3 * STATUS_STEP_SECONDS) == STATUS_DELIVERED);
    assert(order_status_at(1000) == STATUS_DELIVERED);
}

int main(void) {
    test_menu();
    test_add_set_and_remove();
    test_limits_and_unknown_dishes();
    test_total_in_cents();
    test_address_validation();
    test_phone_validation();
    test_luhn();
    test_card_payment_keeps_last_four();
    test_place_order_rules();
    test_status_follows_elapsed_time();
    printf("All food ordering tests passed.\n");
    return 0;
}
