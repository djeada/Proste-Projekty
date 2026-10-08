/* Terminal menu for the shopping cart. */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shopping_cart.h"

#define LINE_SIZE 128

/* Reads one line without surrounding spaces. Returns 0 at end of input. */
static int read_line(char *buf, size_t size) {
    char *start = buf;
    size_t length;

    if (fgets(buf, (int)size, stdin) == NULL) {
        return 0;
    }
    while (isspace((unsigned char)*start)) {
        start++;
    }
    memmove(buf, start, strlen(start) + 1);
    length = strlen(buf);
    while (length > 0 && isspace((unsigned char)buf[length - 1])) {
        buf[--length] = '\0';
    }
    return 1;
}

static int parse_int(const char *text, int *value) {
    char *end;
    long number;

    if (*text == '\0') {
        return 0;
    }
    number = strtol(text, &end, 10);
    if (*end != '\0' || number < -100000 || number > 100000) {
        return 0;
    }
    *value = (int)number;
    return 1;
}

static void format_money(int cents, char *buf, size_t size) {
    snprintf(buf, size, "%s$%d.%02d", cents < 0 ? "-" : "", abs(cents) / 100, abs(cents) % 100);
}

static const char *result_message(CartResult result) {
    switch (result) {
    case CART_OK:
        return "Done.";
    case CART_BAD_PRODUCT:
        return "There is no product with that number.";
    case CART_BAD_QUANTITY:
        return "Quantity is out of range: a product can be in the cart at most 99 times.";
    case CART_UNKNOWN_CODE:
        return "Unknown discount code.";
    case CART_MIN_ORDER:
        return "This code needs a higher order value (see DISCOUNT_CODES in the source).";
    case CART_BAD_NAME:
        return "Name must be 2-40 characters and contain a letter.";
    case CART_BAD_ADDRESS:
        return "Address must be 5-80 characters and contain a house number (a digit).";
    }
    return "Unknown error.";
}

/* Asks until the answer is a valid product number. Returns the 0-based index, or -1 at end of input. */
static int ask_product(void) {
    char line[LINE_SIZE];
    int number;

    for (;;) {
        printf("Product number (1-%d): ", CATALOG_SIZE);
        if (!read_line(line, sizeof line)) {
            return -1;
        }
        if (parse_int(line, &number) && number >= 1 && number <= CATALOG_SIZE) {
            return number - 1;
        }
        puts("Please type a number from the catalog.");
    }
}

/* An empty answer gives blank_value. Returns 0 at end of input. */
static int ask_quantity(const char *prompt, int blank_value, int *quantity) {
    char line[LINE_SIZE];

    for (;;) {
        printf("%s", prompt);
        if (!read_line(line, sizeof line)) {
            return 0;
        }
        if (line[0] == '\0') {
            *quantity = blank_value;
            return 1;
        }
        if (parse_int(line, quantity) && *quantity >= 0 && *quantity <= MAX_QUANTITY) {
            return 1;
        }
        puts("Please type a number from 0 to 99.");
    }
}

static void print_money_row(const char *label, int cents) {
    char money[32];

    format_money(cents, money, sizeof money);
    printf("%-32s %10s\n", label, money);
}

static void print_cart_lines(const Cart *cart) {
    char price[32];
    char line_total[32];

    printf("%-16s %4s %10s %10s\n", "Product", "Qty", "Price", "Line");
    for (int i = 0; i < CATALOG_SIZE; i++) {
        int quantity = cart->quantities[i];
        if (quantity == 0) {
            continue;
        }
        format_money(CATALOG[i].price_cents, price, sizeof price);
        format_money(quantity * CATALOG[i].price_cents, line_total, sizeof line_total);
        printf("%-16s %4d %10s %10s\n", CATALOG[i].name, quantity, price, line_total);
    }
}

static void show_catalog(void) {
    char price[32];

    printf("\n%-3s %-16s %-12s %10s\n", "#", "Product", "Category", "Price");
    for (int i = 0; i < CATALOG_SIZE; i++) {
        format_money(CATALOG[i].price_cents, price, sizeof price);
        printf("%-3d %-16s %-12s %10s\n", i + 1, CATALOG[i].name, CATALOG[i].category, price);
    }
}

