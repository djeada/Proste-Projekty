/* Shopping cart rules. Products are identified by their index in CATALOG. */
#include "shopping_cart.h"

#include <ctype.h>
#include <string.h>

const Product CATALOG[CATALOG_SIZE] = {
    {"Notebook", "Stationery", 349},
    {"Pen set", "Stationery", 1290},
    {"Coffee mug", "Kitchen", 875},
    {"Water bottle", "Kitchen", 1120},
    {"Desk lamp", "Home", 2499},
    {"T-shirt", "Clothing", 1500},
    {"Running socks", "Clothing", 650},
    {"Paperback novel", "Books", 999},
};

const DiscountCode DISCOUNT_CODES[DISCOUNT_CODE_COUNT] = {
    {"SAVE10", 10, 0, 0},
    {"FLAT5", 0, 500, 3000},
};

static int equal_ignore_case(const char *a, const char *b) {
    for (; *a != '\0' && *b != '\0'; a++, b++) {
        if (toupper((unsigned char)*a) != toupper((unsigned char)*b)) {
            return 0;
        }
    }
    return *a == *b;
}

void cart_clear(Cart *cart) {
    memset(cart, 0, sizeof *cart);
}

CartResult cart_add(Cart *cart, int product, int quantity) {
    if (product < 0 || product >= CATALOG_SIZE) {
        return CART_BAD_PRODUCT;
    }
    if (quantity < 1 || cart->quantities[product] + quantity > MAX_QUANTITY) {
        return CART_BAD_QUANTITY;
    }
    cart->quantities[product] += quantity;
    return CART_OK;
}

CartResult cart_set_quantity(Cart *cart, int product, int quantity) {
    if (product < 0 || product >= CATALOG_SIZE) {
        return CART_BAD_PRODUCT;
    }
    if (quantity < 0 || quantity > MAX_QUANTITY) {
        return CART_BAD_QUANTITY;
    }
    cart->quantities[product] = quantity;
    return CART_OK;
}

CartResult cart_apply_code(Cart *cart, const char *code) {
    for (int i = 0; i < DISCOUNT_CODE_COUNT; i++) {
        const DiscountCode *candidate = &DISCOUNT_CODES[i];
        if (!equal_ignore_case(code, candidate->code)) {
            continue;
        }
        if (cart_subtotal(cart) < candidate->min_order_cents) {
            return CART_MIN_ORDER;
        }
        cart->discount = candidate;
        return CART_OK;
    }
    return CART_UNKNOWN_CODE;
}

int cart_is_empty(const Cart *cart) {
    for (int i = 0; i < CATALOG_SIZE; i++) {
        if (cart->quantities[i] > 0) {
            return 0;
        }
    }
    return 1;
}

int cart_subtotal(const Cart *cart) {
    int sum = 0;
    for (int i = 0; i < CATALOG_SIZE; i++) {
        sum += CATALOG[i].price_cents * cart->quantities[i];
    }
    return sum;
}

int cart_discount(const Cart *cart) {
    const DiscountCode *code = cart->discount;
    int subtotal = cart_subtotal(cart);
    if (code == NULL || subtotal < code->min_order_cents) {
        return 0;
    }
    if (code->percent > 0) {
        return (subtotal * code->percent + 50) / 100; /* rounded to the nearest cent */
    }
    return code->amount_cents < subtotal ? code->amount_cents : subtotal;
}

int cart_total(const Cart *cart) {
    return cart_subtotal(cart) - cart_discount(cart);
}

CartResult validate_name(const char *name) {
    size_t length = strlen(name);
    int has_letter = 0;
    if (length < 2 || length > 40) {
        return CART_BAD_NAME;
    }
    for (size_t i = 0; i < length; i++) {
        if (isalpha((unsigned char)name[i])) {
            has_letter = 1;
        }
    }
    return has_letter ? CART_OK : CART_BAD_NAME;
}

CartResult validate_address(const char *address) {
    size_t length = strlen(address);
    int has_digit = 0;
    if (length < 5 || length > 80) {
        return CART_BAD_ADDRESS;
    }
    for (size_t i = 0; i < length; i++) {
        if (isdigit((unsigned char)address[i])) {
            has_digit = 1;
        }
    }
    return has_digit ? CART_OK : CART_BAD_ADDRESS;
}
