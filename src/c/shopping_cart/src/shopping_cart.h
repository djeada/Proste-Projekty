#ifndef SHOPPING_CART_H
#define SHOPPING_CART_H

#define MAX_PRODUCTS 100
#define MAX_CART_ITEMS 50
#define MAX_NAME_LENGTH 64

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    double price;
    int stock;
} Product;

typedef struct {
    int product_id;
    int quantity;
} CartItem;

typedef struct {
    CartItem items[MAX_CART_ITEMS];
    int item_count;
    double discount_percent;
} Cart;

typedef struct {
    Product products[MAX_PRODUCTS];
    int product_count;
} Store;

void store_init(Store *store);
int store_add_product(Store *store, const char *name, double price, int stock);
Product *store_find_product(Store *store, int id);
void store_list_products(const Store *store);

void cart_init(Cart *cart);
int cart_add_item(Cart *cart, Store *store, int product_id, int quantity);
int cart_remove_item(Cart *cart, int product_id);
int cart_update_quantity(Cart *cart, Store *store, int product_id, int quantity);
double cart_get_total(const Cart *cart, const Store *store);
void cart_apply_discount(Cart *cart, double percent);
void cart_print(const Cart *cart, const Store *store);
int cart_checkout(Cart *cart, Store *store);

#endif // SHOPPING_CART_H