static void print_totals(const Cart *cart) {
    char label[64];

    print_money_row("Subtotal", cart_subtotal(cart));
    if (cart->discount != NULL) {
        snprintf(label, sizeof label, "Discount (%s)", cart->discount->code);
        print_money_row(label, -cart_discount(cart));
    }
    print_money_row("Total", cart_total(cart));
}

static void show_cart(const Cart *cart) {
    if (cart_is_empty(cart)) {
        puts("\nYour cart is empty.");
        return;
    }
    puts("");
    print_cart_lines(cart);
    puts("");
    print_totals(cart);
}

static void add_product(Cart *cart) {
    int product = ask_product();
    int quantity;
    CartResult result;

    if (product < 0) {
        return;
    }
    if (!ask_quantity("Quantity [1]: ", 1, &quantity)) {
        return;
    }
    result = cart_add(cart, product, quantity);
    puts(result_message(result));
}

static void change_quantity(Cart *cart) {
    int product = ask_product();
    int quantity;

    if (product < 0) {
        return;
    }
    if (!ask_quantity("New quantity (0 removes it): ", -1, &quantity)) {
        return;
    }
    puts(result_message(cart_set_quantity(cart, product, quantity)));
}

static void remove_product(Cart *cart) {
    int product = ask_product();

    if (product < 0) {
        return;
    }
    puts(result_message(cart_set_quantity(cart, product, 0)));
}

static void apply_code(Cart *cart) {
    char code[LINE_SIZE];

    printf("Discount code (try SAVE10 or FLAT5): ");
    if (!read_line(code, sizeof code)) {
        return;
    }
    puts(result_message(cart_apply_code(cart, code)));
}

/* Asks for a field until it is valid. Returns 0 at end of input. */
static int ask_valid(const char *prompt, char *buf, size_t size, CartResult (*validate)(const char *)) {
    CartResult result;

    for (;;) {
        printf("%s", prompt);
        if (!read_line(buf, size)) {
            return 0;
        }
        result = validate(buf);
        if (result == CART_OK) {
            return 1;
        }
        puts(result_message(result));
    }
}

static void checkout(Cart *cart) {
    char name[LINE_SIZE];
    char address[LINE_SIZE];

    if (cart_is_empty(cart)) {
        puts("\nYour cart is empty.");
        return;
    }
    if (!ask_valid("Full name: ", name, sizeof name, validate_name) ||
        !ask_valid("Delivery address: ", address, sizeof address, validate_address)) {
        return;
    }

    printf("\n=== Order summary ===\n%s\n%s\n\n", name, address);
    print_cart_lines(cart);
    puts("");
    print_totals(cart);
    puts("\nThank you for your order!");
    cart_clear(cart);
}

static void print_menu(void) {
    puts("\n=== Shopping Cart ===");
    puts("1. Show catalog");
    puts("2. Add product to cart");
    puts("3. Change quantity");
    puts("4. Remove product");
    puts("5. Show cart");
    puts("6. Apply discount code");
    puts("7. Checkout");
    puts("0. Quit");
    printf("Choose an option: ");
}

int main(void) {
    Cart cart;
    char line[LINE_SIZE];
    int choice;

    cart_clear(&cart);
    for (;;) {
        print_menu();
        if (!read_line(line, sizeof line)) {
            break;
        }
        if (!parse_int(line, &choice)) {
            choice = -1;
        }
        switch (choice) {
        case 1:
            show_catalog();
            break;
        case 2:
            add_product(&cart);
            break;
        case 3:
            change_quantity(&cart);
            break;
        case 4:
            remove_product(&cart);
            break;
        case 5:
            show_cart(&cart);
            break;
        case 6:
            apply_code(&cart);
            break;
        case 7:
            checkout(&cart);
            break;
        case 0:
            return 0;
        default:
            puts("Unknown option.");
        }
    }
    return 0;
}
