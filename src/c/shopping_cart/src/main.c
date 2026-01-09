#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shopping_cart.h"

void print_menu(void) {
    printf("\n=== Shopping Cart ===\n");
    printf("1. View products\n");
    printf("2. Add to cart\n");
    printf("3. View cart\n");
    printf("4. Update quantity\n");
    printf("5. Remove from cart\n");
    printf("6. Apply discount code\n");
    printf("7. Checkout\n");
    printf("8. Exit\n");
    printf("Choice: ");
}

int main(void) {
    Store store;
    Cart cart;
    char input[256];
    int running = 1;

    store_init(&store);
    cart_init(&cart);

    // Add sample products
    store_add_product(&store, "Laptop", 999.99, 10);
    store_add_product(&store, "Mouse", 29.99, 50);
    store_add_product(&store, "Keyboard", 79.99, 30);
    store_add_product(&store, "Monitor 24\"", 249.99, 15);
    store_add_product(&store, "USB Cable", 9.99, 100);
    store_add_product(&store, "Headphones", 149.99, 25);
    store_add_product(&store, "Webcam", 69.99, 20);
    store_add_product(&store, "Mouse Pad", 14.99, 40);

    printf("Welcome to the Online Store!\n");

    while (running) {
        print_menu();
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }

        int choice = atoi(input);

        switch (choice) {
            case 1:
                store_list_products(&store);
                break;

            case 2: {
                printf("Enter product ID: ");
                if (!fgets(input, sizeof(input), stdin)) break;
                int id = atoi(input);

                printf("Enter quantity: ");
                if (!fgets(input, sizeof(input), stdin)) break;
                int qty = atoi(input);

                if (cart_add_item(&cart, &store, id, qty)) {
                    printf("Added to cart.\n");
                } else {
                    printf("Failed to add. Check ID and stock.\n");
                }
                break;
            }

            case 3:
                cart_print(&cart, &store);
                break;

            case 4: {
                printf("Enter product ID: ");
                if (!fgets(input, sizeof(input), stdin)) break;
                int id = atoi(input);

                printf("Enter new quantity (0 to remove): ");
                if (!fgets(input, sizeof(input), stdin)) break;
                int qty = atoi(input);

                if (cart_update_quantity(&cart, &store, id, qty)) {
                    printf("Quantity updated.\n");
                } else {
                    printf("Failed to update.\n");
                }
                break;
            }

            case 5: {
                printf("Enter product ID to remove: ");
                if (!fgets(input, sizeof(input), stdin)) break;
                int id = atoi(input);

                if (cart_remove_item(&cart, id)) {
                    printf("Removed from cart.\n");
                } else {
                    printf("Item not found in cart.\n");
                }
                break;
            }

            case 6: {
                printf("Enter discount code: ");
                if (!fgets(input, sizeof(input), stdin)) break;
                input[strcspn(input, "\n")] = '\0';

                // Simple discount codes
                if (strcmp(input, "SAVE10") == 0) {
                    cart_apply_discount(&cart, 10.0);
                    printf("10%% discount applied!\n");
                } else if (strcmp(input, "SAVE20") == 0) {
                    cart_apply_discount(&cart, 20.0);
                    printf("20%% discount applied!\n");
                } else {
                    printf("Invalid discount code.\n");
                }
                break;
            }

            case 7:
                if (cart.item_count == 0) {
                    printf("Cart is empty.\n");
                } else {
                    cart_print(&cart, &store);
                    printf("Proceed with checkout? (y/n): ");
                    if (fgets(input, sizeof(input), stdin) && (input[0] == 'y' || input[0] == 'Y')) {
                        if (cart_checkout(&cart, &store)) {
                            printf("Checkout successful! Thank you for your purchase.\n");
                        } else {
                            printf("Checkout failed. Please check stock availability.\n");
                        }
                    } else {
                        printf("Checkout cancelled.\n");
                    }
                }
                break;

            case 8:
                running = 0;
                printf("Goodbye!\n");
                break;

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
