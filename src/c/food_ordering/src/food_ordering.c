/* Food ordering rules: menu, order, validation, card check and order status. */
#include "food_ordering.h"

#include <ctype.h>
#include <string.h>

#define MIN_ADDRESS_LEN 5
#define MIN_PHONE_DIGITS 9
#define MAX_PHONE_DIGITS 15
#define MAX_CARD_DIGITS 19
#define MIN_CARD_DIGITS 13

/* Prices are integers in cents, so 9.90 is 990 and no rounding errors appear. */
const Dish menu[MENU_SIZE] = {
    {1, "Garlic Bread", "Toasted baguette with garlic butter", 450, CATEGORY_STARTER},
    {2, "Tomato Soup", "Slow-cooked tomato soup with basil", 520, CATEGORY_STARTER},
    {3, "Margherita Pizza", "Tomato, mozzarella and fresh basil", 1200, CATEGORY_MAIN},
    {4, "Chicken Pasta", "Penne with grilled chicken and pesto", 1350, CATEGORY_MAIN},
    {5, "Beef Burger", "Beef patty, cheddar, pickles and fries", 1450, CATEGORY_MAIN},
    {6, "Grilled Salmon", "Atlantic salmon with lemon and herbs", 1890, CATEGORY_MAIN},
    {7, "Cheesecake", "Baked cheesecake with berry sauce", 620, CATEGORY_DESSERT},
    {8, "Chocolate Cake", "Rich dark chocolate layer cake", 590, CATEGORY_DESSERT},
    {9, "Coffee", "Freshly brewed coffee", 290, CATEGORY_DRINK},
    {10, "Lemonade", "Homemade lemonade with mint", 350, CATEGORY_DRINK},
};

const Dish *dish_find(int id) {
    for (int i = 0; i < MENU_SIZE; i++) {
        if (menu[i].id == id) {
            return &menu[i];
        }
    }
    return NULL;
}

const char *category_name(Category category) {
    switch (category) {
        case CATEGORY_STARTER: return "Starters";
        case CATEGORY_MAIN: return "Main courses";
        case CATEGORY_DESSERT: return "Desserts";
        case CATEGORY_DRINK: return "Drinks";
        default: return "Unknown";
    }
}

const char *payment_name(Payment payment) {
    switch (payment) {
        case PAYMENT_PAYPAL: return "PayPal";
        case PAYMENT_CARD: return "Credit card";
        case PAYMENT_CASH: return "Cash on delivery";
        default: return "Not chosen";
    }
}

const char *status_name(Status status) {
    switch (status) {
        case STATUS_RECEIVED: return "Received";
        case STATUS_PREPARING: return "Preparing";
        case STATUS_ON_THE_WAY: return "On the way";
        case STATUS_DELIVERED: return "Delivered";
        default: return "Unknown";
    }
}

void order_init(Order *order) {
    memset(order, 0, sizeof(*order));
}

static int find_line(const Order *order, int dish_id) {
    for (int i = 0; i < order->line_count; i++) {
        if (order->lines[i].dish_id == dish_id) {
            return i;
        }
    }
    return -1;
}

int order_quantity(const Order *order, int dish_id) {
    int index = find_line(order, dish_id);
    return index < 0 ? 0 : order->lines[index].quantity;
}

bool order_set_quantity(Order *order, int dish_id, int quantity) {
    if (order->placed || !dish_find(dish_id) || quantity < 0 || quantity > MAX_QUANTITY) {
        return false;
    }
    int index = find_line(order, dish_id);
    if (quantity == 0) {
        if (index >= 0) {
            for (int i = index; i < order->line_count - 1; i++) {
                order->lines[i] = order->lines[i + 1];
            }
            order->line_count--;
        }
        return true;
    }
    if (index < 0) {
        index = order->line_count++;
        order->lines[index].dish_id = dish_id;
    }
    order->lines[index].quantity = quantity;
    return true;
}

bool order_add_dish(Order *order, int dish_id, int quantity) {
    if (quantity <= 0) {
        return false;
    }
    return order_set_quantity(order, dish_id, order_quantity(order, dish_id) + quantity);
}

int order_total_cents(const Order *order) {
    int total = 0;
    for (int i = 0; i < order->line_count; i++) {
        total += dish_find(order->lines[i].dish_id)->price_cents * order->lines[i].quantity;
    }
    return total;
}

/* Copies src without leading and trailing spaces. Fails if the result does not fit in size. */
static bool copy_trimmed(const char *src, char *dst, size_t size) {
    while (isspace((unsigned char)*src)) {
        src++;
    }
    size_t len = strlen(src);
    while (len > 0 && isspace((unsigned char)src[len - 1])) {
        len--;
    }
    if (len >= size) {
        return false;
    }
    memcpy(dst, src, len);
    dst[len] = '\0';
    return true;
}

