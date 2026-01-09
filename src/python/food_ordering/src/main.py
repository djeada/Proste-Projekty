"""
Python implementation of a food ordering system.
"""
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)

from src.python.food_ordering.src.logic.ordering import (
    Menu,
    OrderSystem,
    FoodCategory,
    PaymentMethod,
)


def main() -> None:
    menu = Menu()

    # Add sample menu items
    menu.add_item("Spring Rolls", "Crispy vegetable spring rolls", 5.99, FoodCategory.APPETIZER)
    menu.add_item("Soup of the Day", "Fresh homemade soup", 4.99, FoodCategory.APPETIZER)
    menu.add_item("Grilled Salmon", "Fresh Atlantic salmon with herbs", 18.99, FoodCategory.MAIN_COURSE)
    menu.add_item("Pasta Carbonara", "Classic Italian pasta dish", 14.99, FoodCategory.MAIN_COURSE)
    menu.add_item("Beef Burger", "Juicy beef burger with fries", 12.99, FoodCategory.MAIN_COURSE)
    menu.add_item("Chocolate Cake", "Rich chocolate layer cake", 6.99, FoodCategory.DESSERT)
    menu.add_item("Ice Cream", "Three scoops of ice cream", 4.99, FoodCategory.DESSERT)
    menu.add_item("Soda", "Soft drink", 2.49, FoodCategory.BEVERAGE)
    menu.add_item("Coffee", "Fresh brewed coffee", 2.99, FoodCategory.BEVERAGE)

    order_system = OrderSystem(menu)

    print("Food Ordering System")
    print("Commands: menu, order, add <id> <qty>, address <addr>, confirm, status, quit")
    print()

    current_order_id = None

    while True:
        try:
            user_input = input("> ").strip()
        except (EOFError, KeyboardInterrupt):
            print("\nGoodbye!")
            break

        if not user_input:
            continue

        parts = user_input.split(maxsplit=1)
        command = parts[0].lower()

        if command == "quit" or command == "q":
            print("Goodbye!")
            break
        elif command == "menu":
            print(menu.display())
        elif command == "order" or command == "new":
            current_order_id = order_system.create_order()
            print(f"Created new order #{current_order_id}")
        elif command == "add":
            if not current_order_id:
                print("Create an order first with 'order'")
            elif len(parts) < 2:
                print("Usage: add <menu_item_id> <quantity>")
            else:
                try:
                    add_parts = parts[1].split()
                    item_id = int(add_parts[0])
                    qty = int(add_parts[1]) if len(add_parts) > 1 else 1
                    if order_system.add_item_to_order(current_order_id, item_id, qty):
                        item = menu.find_item(item_id)
                        if item:
                            print(f"Added {qty}x {item.name} to order")
                    else:
                        print("Could not add item")
                except (ValueError, IndexError):
                    print("Usage: add <menu_item_id> <quantity>")
        elif command == "address":
            if not current_order_id:
                print("Create an order first with 'order'")
            elif len(parts) < 2:
                print("Usage: address <delivery address>")
            else:
                if order_system.set_delivery_address(current_order_id, parts[1]):
                    print(f"Delivery address set to: {parts[1]}")
                else:
                    print("Could not set address")
        elif command == "payment":
            if not current_order_id:
                print("Create an order first with 'order'")
            elif len(parts) < 2:
                print("Usage: payment <cash|card|paypal>")
            else:
                method_str = parts[1].lower()
                method_map = {
                    "cash": PaymentMethod.CASH,
                    "card": PaymentMethod.CARD,
                    "paypal": PaymentMethod.PAYPAL,
                }
                method = method_map.get(method_str)
                if method and order_system.set_payment_method(current_order_id, method):
                    print(f"Payment method set to: {method_str}")
                else:
                    print("Invalid payment method")
        elif command == "confirm":
            if not current_order_id:
                print("Create an order first with 'order'")
            elif order_system.confirm_order(current_order_id):
                total = order_system.calculate_total(current_order_id)
                print(f"Order confirmed! Total: ${total:.2f}")
            else:
                print("Could not confirm order. Make sure you have items and an address.")
        elif command == "status" or command == "view":
            if not current_order_id:
                print("Create an order first with 'order'")
            else:
                order_system.calculate_total(current_order_id)
                print(order_system.display_order(current_order_id))
        else:
            print(f"Unknown command: {command}")
            print("Commands: menu, order, add <id> <qty>, address <addr>, confirm, status, quit")


if __name__ == "__main__":
    main()
