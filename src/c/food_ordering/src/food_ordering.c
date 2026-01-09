#include "food_ordering.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int next_menu_item_id = 1;

const char *category_to_string(FoodCategory category) {
    switch (category) {
        case CATEGORY_APPETIZER: return "Appetizer";
        case CATEGORY_MAIN_COURSE: return "Main Course";
        case CATEGORY_DESSERT: return "Dessert";
        case CATEGORY_BEVERAGE: return "Beverage";
        case CATEGORY_OTHER: return "Other";
        default: return "Unknown";
    }
}

const char *status_to_string(OrderStatus status) {
    switch (status) {
        case STATUS_PENDING: return "Pending";
        case STATUS_CONFIRMED: return "Confirmed";
        case STATUS_PREPARING: return "Preparing";
        case STATUS_OUT_FOR_DELIVERY: return "Out for Delivery";
        case STATUS_DELIVERED: return "Delivered";
        case STATUS_CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

const char *payment_to_string(PaymentMethod method) {
    switch (method) {
        case PAYMENT_CASH: return "Cash on Delivery";
        case PAYMENT_CARD: return "Credit Card";
        case PAYMENT_PAYPAL: return "PayPal";
        default: return "Unknown";
    }
}

void menu_init(Menu *menu) {
    if (!menu) return;
    menu->item_count = 0;
}

int menu_add_item(Menu *menu, const char *name, const char *description,
                  double price, FoodCategory category) {
    if (!menu || !name || menu->item_count >= MAX_MENU_ITEMS) {
        return 0;
    }

    MenuItem *item = &menu->items[menu->item_count];
    item->id = next_menu_item_id++;
    strncpy(item->name, name, MAX_NAME_LENGTH - 1);
    item->name[MAX_NAME_LENGTH - 1] = '\0';
    
    if (description) {
        strncpy(item->description, description, MAX_DESCRIPTION_LENGTH - 1);
        item->description[MAX_DESCRIPTION_LENGTH - 1] = '\0';
    } else {
        item->description[0] = '\0';
    }
    
    item->price = price;
    item->category = category;
    item->available = 1;
    menu->item_count++;

    return item->id;
}

MenuItem *menu_find_item(Menu *menu, int id) {
    if (!menu) return NULL;
    
    for (int i = 0; i < menu->item_count; i++) {
        if (menu->items[i].id == id) {
            return &menu->items[i];
        }
    }
    return NULL;
}

void menu_display(const Menu *menu) {
    if (!menu) return;

    printf("\n========== MENU ==========\n\n");
    
    for (int cat = CATEGORY_APPETIZER; cat <= CATEGORY_OTHER; cat++) {
        int has_items = 0;
        for (int i = 0; i < menu->item_count; i++) {
            if (menu->items[i].category == cat) {
                has_items = 1;
                break;
            }
        }
        
        if (has_items) {
            printf("--- %s ---\n", category_to_string((FoodCategory)cat));
            for (int i = 0; i < menu->item_count; i++) {
                const MenuItem *item = &menu->items[i];
                if (item->category == cat) {
                    printf("%2d. %-25s $%.2f%s\n", 
                           item->id, item->name, item->price,
                           item->available ? "" : " (N/A)");
                    if (item->description[0]) {
                        printf("    %s\n", item->description);
                    }
                }
            }
            printf("\n");
        }
    }
}

void menu_display_by_category(const Menu *menu, FoodCategory category) {
    if (!menu) return;

    printf("\n--- %s ---\n", category_to_string(category));
    for (int i = 0; i < menu->item_count; i++) {
        const MenuItem *item = &menu->items[i];
        if (item->category == category && item->available) {
            printf("%2d. %-25s $%.2f\n", item->id, item->name, item->price);
            if (item->description[0]) {
                printf("    %s\n", item->description);
            }
        }
    }
}

void order_system_init(OrderSystem *system) {
    if (!system) return;
    system->order_count = 0;
    system->next_order_id = 1;
}

int order_create(OrderSystem *system) {
    if (!system || system->order_count >= 100) {
        return -1;
    }

    Order *order = &system->orders[system->order_count];
    memset(order, 0, sizeof(Order));
    order->id = system->next_order_id++;
    order->item_count = 0;
    order->status = STATUS_PENDING;
    order->payment_method = PAYMENT_CASH;
    order->total = 0.0;
    system->order_count++;

    return order->id;
}

Order *order_find(OrderSystem *system, int order_id) {
    if (!system) return NULL;
    
    for (int i = 0; i < system->order_count; i++) {
        if (system->orders[i].id == order_id) {
            return &system->orders[i];
        }
    }
    return NULL;
}

int order_add_item(OrderSystem *system, int order_id, int menu_item_id, int quantity) {
    if (!system || quantity <= 0) return 0;

    Order *order = order_find(system, order_id);
    if (!order || order->status != STATUS_PENDING) return 0;

    // Check if item already in order
    for (int i = 0; i < order->item_count; i++) {
        if (order->items[i].menu_item_id == menu_item_id) {
            order->items[i].quantity += quantity;
            return 1;
        }
    }

    // Add new item
    if (order->item_count >= MAX_ORDER_ITEMS) return 0;

    order->items[order->item_count].menu_item_id = menu_item_id;
    order->items[order->item_count].quantity = quantity;
    order->item_count++;

    return 1;
}

int order_remove_item(OrderSystem *system, int order_id, int menu_item_id) {
    if (!system) return 0;

    Order *order = order_find(system, order_id);
    if (!order || order->status != STATUS_PENDING) return 0;

    for (int i = 0; i < order->item_count; i++) {
        if (order->items[i].menu_item_id == menu_item_id) {
            // Shift remaining items
            for (int j = i; j < order->item_count - 1; j++) {
                order->items[j] = order->items[j + 1];
            }
            order->item_count--;
            return 1;
        }
    }
    return 0;
}

int order_set_address(OrderSystem *system, int order_id, const char *address) {
    if (!system || !address) return 0;

    Order *order = order_find(system, order_id);
    if (!order) return 0;

    strncpy(order->delivery_address, address, MAX_ADDRESS_LENGTH - 1);
    order->delivery_address[MAX_ADDRESS_LENGTH - 1] = '\0';
    return 1;
}

int order_set_payment(OrderSystem *system, int order_id, PaymentMethod method) {
    if (!system) return 0;

    Order *order = order_find(system, order_id);
    if (!order) return 0;

    order->payment_method = method;
    return 1;
}

double order_calculate_total(OrderSystem *system, int order_id, const Menu *menu) {
    if (!system || !menu) return 0.0;

    Order *order = order_find(system, order_id);
    if (!order) return 0.0;

    double total = 0.0;
    for (int i = 0; i < order->item_count; i++) {
        for (int j = 0; j < menu->item_count; j++) {
            if (menu->items[j].id == order->items[i].menu_item_id) {
                total += menu->items[j].price * order->items[i].quantity;
                break;
            }
        }
    }

    order->total = total;
    return total;
}

int order_confirm(OrderSystem *system, int order_id) {
    if (!system) return 0;

    Order *order = order_find(system, order_id);
    if (!order || order->status != STATUS_PENDING) return 0;
    if (order->item_count == 0) return 0;
    if (order->delivery_address[0] == '\0') return 0;

    order->status = STATUS_CONFIRMED;
    return 1;
}

int order_update_status(OrderSystem *system, int order_id, OrderStatus status) {
    if (!system) return 0;

    Order *order = order_find(system, order_id);
    if (!order) return 0;

    order->status = status;
    return 1;
}

void order_display(const Order *order, const Menu *menu) {
    if (!order || !menu) return;

    printf("\n========== ORDER #%d ==========\n", order->id);
    printf("Status: %s\n", status_to_string(order->status));
    printf("Payment: %s\n", payment_to_string(order->payment_method));
    if (order->delivery_address[0]) {
        printf("Delivery: %s\n", order->delivery_address);
    }
    printf("\nItems:\n");
    printf("%-25s %8s %10s\n", "Name", "Qty", "Subtotal");
    printf("----------------------------------------\n");

    double total = 0.0;
    for (int i = 0; i < order->item_count; i++) {
        for (int j = 0; j < menu->item_count; j++) {
            if (menu->items[j].id == order->items[i].menu_item_id) {
                double subtotal = menu->items[j].price * order->items[i].quantity;
                printf("%-25s %8d $%9.2f\n",
                       menu->items[j].name,
                       order->items[i].quantity,
                       subtotal);
                total += subtotal;
                break;
            }
        }
    }
    printf("----------------------------------------\n");
    printf("%-25s %8s $%9.2f\n", "TOTAL", "", total);
    printf("\n");
}
