#include "shopping_cart.h"
#include <stdio.h>
#include <string.h>

static int next_product_id = 1;

void store_init(Store *store) {
    store->product_count = 0;
}

int store_add_product(Store *store, const char *name, double price, int stock) {
    if (store->product_count >= MAX_PRODUCTS) {
        return 0;
    }
    Product *p = &store->products[store->product_count];
    p->id = next_product_id++;
    strncpy(p->name, name, MAX_NAME_LENGTH - 1);
    p->name[MAX_NAME_LENGTH - 1] = '\0';
    p->price = price;
    p->stock = stock;
    store->product_count++;
    return p->id;
}

Product *store_find_product(Store *store, int id) {
    for (int i = 0; i < store->product_count; i++) {
        if (store->products[i].id == id) {
            return &store->products[i];
        }
    }
    return NULL;
}

void store_list_products(const Store *store) {
    printf("\n%-4s %-30s %10s %8s\n", "ID", "Product", "Price", "Stock");
    printf("------------------------------------------------------\n");
    for (int i = 0; i < store->product_count; i++) {
        const Product *p = &store->products[i];
        printf("%-4d %-30s %10.2f %8d\n", p->id, p->name, p->price, p->stock);
    }
    printf("\n");
}

void cart_init(Cart *cart) {
    cart->item_count = 0;
    cart->discount_percent = 0.0;
}

int cart_add_item(Cart *cart, Store *store, int product_id, int quantity) {
    if (quantity <= 0) {
        return 0;
    }

    Product *product = store_find_product(store, product_id);
    if (!product || product->stock < quantity) {
        return 0;
    }

    // Check if already in cart
    for (int i = 0; i < cart->item_count; i++) {
        if (cart->items[i].product_id == product_id) {
            if (product->stock < cart->items[i].quantity + quantity) {
                return 0;
            }
            cart->items[i].quantity += quantity;
            return 1;
        }
    }

    if (cart->item_count >= MAX_CART_ITEMS) {
        return 0;
    }

    cart->items[cart->item_count].product_id = product_id;
    cart->items[cart->item_count].quantity = quantity;
    cart->item_count++;
    return 1;
}

int cart_remove_item(Cart *cart, int product_id) {
    for (int i = 0; i < cart->item_count; i++) {
        if (cart->items[i].product_id == product_id) {
            // Shift items
            for (int j = i; j < cart->item_count - 1; j++) {
                cart->items[j] = cart->items[j + 1];
            }
            cart->item_count--;
            return 1;
        }
    }
    return 0;
}

int cart_update_quantity(Cart *cart, Store *store, int product_id, int quantity) {
    if (quantity <= 0) {
        return cart_remove_item(cart, product_id);
    }

    Product *product = store_find_product(store, product_id);
    if (!product || product->stock < quantity) {
        return 0;
    }

    for (int i = 0; i < cart->item_count; i++) {
        if (cart->items[i].product_id == product_id) {
            cart->items[i].quantity = quantity;
            return 1;
        }
    }
    return 0;
}

double cart_get_total(const Cart *cart, const Store *store) {
    double total = 0.0;
    for (int i = 0; i < cart->item_count; i++) {
        const Product *p = NULL;
        for (int j = 0; j < store->product_count; j++) {
            if (store->products[j].id == cart->items[i].product_id) {
                p = &store->products[j];
                break;
            }
        }
        if (p) {
            total += p->price * cart->items[i].quantity;
        }
    }
    double discount = total * cart->discount_percent / 100.0;
    return total - discount;
}

void cart_apply_discount(Cart *cart, double percent) {
    if (percent >= 0 && percent <= 100) {
        cart->discount_percent = percent;
    }
}

void cart_print(const Cart *cart, const Store *store) {
    if (cart->item_count == 0) {
        printf("\nCart is empty.\n\n");
        return;
    }

    printf("\n%-30s %10s %8s %12s\n", "Product", "Price", "Qty", "Subtotal");
    printf("--------------------------------------------------------------\n");

    double total = 0.0;
    for (int i = 0; i < cart->item_count; i++) {
        const Product *p = NULL;
        for (int j = 0; j < store->product_count; j++) {
            if (store->products[j].id == cart->items[i].product_id) {
                p = &store->products[j];
                break;
            }
        }
        if (p) {
            double subtotal = p->price * cart->items[i].quantity;
            printf("%-30s %10.2f %8d %12.2f\n", p->name, p->price,
                   cart->items[i].quantity, subtotal);
            total += subtotal;
        }
    }

    printf("--------------------------------------------------------------\n");
    printf("%50s %12.2f\n", "Subtotal:", total);
    if (cart->discount_percent > 0) {
        double discount = total * cart->discount_percent / 100.0;
        printf("%50s %12.2f\n", "Discount:", -discount);
        printf("%50s %12.2f\n", "Total:", total - discount);
    }
    printf("\n");
}

int cart_checkout(Cart *cart, Store *store) {
    for (int i = 0; i < cart->item_count; i++) {
        Product *p = store_find_product(store, cart->items[i].product_id);
        if (!p || p->stock < cart->items[i].quantity) {
            return 0;
        }
    }

    for (int i = 0; i < cart->item_count; i++) {
        Product *p = store_find_product(store, cart->items[i].product_id);
        if (p) {
            p->stock -= cart->items[i].quantity;
        }
    }

    cart_init(cart);
    return 1;
}
