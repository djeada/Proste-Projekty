"""
Python implementation of a shopping cart system.
"""
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
)

from src.python.shopping_cart.src.logic.cart import Store, Cart


def main() -> None:
    store = Store()
    cart = Cart(store)

    # Add sample products
    store.add_product("Apple", 0.99, 100)
    store.add_product("Banana", 0.59, 150)
    store.add_product("Orange", 1.29, 75)
    store.add_product("Milk (1L)", 2.49, 50)
    store.add_product("Bread", 1.99, 30)

    print("Shopping Cart Application")
    print("Commands: products, add <id> <qty>, remove <id>, cart, checkout, quit")
    print()

    while True:
        try:
            user_input = input("> ").strip()
        except (EOFError, KeyboardInterrupt):
            print("\nGoodbye!")
            break

        if not user_input:
            continue

        parts = user_input.split()
        command = parts[0].lower()

        if command == "quit" or command == "q":
            print("Goodbye!")
            break
        elif command == "products" or command == "ls":
            print(store.list_products())
        elif command == "add":
            if len(parts) < 3:
                print("Usage: add <product_id> <quantity>")
            else:
                try:
                    product_id = int(parts[1])
                    quantity = int(parts[2])
                    if cart.add_item(product_id, quantity):
                        product = store.find_product(product_id)
                        if product:
                            print(f"Added {quantity}x {product.name} to cart")
                    else:
                        print("Could not add item (invalid ID or insufficient stock)")
                except ValueError:
                    print("Please enter valid numbers")
        elif command == "remove" or command == "rm":
            if len(parts) < 2:
                print("Usage: remove <product_id>")
            else:
                try:
                    product_id = int(parts[1])
                    if cart.remove_item(product_id):
                        print("Item removed from cart")
                    else:
                        print("Item not in cart")
                except ValueError:
                    print("Please enter a valid number")
        elif command == "cart":
            print(cart.print_cart())
        elif command == "discount":
            if len(parts) < 2:
                print("Usage: discount <percent>")
            else:
                try:
                    percent = float(parts[1])
                    cart.apply_discount(percent)
                    print(f"Applied {percent}% discount")
                except ValueError:
                    print("Please enter a valid number")
        elif command == "checkout":
            total = cart.get_total()
            if cart.checkout():
                print(f"Purchase complete! Total: ${total:.2f}")
            else:
                print("Checkout failed - please check cart and stock")
        elif command == "clear":
            cart.clear()
            print("Cart cleared")
        else:
            print(f"Unknown command: {command}")
            print("Commands: products, add <id> <qty>, remove <id>, cart, checkout, quit")


if __name__ == "__main__":
    main()