/* A valid address has 5-100 characters, a house number (a digit) and a street name (a letter). */
bool order_set_address(Order *order, const char *address) {
    char clean[MAX_ADDRESS_LEN + 1];
    if (order->placed || !copy_trimmed(address, clean, sizeof(clean))) {
        return false;
    }
    size_t len = strlen(clean);
    int has_digit = 0;
    int has_letter = 0;
    for (size_t i = 0; i < len; i++) {
        has_digit = has_digit || isdigit((unsigned char)clean[i]);
        has_letter = has_letter || isalpha((unsigned char)clean[i]);
    }
    if (len < MIN_ADDRESS_LEN || !has_digit || !has_letter) {
        return false;
    }
    strcpy(order->address, clean);
    return true;
}

/* Spaces, dashes and brackets are ignored. The result keeps an optional leading '+' and 9-15 digits. */
bool order_set_phone(Order *order, const char *phone) {
    char clean[MAX_PHONE_LEN + 1];
    size_t n = 0;
    int digits = 0;
    if (order->placed) {
        return false;
    }
    for (const char *p = phone; *p; p++) {
        if (*p == '+' && n == 0) {
            clean[n++] = *p;
        } else if (isdigit((unsigned char)*p)) {
            if (digits == MAX_PHONE_DIGITS) {
                return false;
            }
            clean[n++] = *p;
            digits++;
        } else if (!isspace((unsigned char)*p) && *p != '-' && *p != '(' && *p != ')') {
            return false;
        }
    }
    if (digits < MIN_PHONE_DIGITS) {
        return false;
    }
    clean[n] = '\0';
    strcpy(order->phone, clean);
    return true;
}

bool order_set_payment(Order *order, Payment payment) {
    if (order->placed || payment == PAYMENT_NONE || payment == PAYMENT_CARD) {
        return false;
    }
    order->payment = payment;
    order->card_last4[0] = '\0';
    return true;
}

/* Collects the digits of a card number into digits. Returns their count, or -1 if the text is not a card number. */
static int card_digits(const char *text, int *digits) {
    int count = 0;
    for (const char *p = text; *p; p++) {
        if (isdigit((unsigned char)*p)) {
            if (count == MAX_CARD_DIGITS) {
                return -1;
            }
            digits[count++] = *p - '0';
        } else if (*p != ' ' && *p != '-') {
            return -1;
        }
    }
    return count >= MIN_CARD_DIGITS ? count : -1;
}

/* Luhn check: from the right, double every second digit (subtract 9 if the result is over 9),
 * add everything up. A real card number gives a sum divisible by 10. */
bool luhn_is_valid(const char *number) {
    int digits[MAX_CARD_DIGITS];
    int count = card_digits(number, digits);
    if (count < 0) {
        return false;
    }
    int sum = 0;
    for (int i = 0; i < count; i++) {
        int d = digits[count - 1 - i];
        if (i % 2 == 1) {
            d *= 2;
            if (d > 9) {
                d -= 9;
            }
        }
        sum += d;
    }
    return sum % 10 == 0;
}

bool order_set_card(Order *order, const char *number) {
    int digits[MAX_CARD_DIGITS];
    int count = card_digits(number, digits);
    if (order->placed || !luhn_is_valid(number)) {
        return false;
    }
    for (int i = 0; i < 4; i++) {
        order->card_last4[i] = (char)('0' + digits[count - 4 + i]);
    }
    order->card_last4[4] = '\0';
    order->payment = PAYMENT_CARD;
    return true;
}

const char *order_place(Order *order, long long now) {
    if (order->placed) {
        return "The order has already been placed.";
    }
    if (order->line_count == 0) {
        return "Your order is empty. Add a dish first.";
    }
    if (order->address[0] == '\0') {
        return "Please enter a delivery address.";
    }
    if (order->phone[0] == '\0') {
        return "Please enter a phone number.";
    }
    if (order->payment == PAYMENT_NONE) {
        return "Please choose a payment method.";
    }
    order->placed = true;
    order->placed_at = now;
    return NULL;
}

Status order_status_at(long long elapsed_seconds) {
    if (elapsed_seconds < STATUS_STEP_SECONDS) {
        return STATUS_RECEIVED;
    }
    if (elapsed_seconds < 2 * STATUS_STEP_SECONDS) {
        return STATUS_PREPARING;
    }
    if (elapsed_seconds < 3 * STATUS_STEP_SECONDS) {
        return STATUS_ON_THE_WAY;
    }
    return STATUS_DELIVERED;
}
