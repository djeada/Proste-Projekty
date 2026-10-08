/* Terminal user interface: a numbered menu that uses the food ordering rules. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "food_ordering.h"

static void read_line(char *buf, size_t size) {
    fflush(stdout);
    if (!fgets(buf, (int)size, stdin)) {
        printf("\nGoodbye!\n");
        exit(0);
    }
    buf[strcspn(buf, "\n")] = '\0';
}

static int read_number(const char *prompt, int min, int max) {
    char line[64];
    char *end;
    for (;;) {
        printf("%s", prompt);
        read_line(line, sizeof(line));
        long value = strtol(line, &end, 10);
        if (end != line && *end == '\0' && value >= min && value <= max) {
            return (int)value;
        }
        printf("Please enter a number from %d to %d.\n", min, max);
    }
}

static void print_money(int cents, int width) {
    char text[16];
    snprintf(text, sizeof(text), "$%d.%02d", cents / 100, cents % 100);
    printf("%*s", width, text);
}

static void print_category(Category category) {
    printf("%s\n", category_name(category));
    for (int i = 0; i < MENU_SIZE; i++) {
        if (menu[i].category == category) {
            printf("  %2d. %-18s ", menu[i].id, menu[i].name);
            print_money(menu[i].price_cents, 6);
            printf("  %s\n", menu[i].description);
        }
    }
    printf("\n");
}

static void print_full_menu(void) {
    for (int c = 0; c < CATEGORY_COUNT; c++) {
        print_category(c);
    }
}

static void browse_menu(void) {
    for (int c = 0; c < CATEGORY_COUNT; c++) {
        printf("  %d. %s\n", c + 1, category_name(c));
    }
    int choice = read_number("Choose a category (0 to go back): ", 0, CATEGORY_COUNT);
    if (choice > 0) {
        printf("\n");
        print_category(choice - 1);
    }
}

static void show_order(const Order *order) {
    if (order->line_count == 0) {
        printf("Your order is empty.\n");
    }
    for (int i = 0; i < order->line_count; i++) {
        const Dish *dish = dish_find(order->lines[i].dish_id);
        printf("  %2d x %-18s ", order->lines[i].quantity, dish->name);
        print_money(dish->price_cents * order->lines[i].quantity, 7);
        printf("\n");
    }
    printf("  %-23s ", "Total:");
    print_money(order_total_cents(order), 7);
    printf("\n");
    printf("Address: %s\n", order->address[0] ? order->address : "(not set)");
    printf("Phone:   %s\n", order->phone[0] ? order->phone : "(not set)");
    if (order->payment == PAYMENT_CARD) {
        printf("Payment: Credit card ending in %s\n", order->card_last4);
    } else {
        printf("Payment: %s\n", payment_name(order->payment));
    }
}

static void add_dish(Order *order) {
    print_full_menu();
    int id = read_number("Dish number (0 to cancel): ", 0, MENU_SIZE);
    if (id == 0) {
        return;
    }
    int quantity = read_number("How many? (1-20): ", 1, MAX_QUANTITY);
    if (order_add_dish(order, id, quantity)) {
        printf("Added %d x %s.\n", quantity, dish_find(id)->name);
    } else {
        printf("Cannot add the dish: the order is placed or the quantity is too high.\n");
    }
}

static void change_quantity(Order *order) {
    show_order(order);
    if (order->line_count == 0) {
        return;
    }
    int id = read_number("Dish number to change (0 to cancel): ", 0, MENU_SIZE);
    if (id == 0) {
        return;
    }
    int quantity = read_number("New quantity (0 removes the dish): ", 0, MAX_QUANTITY);
    if (order_set_quantity(order, id, quantity)) {
        printf("Order updated.\n");
    } else {
        printf("Cannot change the order: it has already been placed.\n");
    }
}

static void card_details(Order *order) {
    char line[64];
    for (;;) {
        printf("Card number (spaces allowed, empty to cancel): ");
        read_line(line, sizeof(line));
        if (line[0] == '\0') {
            return;
        }
        if (order_set_card(order, line)) {
            printf("Card accepted, ending in %s.\n", order->card_last4);
            return;
        }
        printf("Invalid card: it needs 13-19 digits and must pass the Luhn check.\n");
    }
}

static void payment_method(Order *order) {
    if (order->placed) {
        printf("The order is placed, so the payment cannot be changed.\n");
        return;
    }
    printf("1. PayPal\n2. Credit card\n3. Cash on delivery\n0. Back\n");
    int choice = read_number("Choose: ", 0, 3);
    if (choice == 0) {
        return;
    }
    if (choice == 2) {
        card_details(order);
        return;
    }
    if (order_set_payment(order, choice == 1 ? PAYMENT_PAYPAL : PAYMENT_CASH)) {
        printf("Payment: %s.\n", payment_name(order->payment));
    } else {
        printf("Cannot change the payment: the order is placed.\n");
    }
}

/* Asks until the text is valid. An empty line skips the question. */
static void delivery_details(Order *order) {
    char line[256];
    if (order->placed) {
        printf("The order is placed, so the delivery details cannot be changed.\n");
        return;
    }
    printf("Empty lines keep the current values.\n");
    for (;;) {
        printf("Delivery address (street and house number): ");
        read_line(line, sizeof(line));
        if (line[0] == '\0' || order_set_address(order, line)) {
            break;
        }
        printf("Address must be 5-100 characters, with a street name and a house number.\n");
    }
    for (;;) {
        printf("Phone number (9-15 digits): ");
        read_line(line, sizeof(line));
        if (line[0] == '\0' || order_set_phone(order, line)) {
            break;
        }
        printf("Phone number must contain 9-15 digits (spaces, - and brackets are allowed).\n");
    }
}

