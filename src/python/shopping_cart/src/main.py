"""Terminal menu for the shopping cart."""

from shopping_cart import CATALOG, Cart, CartError, validate_address, validate_name


def money(cents):
    sign = "-" if cents < 0 else ""
    return f"{sign}${abs(cents) // 100}.{abs(cents) % 100:02d}"


def ask(prompt):
    """Returns the answer without surrounding spaces. Raises EOFError at end of input."""
    return input(prompt).strip()


def ask_product():
    """Returns the 0-based product index, or None if the answer is not a catalog number."""
    answer = ask(f"Product number (1-{len(CATALOG)}): ")
    if answer.isdigit() and 1 <= int(answer) <= len(CATALOG):
        return int(answer) - 1
    print("There is no product with that number.")
    return None


def print_cart_lines(cart):
    print(f"{'Product':<16} {'Qty':>4} {'Price':>10} {'Line':>10}")
    for product, quantity in zip(CATALOG, cart.quantities):
        if quantity:
            line = f"{product.name:<16} {quantity:>4} {money(product.price_cents):>10}"
            print(f"{line} {money(quantity * product.price_cents):>10}")


def print_totals(cart):
    print(f"{'Subtotal':<32} {money(cart.subtotal()):>10}")
    if cart.discount is not None:
        print(f"{'Discount (' + cart.discount.code + ')':<32} {money(-cart.discount_cents()):>10}")
    print(f"{'Total':<32} {money(cart.total()):>10}")


def show_catalog():
    print(f"\n{'#':<3} {'Product':<16} {'Category':<12} {'Price':>10}")
    for number, product in enumerate(CATALOG, start=1):
        print(f"{number:<3} {product.name:<16} {product.category:<12} {money(product.price_cents):>10}")


def show_cart(cart):
    if cart.is_empty():
        print("\nYour cart is empty.")
        return
    print()
    print_cart_lines(cart)
    print()
    print_totals(cart)


def ask_number(prompt, blank=None):
    """Returns the number typed, the blank value for an empty answer, or None if the answer is not a number."""
    answer = ask(prompt)
    if answer == "" and blank is not None:
        return blank
    if answer.isdigit():
        return int(answer)
    print("Please type a number.")
    return None


def add_product(cart):
    product = ask_product()
    quantity = ask_number("Quantity [1]: ", blank=1) if product is not None else None
    if quantity is None:
        return
    try:
        cart.add(product, quantity)
    except CartError as error:
        print(error)


def change_quantity(cart):
    product = ask_product()
    quantity = ask_number("New quantity (0 removes it): ") if product is not None else None
    if quantity is None:
        return
    try:
        cart.set_quantity(product, quantity)
    except CartError as error:
        print(error)


def remove_product(cart):
    product = ask_product()
    if product is not None:
        cart.set_quantity(product, 0)


def apply_code(cart):
    try:
        cart.apply_code(ask("Discount code (try SAVE10 or FLAT5): "))
        print("Discount applied.")
    except CartError as error:
        print(error)


def ask_valid(prompt, validate):
    while True:
        answer = ask(prompt)
        try:
            validate(answer)
            return answer
        except CartError as error:
            print(error)


def checkout(cart):
    if cart.is_empty():
        print("\nYour cart is empty.")
        return
    name = ask_valid("Full name: ", validate_name)
    address = ask_valid("Delivery address: ", validate_address)
    print(f"\n=== Order summary ===\n{name}\n{address}\n")
    print_cart_lines(cart)
    print()
    print_totals(cart)
    print("\nThank you for your order!")
    cart.clear()


ACTIONS = {
    "1": lambda cart: show_catalog(),
    "2": add_product,
    "3": change_quantity,
    "4": remove_product,
    "5": show_cart,
    "6": apply_code,
    "7": checkout,
}

MENU = """
=== Shopping Cart ===
1. Show catalog
2. Add product to cart
3. Change quantity
4. Remove product
5. Show cart
6. Apply discount code
7. Checkout
0. Quit"""


def main():
    cart = Cart()
    try:
        while True:
            print(MENU)
            choice = ask("Choose an option: ")
            if choice == "0":
                break
            if choice in ACTIONS:
                ACTIONS[choice](cart)
            else:
                print("Unknown option.")
    except EOFError:
        print()


if __name__ == "__main__":
    main()
