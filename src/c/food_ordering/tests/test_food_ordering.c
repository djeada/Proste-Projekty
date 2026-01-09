#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../src/food_ordering.h"

void test_menu_init(void) {
    Menu menu;
    menu_init(&menu);
    assert(menu.item_count == 0);
}

void test_menu_add_item(void) {
    Menu menu;
    menu_init(&menu);

    int id = menu_add_item(&menu, "Pizza", "Delicious pizza", 12.99, CATEGORY_MAIN_COURSE);
    assert(id > 0);
    assert(menu.item_count == 1);

    MenuItem *item = menu_find_item(&menu, id);
    assert(item != NULL);
    assert(strcmp(item->name, "Pizza") == 0);
    assert(fabs(item->price - 12.99) < 0.001);
    assert(item->category == CATEGORY_MAIN_COURSE);
    assert(item->available == 1);
}

void test_menu_find_item(void) {
    Menu menu;
    menu_init(&menu);

    int id1 = menu_add_item(&menu, "Burger", "Beef burger", 9.99, CATEGORY_MAIN_COURSE);
    int id2 = menu_add_item(&menu, "Salad", "Fresh salad", 7.99, CATEGORY_APPETIZER);

    assert(menu_find_item(&menu, id1) != NULL);
    assert(menu_find_item(&menu, id2) != NULL);
    assert(menu_find_item(&menu, 9999) == NULL);
}

void test_order_system_init(void) {
    OrderSystem system;
    order_system_init(&system);
    assert(system.order_count == 0);
    assert(system.next_order_id == 1);
}

void test_order_create(void) {
    OrderSystem system;
    order_system_init(&system);

    int order_id = order_create(&system);
    assert(order_id == 1);
    assert(system.order_count == 1);

    Order *order = order_find(&system, order_id);
    assert(order != NULL);
    assert(order->status == STATUS_PENDING);
    assert(order->item_count == 0);
}

void test_order_add_item(void) {
    Menu menu;
    OrderSystem system;
    menu_init(&menu);
    order_system_init(&system);

    int item_id = menu_add_item(&menu, "Pasta", "Italian pasta", 11.99, CATEGORY_MAIN_COURSE);
    int order_id = order_create(&system);

    assert(order_add_item(&system, order_id, item_id, 2) == 1);
    
    Order *order = order_find(&system, order_id);
    assert(order->item_count == 1);
    assert(order->items[0].menu_item_id == item_id);
    assert(order->items[0].quantity == 2);
}

void test_order_calculate_total(void) {
    Menu menu;
    OrderSystem system;
    menu_init(&menu);
    order_system_init(&system);

    int item1 = menu_add_item(&menu, "Item A", "", 10.00, CATEGORY_MAIN_COURSE);
    int item2 = menu_add_item(&menu, "Item B", "", 5.00, CATEGORY_BEVERAGE);

    int order_id = order_create(&system);
    order_add_item(&system, order_id, item1, 2);  // 20.00
    order_add_item(&system, order_id, item2, 3);  // 15.00

    double total = order_calculate_total(&system, order_id, &menu);
    assert(fabs(total - 35.00) < 0.001);
}

void test_order_confirm(void) {
    Menu menu;
    OrderSystem system;
    menu_init(&menu);
    order_system_init(&system);

    int item_id = menu_add_item(&menu, "Food", "", 10.00, CATEGORY_MAIN_COURSE);
    int order_id = order_create(&system);

    // Cannot confirm without items
    assert(order_confirm(&system, order_id) == 0);

    order_add_item(&system, order_id, item_id, 1);
    // Cannot confirm without address
    assert(order_confirm(&system, order_id) == 0);

    order_set_address(&system, order_id, "123 Main St");
    assert(order_confirm(&system, order_id) == 1);

    Order *order = order_find(&system, order_id);
    assert(order->status == STATUS_CONFIRMED);
}

void test_order_set_payment(void) {
    OrderSystem system;
    order_system_init(&system);

    int order_id = order_create(&system);
    assert(order_set_payment(&system, order_id, PAYMENT_PAYPAL) == 1);

    Order *order = order_find(&system, order_id);
    assert(order->payment_method == PAYMENT_PAYPAL);
}

void test_category_to_string(void) {
    assert(strcmp(category_to_string(CATEGORY_APPETIZER), "Appetizer") == 0);
    assert(strcmp(category_to_string(CATEGORY_MAIN_COURSE), "Main Course") == 0);
    assert(strcmp(category_to_string(CATEGORY_DESSERT), "Dessert") == 0);
    assert(strcmp(category_to_string(CATEGORY_BEVERAGE), "Beverage") == 0);
}

void test_status_to_string(void) {
    assert(strcmp(status_to_string(STATUS_PENDING), "Pending") == 0);
    assert(strcmp(status_to_string(STATUS_CONFIRMED), "Confirmed") == 0);
    assert(strcmp(status_to_string(STATUS_DELIVERED), "Delivered") == 0);
}

int main(void) {
    test_menu_init();
    test_menu_add_item();
    test_menu_find_item();
    test_order_system_init();
    test_order_create();
    test_order_add_item();
    test_order_calculate_total();
    test_order_confirm();
    test_order_set_payment();
    test_category_to_string();
    test_status_to_string();
    printf("All tests passed!\n");
    return 0;
}