static void place_order(Order *order) {
    const char *error = order_place(order, (long long)time(NULL));
    if (error) {
        printf("%s\n", error);
        return;
    }
    printf("Order placed! Total to pay: ");
    print_money(order_total_cents(order), 0);
    printf("\nUse option 8 to follow its status.\n");
}

static void track_order(Order *order) {
    if (!order->placed) {
        printf("No order has been placed yet.\n");
        return;
    }
    long long elapsed = (long long)time(NULL) - order->placed_at;
    Status status = order_status_at(elapsed);
    for (Status s = STATUS_RECEIVED; s <= STATUS_DELIVERED; s++) {
        char mark = s < status ? 'x' : (s == status ? '>' : ' ');
        printf("  [%c] %s\n", mark, status_name(s));
    }
    printf("%lld seconds after ordering.\n", elapsed);
    if (status == STATUS_DELIVERED) {
        printf("Delivered. Thank you for ordering! The order is closed, you can start a new one.\n");
        order_init(order);
    }
}

static void print_summary(const Order *order) {
    int items = 0;
    for (int i = 0; i < order->line_count; i++) {
        items += order->lines[i].quantity;
    }
    printf("Dishes: %d | Total: ", items);
    print_money(order_total_cents(order), 0);
    printf(" | %s\n", order->placed ? "order placed" : "not placed yet");
}

int main(void) {
    Order order;
    order_init(&order);
    printf("=== Food Ordering ===\n");
    for (;;) {
        printf("\n");
        print_summary(&order);
        printf("\n1. Browse the menu by category\n");
        printf("2. Add a dish to the order\n");
        printf("3. Change a quantity or remove a dish\n");
        printf("4. Show my order\n");
        printf("5. Delivery details\n");
        printf("6. Payment method\n");
        printf("7. Place the order\n");
        printf("8. Track order status\n");
        printf("0. Exit\n");
        int choice = read_number("Choose an option: ", 0, 8);
        printf("\n");
        switch (choice) {
            case 1: browse_menu(); break;
            case 2: add_dish(&order); break;
            case 3: change_quantity(&order); break;
            case 4: show_order(&order); break;
            case 5: delivery_details(&order); break;
            case 6: payment_method(&order); break;
            case 7: place_order(&order); break;
            case 8: track_order(&order); break;
            default:
                printf("Goodbye!\n");
                return 0;
        }
    }
}
