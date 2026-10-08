"""Terminal user interface: a numbered menu that uses the food ordering rules."""
import sys
import time

import food_ordering as fo


def ask(prompt: str) -> str:
    try:
        return input(prompt).strip()
    except EOFError:
        print("\nGoodbye!")
        sys.exit(0)


def ask_number(prompt: str, low: int, high: int) -> int:
    while True:
        text = ask(prompt)
        if text.isdecimal() and low <= int(text) <= high:
            return int(text)
        print(f"Please enter a number from {low} to {high}.")


def money(cents: int) -> str:
    return f"${cents // 100}.{cents % 100:02d}"


def print_category(category: fo.Category) -> None:
    print(category.value)
    for dish in fo.MENU:
        if dish.category is category:
            print(f"  {dish.id:2}. {dish.name:<18} {money(dish.price_cents):>6}  {dish.description}")
    print()


def browse_menu() -> None:
    for number, category in enumerate(fo.Category, start=1):
        print(f"  {number}. {category.value}")
    choice = ask_number("Choose a category (0 to go back): ", 0, len(fo.Category))
    if choice:
        print()
        print_category(list(fo.Category)[choice - 1])


def show_order(order: fo.Order) -> None:
    if not order.lines:
        print("Your order is empty.")
    for dish_id, quantity in order.lines.items():
        dish = fo.dish_by_id(dish_id)
        print(f"  {quantity:2} x {dish.name:<18} {money(dish.price_cents * quantity):>6}")
    print(f"  {'Total:':<23} {money(fo.total_cents(order)):>6}")
    print(f"Address: {order.address or '(not set)'}")
    print(f"Phone:   {order.phone or '(not set)'}")
    if order.payment is fo.Payment.CARD:
        print(f"Payment: Credit card ending in {order.card_last4}")
    else:
        print(f"Payment: {order.payment.value if order.payment else '(not chosen)'}")


def add_dish(order: fo.Order) -> None:
    for category in fo.Category:
        print_category(category)
    dish_id = ask_number("Dish number (0 to cancel): ", 0, len(fo.MENU))
    if dish_id == 0:
        return
    quantity = ask_number(f"How many? (1-{fo.MAX_QUANTITY}): ", 1, fo.MAX_QUANTITY)
    if fo.add_dish(order, dish_id, quantity):
        print(f"Added {quantity} x {fo.dish_by_id(dish_id).name}.")
    else:
        print("Cannot add the dish: the order is placed or the quantity is too high.")


def change_quantity(order: fo.Order) -> None:
    show_order(order)
    if not order.lines:
        return
    dish_id = ask_number("Dish number to change (0 to cancel): ", 0, len(fo.MENU))
    if dish_id == 0:
        return
    quantity = ask_number(f"New quantity (0 removes the dish, max {fo.MAX_QUANTITY}): ", 0, fo.MAX_QUANTITY)
    if fo.set_quantity(order, dish_id, quantity):
        print("Order updated.")
    else:
        print("Cannot change the order: it has already been placed.")


def card_details(order: fo.Order) -> None:
    while True:
        number = ask("Card number (spaces allowed, empty to cancel): ")
        if not number:
            return
        if fo.set_card(order, number):
            print(f"Card accepted, ending in {order.card_last4}.")
            return
        print("Invalid card: it needs 13-19 digits and must pass the Luhn check.")


def payment_method(order: fo.Order) -> None:
    if order.placed:
        print("The order is placed, so the payment cannot be changed.")
        return
    print("1. PayPal\n2. Credit card\n3. Cash on delivery\n0. Back")
    choice = ask_number("Choose: ", 0, 3)
    if choice == 0:
        return
    if choice == 2:
        card_details(order)
    elif fo.set_payment(order, [fo.Payment.PAYPAL, fo.Payment.CASH][choice - 1]):
        print(f"Payment: {order.payment.value}.")


def delivery_details(order: fo.Order) -> None:
    if order.placed:
        print("The order is placed, so the delivery details cannot be changed.")
        return
    print("Empty lines keep the current values.")
    while True:
        address = ask("Delivery address (street and house number): ")
        if not address or fo.set_address(order, address):
            break
        print("Address must be 5-100 characters, with a street name and a house number.")
    while True:
        phone = ask("Phone number (9-15 digits): ")
        if not phone or fo.set_phone(order, phone):
            break
        print("Phone number must contain 9-15 digits (spaces, - and brackets are allowed).")


def place_order(order: fo.Order) -> None:
    error = fo.place_order(order, time.time())
    if error:
        print(error)
        return
    print(f"Order placed! Total to pay: {money(fo.total_cents(order))}")
    print("Use option 8 to follow its status.")


def track_order(order: fo.Order) -> bool:
    """Print the status. Returns True when the order has been delivered."""
    if not order.placed:
        print("No order has been placed yet.")
        return False
    elapsed = time.time() - order.placed_at
    steps = list(fo.Status)
    current = steps.index(fo.status_at(elapsed))
    for index, step in enumerate(steps):
        mark = ">" if index == current else ("x" if index < current else " ")
        print(f"  [{mark}] {step.value}")
    print(f"{elapsed:.0f} seconds after ordering.")
    if current == len(steps) - 1:
        print("Delivered. Thank you for ordering! The order is closed, you can start a new one.")
        return True
    return False


def print_summary(order: fo.Order) -> None:
    state = "order placed" if order.placed else "not placed yet"
    print(f"Dishes: {fo.item_count(order)} | Total: {money(fo.total_cents(order))} | {state}")


def main() -> None:
    order = fo.Order()
    print("=== Food Ordering ===")
    while True:
        print()
        print_summary(order)
        print()
        print("1. Browse the menu by category")
        print("2. Add a dish to the order")
        print("3. Change a quantity or remove a dish")
        print("4. Show my order")
        print("5. Delivery details")
        print("6. Payment method")
        print("7. Place the order")
        print("8. Track order status")
        print("0. Exit")
        choice = ask_number("Choose an option: ", 0, 8)
        print()
        if choice == 0:
            print("Goodbye!")
            return
        if choice == 1:
            browse_menu()
        elif choice == 2:
            add_dish(order)
        elif choice == 3:
            change_quantity(order)
        elif choice == 4:
            show_order(order)
        elif choice == 5:
            delivery_details(order)
        elif choice == 6:
            payment_method(order)
        elif choice == 7:
            place_order(order)
        elif choice == 8 and track_order(order):
            order = fo.Order()


if __name__ == "__main__":
    main()
