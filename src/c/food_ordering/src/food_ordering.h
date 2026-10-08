/* Food ordering rules: menu, order, validation, card check and order status. No input or output here. */
#ifndef FOOD_ORDERING_H
#define FOOD_ORDERING_H

#include <stdbool.h>

#define MENU_SIZE 10
#define MAX_QUANTITY 20
#define MAX_ADDRESS_LEN 100
#define MAX_PHONE_LEN 16 /* an optional '+' and up to 15 digits */
#define STATUS_STEP_SECONDS 3

typedef enum {
    CATEGORY_STARTER,
    CATEGORY_MAIN,
    CATEGORY_DESSERT,
    CATEGORY_DRINK,
    CATEGORY_COUNT
} Category;

typedef struct {
    int id;
    const char *name;
    const char *description;
    int price_cents;
    Category category;
} Dish;

typedef enum { PAYMENT_NONE, PAYMENT_PAYPAL, PAYMENT_CARD, PAYMENT_CASH } Payment;

typedef enum { STATUS_RECEIVED, STATUS_PREPARING, STATUS_ON_THE_WAY, STATUS_DELIVERED } Status;

typedef struct {
    int dish_id;
    int quantity;
} OrderLine;

typedef struct {
    OrderLine lines[MENU_SIZE];
    int line_count;
    char address[MAX_ADDRESS_LEN + 1];
    char phone[MAX_PHONE_LEN + 1];
    Payment payment;
    char card_last4[5];
    bool placed;
    long long placed_at;
} Order;

extern const Dish menu[MENU_SIZE];

const Dish *dish_find(int id);
const char *category_name(Category category);
const char *payment_name(Payment payment);
const char *status_name(Status status);

void order_init(Order *order);
bool order_set_quantity(Order *order, int dish_id, int quantity); /* 0 removes the dish */
bool order_add_dish(Order *order, int dish_id, int quantity);
int order_quantity(const Order *order, int dish_id);
int order_total_cents(const Order *order);
bool order_set_address(Order *order, const char *address);
bool order_set_phone(Order *order, const char *phone);
bool order_set_payment(Order *order, Payment payment); /* PayPal or cash; cards use order_set_card */
bool order_set_card(Order *order, const char *number);
const char *order_place(Order *order, long long now); /* NULL on success, otherwise the reason */
Status order_status_at(long long elapsed_seconds);

bool luhn_is_valid(const char *number);

#endif
