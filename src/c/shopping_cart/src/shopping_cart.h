/* Shopping cart rules: catalog, quantities, discount codes, totals and validation. No I/O here. */
#ifndef SHOPPING_CART_H
#define SHOPPING_CART_H

#include <stddef.h>

#define CATALOG_SIZE 8
#define DISCOUNT_CODE_COUNT 2
#define MAX_QUANTITY 99

/* All money is in integer cents: 0.10 cannot be stored exactly in a float. */
typedef struct {
    const char *name;
    const char *category;
    int price_cents;
} Product;

typedef struct {
    const char *code;
    int percent;      /* percentage discount, or 0 */
    int amount_cents; /* fixed discount, used when percent is 0 */
    int min_order_cents;
} DiscountCode;

typedef struct {
    int quantities[CATALOG_SIZE]; /* quantity of CATALOG[i] in the cart */
    const DiscountCode *discount; /* applied code, or NULL */
} Cart;

typedef enum {
    CART_OK,
    CART_BAD_PRODUCT,
    CART_BAD_QUANTITY,
    CART_UNKNOWN_CODE,
    CART_MIN_ORDER,
    CART_BAD_NAME,
    CART_BAD_ADDRESS
} CartResult;

extern const Product CATALOG[CATALOG_SIZE];
extern const DiscountCode DISCOUNT_CODES[DISCOUNT_CODE_COUNT];

void cart_clear(Cart *cart);
CartResult cart_add(Cart *cart, int product, int quantity);
CartResult cart_set_quantity(Cart *cart, int product, int quantity);
CartResult cart_apply_code(Cart *cart, const char *code);
int cart_is_empty(const Cart *cart);
int cart_subtotal(const Cart *cart);
int cart_discount(const Cart *cart);
int cart_total(const Cart *cart);
CartResult validate_name(const char *name);
CartResult validate_address(const char *address);

#endif /* SHOPPING_CART_H */
