#ifndef FOOD_ORDERING_H
#define FOOD_ORDERING_H

#define MAX_NAME_LENGTH 64
#define MAX_DESCRIPTION_LENGTH 256
#define MAX_ADDRESS_LENGTH 256
#define MAX_MENU_ITEMS 50
#define MAX_ORDER_ITEMS 20
#define MAX_CATEGORIES 10

typedef enum {
    CATEGORY_APPETIZER,
    CATEGORY_MAIN_COURSE,
    CATEGORY_DESSERT,
    CATEGORY_BEVERAGE,
    CATEGORY_OTHER
} FoodCategory;

typedef enum {
    PAYMENT_CASH,
    PAYMENT_CARD,
    PAYMENT_PAYPAL
} PaymentMethod;

typedef enum {
    STATUS_PENDING,
    STATUS_CONFIRMED,
    STATUS_PREPARING,
    STATUS_OUT_FOR_DELIVERY,
    STATUS_DELIVERED,
    STATUS_CANCELLED
} OrderStatus;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    double price;
    FoodCategory category;
    int available;
} MenuItem;

typedef struct {
    int menu_item_id;
    int quantity;
} OrderItem;

typedef struct {
    int id;
    OrderItem items[MAX_ORDER_ITEMS];
    int item_count;
    char delivery_address[MAX_ADDRESS_LENGTH];
    PaymentMethod payment_method;
    OrderStatus status;
    double total;
} Order;

typedef struct {
    MenuItem items[MAX_MENU_ITEMS];
    int item_count;
} Menu;

typedef struct {
    Order orders[100];
    int order_count;
    int next_order_id;
} OrderSystem;

// Menu functions
void menu_init(Menu *menu);
int menu_add_item(Menu *menu, const char *name, const char *description, 
                  double price, FoodCategory category);
MenuItem *menu_find_item(Menu *menu, int id);
void menu_display(const Menu *menu);
void menu_display_by_category(const Menu *menu, FoodCategory category);
const char *category_to_string(FoodCategory category);

// Order functions
void order_system_init(OrderSystem *system);
int order_create(OrderSystem *system);
int order_add_item(OrderSystem *system, int order_id, int menu_item_id, int quantity);
int order_remove_item(OrderSystem *system, int order_id, int menu_item_id);
int order_set_address(OrderSystem *system, int order_id, const char *address);
int order_set_payment(OrderSystem *system, int order_id, PaymentMethod method);
double order_calculate_total(OrderSystem *system, int order_id, const Menu *menu);
int order_confirm(OrderSystem *system, int order_id);
int order_update_status(OrderSystem *system, int order_id, OrderStatus status);
Order *order_find(OrderSystem *system, int order_id);
void order_display(const Order *order, const Menu *menu);
const char *status_to_string(OrderStatus status);
const char *payment_to_string(PaymentMethod method);

#endif // FOOD_ORDERING_H
