#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "food_ordering.h"

void print_menu_options(void) {
    printf("\n=== Food Ordering System ===\n");
    printf("1. View Menu\n");
    printf("2. View Menu by Category\n");
    printf("3. Create New Order\n");
    printf("4. Add Item to Order\n");
    printf("5. Remove Item from Order\n");
    printf("6. Set Delivery Address\n");
    printf("7. Set Payment Method\n");
    printf("8. View Order\n");
    printf("9. Confirm Order\n");
    printf("10. Track Order Status\n");
    printf("11. Cancel Order\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

void populate_sample_menu(Menu *menu) {
    // Appetizers
    menu_add_item(menu, "Spring Rolls", "Crispy vegetable spring rolls", 5.99, CATEGORY_APPETIZER);
    menu_add_item(menu, "Garlic Bread", "Toasted bread with garlic butter", 3.99, CATEGORY_APPETIZER);
    menu_add_item(menu, "Soup of the Day", "Ask server for today's selection", 4.99, CATEGORY_APPETIZER);
    
    // Main Courses
    menu_add_item(menu, "Grilled Salmon", "Fresh salmon with lemon butter sauce", 18.99, CATEGORY_MAIN_COURSE);
    menu_add_item(menu, "Beef Steak", "8oz ribeye with seasonal vegetables", 24.99, CATEGORY_MAIN_COURSE);
    menu_add_item(menu, "Chicken Pasta", "Creamy Alfredo pasta with grilled chicken", 14.99, CATEGORY_MAIN_COURSE);
    menu_add_item(menu, "Vegetable Stir-Fry", "Mixed vegetables with tofu", 12.99, CATEGORY_MAIN_COURSE);
    menu_add_item(menu, "Margherita Pizza", "Classic tomato, mozzarella, and basil", 13.99, CATEGORY_MAIN_COURSE);
    
    // Desserts
    menu_add_item(menu, "Chocolate Cake", "Rich chocolate layer cake", 6.99, CATEGORY_DESSERT);
    menu_add_item(menu, "Ice Cream", "Three scoops of your choice", 4.99, CATEGORY_DESSERT);
    menu_add_item(menu, "Cheesecake", "New York style cheesecake", 7.99, CATEGORY_DESSERT);
    
    // Beverages
    menu_add_item(menu, "Coffee", "Freshly brewed coffee", 2.99, CATEGORY_BEVERAGE);
    menu_add_item(menu, "Soft Drink", "Coca-Cola, Sprite, or Fanta", 1.99, CATEGORY_BEVERAGE);
    menu_add_item(menu, "Fresh Juice", "Orange or apple juice", 3.99, CATEGORY_BEVERAGE);
    menu_add_item(menu, "Mineral Water", "Still or sparkling", 1.49, CATEGORY_BEVERAGE);
}

int main(void) {
    Menu menu;
    OrderSystem order_system;
    char input[256];
    int running = 1;
    int current_order_id = -1;

    menu_init(&menu);
    order_system_init(&order_system);
    populate_sample_menu(&menu);

    printf("Welcome to the Food Ordering System!\n");

    while (running) {
        print_menu_options();
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }

        int choice = atoi(input);

        switch (choice) {
            case 0:
                running = 0;
                break;

            case 1:
                menu_display(&menu);
                break;

            case 2: {
                printf("Select category:\n");
                printf("1. Appetizer\n2. Main Course\n3. Dessert\n4. Beverage\n");
                printf("Choice: ");
                if (fgets(input, sizeof(input), stdin)) {
                    int cat = atoi(input) - 1;
                    if (cat >= 0 && cat <= 3) {
                        menu_display_by_category(&menu, (FoodCategory)cat);
                    } else {
                        printf("Invalid category.\n");
                    }
                }
                break;
            }

            case 3:
                current_order_id = order_create(&order_system);
                if (current_order_id > 0) {
                    printf("Created new order #%d\n", current_order_id);
                } else {
                    printf("Failed to create order.\n");
                }
                break;

            case 4:
                if (current_order_id < 0) {
                    printf("Create an order first (option 3).\n");
                } else {
                    printf("Enter item ID: ");
                    if (fgets(input, sizeof(input), stdin)) {
                        int item_id = atoi(input);
                        printf("Enter quantity: ");
                        if (fgets(input, sizeof(input), stdin)) {
                            int qty = atoi(input);
                            if (menu_find_item(&menu, item_id) && qty > 0) {
                                if (order_add_item(&order_system, current_order_id, item_id, qty)) {
                                    printf("Item added to order.\n");
                                } else {
                                    printf("Failed to add item.\n");
                                }
                            } else {
                                printf("Invalid item ID or quantity.\n");
                            }
                        }
                    }
                }
                break;

            case 5:
                if (current_order_id < 0) {
                    printf("No current order.\n");
                } else {
                    printf("Enter item ID to remove: ");
                    if (fgets(input, sizeof(input), stdin)) {
                        int item_id = atoi(input);
                        if (order_remove_item(&order_system, current_order_id, item_id)) {
                            printf("Item removed.\n");
                        } else {
                            printf("Item not found in order.\n");
                        }
                    }
                }
                break;

            case 6:
                if (current_order_id < 0) {
                    printf("No current order.\n");
                } else {
                    printf("Enter delivery address: ");
                    if (fgets(input, sizeof(input), stdin)) {
                        input[strcspn(input, "\n")] = '\0';
                        if (order_set_address(&order_system, current_order_id, input)) {
                            printf("Address set.\n");
                        } else {
                            printf("Failed to set address.\n");
                        }
                    }
                }
                break;

            case 7:
                if (current_order_id < 0) {
                    printf("No current order.\n");
                } else {
                    printf("Select payment method:\n");
                    printf("1. Cash on Delivery\n2. Credit Card\n3. PayPal\n");
                    printf("Choice: ");
                    if (fgets(input, sizeof(input), stdin)) {
                        int method = atoi(input) - 1;
                        if (method >= 0 && method <= 2) {
                            if (order_set_payment(&order_system, current_order_id, (PaymentMethod)method)) {
                                printf("Payment method set.\n");
                            }
                        } else {
                            printf("Invalid payment method.\n");
                        }
                    }
                }
                break;

            case 8:
                if (current_order_id < 0) {
                    printf("No current order.\n");
                } else {
                    Order *order = order_find(&order_system, current_order_id);
                    if (order) {
                        order_calculate_total(&order_system, current_order_id, &menu);
                        order_display(order, &menu);
                    }
                }
                break;

            case 9:
                if (current_order_id < 0) {
                    printf("No current order.\n");
                } else {
                    order_calculate_total(&order_system, current_order_id, &menu);
                    if (order_confirm(&order_system, current_order_id)) {
                        printf("Order #%d confirmed! Preparing your food...\n", current_order_id);
                        // Simulate order progression
                        order_update_status(&order_system, current_order_id, STATUS_PREPARING);
                        current_order_id = -1;  // Ready for new order
                    } else {
                        printf("Cannot confirm order. Make sure you have items and delivery address.\n");
                    }
                }
                break;

            case 10: {
                printf("Enter order ID to track: ");
                if (fgets(input, sizeof(input), stdin)) {
                    int order_id = atoi(input);
                    Order *order = order_find(&order_system, order_id);
                    if (order) {
                        printf("Order #%d status: %s\n", order_id, status_to_string(order->status));
                    } else {
                        printf("Order not found.\n");
                    }
                }
                break;
            }

            case 11:
                if (current_order_id < 0) {
                    printf("No current order to cancel.\n");
                } else {
                    if (order_update_status(&order_system, current_order_id, STATUS_CANCELLED)) {
                        printf("Order #%d cancelled.\n", current_order_id);
                        current_order_id = -1;
                    }
                }
                break;

            default:
                printf("Invalid option.\n");
        }
    }

    printf("Thank you for using our Food Ordering System!\n");
    return 0;
}
