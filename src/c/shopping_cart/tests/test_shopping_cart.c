#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../src/shopping_cart.h"

void test_store_init() {
    Store store;
    store_init(&store);
    assert(store.product_count == 0);
}

void test_store_add_product() {
    Store store;
    store_init(&store);

    int id1 = store_add_product(&store, "Apple", 1.99, 100);
    assert(id1 > 0);
    assert(store.product_count == 1);

    int id2 = store_add_product(&store, "Banana", 0.99, 50);
    assert(id2 > id1);
    assert(store.product_count == 2);
}

void test_store_find_product() {
    Store store;
    store_init(&store);

    int id = store_add_product(&store, "Orange", 2.49, 30);
    Product *p = store_find_product(&store, id);

    assert(p != NULL);
    assert(strcmp(p->name, "Orange") == 0);
    assert(fabs(p->price - 2.49) < 0.001);
    assert(p->stock == 30);

    assert(store_find_product(&store, 9999) == NULL);
}

void test_cart_init() {
    Cart cart;
    cart_init(&cart);
    assert(cart.item_count == 0);
    assert(fabs(cart.discount_percent) < 0.001);
}

void test_cart_add_item() {
    Store store;
    Cart cart;
    store_init(&store);
    cart_init(&cart);

    int id = store_add_product(&store, "Item", 10.00, 5);

    assert(cart_add_item(&cart, &store, id, 2) == 1);
    assert(cart.item_count == 1);
    assert(cart.items[0].quantity == 2);

    // Add more of same item
    assert(cart_add_item(&cart, &store, id, 1) == 1);
    assert(cart.item_count == 1);
    assert(cart.items[0].quantity == 3);

    // Try to exceed stock
    assert(cart_add_item(&cart, &store, id, 10) == 0);
}

void test_cart_remove_item() {
    Store store;
    Cart cart;
    store_init(&store);
    cart_init(&cart);

    int id = store_add_product(&store, "Item", 10.00, 10);
    cart_add_item(&cart, &store, id, 2);

    assert(cart_remove_item(&cart, id) == 1);
    assert(cart.item_count == 0);

    // Remove non-existent
    assert(cart_remove_item(&cart, id) == 0);
}

void test_cart_get_total() {
    Store store;
    Cart cart;
    store_init(&store);
    cart_init(&cart);

    int id1 = store_add_product(&store, "A", 10.00, 10);
    int id2 = store_add_product(&store, "B", 5.00, 10);

    cart_add_item(&cart, &store, id1, 2);  // 20.00
    cart_add_item(&cart, &store, id2, 3);  // 15.00

    double total = cart_get_total(&cart, &store);
    assert(fabs(total - 35.00) < 0.001);
}

void test_cart_apply_discount() {
    Store store;
    Cart cart;
    store_init(&store);
    cart_init(&cart);

    int id = store_add_product(&store, "X", 100.00, 10);
    cart_add_item(&cart, &store, id, 1);

    cart_apply_discount(&cart, 10.0);
    double total = cart_get_total(&cart, &store);
    assert(fabs(total - 90.00) < 0.001);
}

void test_cart_checkout() {
    Store store;
    Cart cart;
    store_init(&store);
    cart_init(&cart);

    int id = store_add_product(&store, "Product", 50.00, 5);
    cart_add_item(&cart, &store, id, 3);

    assert(cart_checkout(&cart, &store) == 1);
    assert(cart.item_count == 0);

    Product *p = store_find_product(&store, id);
    assert(p->stock == 2);
}

int main() {
    test_store_init();
    test_store_add_product();
    test_store_find_product();
    test_cart_init();
    test_cart_add_item();
    test_cart_remove_item();
    test_cart_get_total();
    test_cart_apply_discount();
    test_cart_checkout();
    printf("All tests passed!\n");
    return 0;
}
