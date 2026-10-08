"""Food ordering rules: menu, order, validation, card check and order status. No input or output here."""
from dataclasses import dataclass, field
from enum import Enum
from typing import Dict, List, Optional

MAX_QUANTITY = 20
STATUS_STEP_SECONDS = 3
DIGITS = "0123456789"


class Category(Enum):
    STARTER = "Starters"
    MAIN = "Main courses"
    DESSERT = "Desserts"
    DRINK = "Drinks"


class Payment(Enum):
    PAYPAL = "PayPal"
    CARD = "Credit card"
    CASH = "Cash on delivery"


class Status(Enum):
    RECEIVED = "Received"
    PREPARING = "Preparing"
    ON_THE_WAY = "On the way"
    DELIVERED = "Delivered"


@dataclass(frozen=True)
class Dish:
    id: int
    name: str
    description: str
    price_cents: int  # integer cents, so 9.90 is 990
    category: Category


MENU: List[Dish] = [
    Dish(1, "Garlic Bread", "Toasted baguette with garlic butter", 450, Category.STARTER),
    Dish(2, "Tomato Soup", "Slow-cooked tomato soup with basil", 520, Category.STARTER),
    Dish(3, "Margherita Pizza", "Tomato, mozzarella and fresh basil", 1200, Category.MAIN),
    Dish(4, "Chicken Pasta", "Penne with grilled chicken and pesto", 1350, Category.MAIN),
    Dish(5, "Beef Burger", "Beef patty, cheddar, pickles and fries", 1450, Category.MAIN),
    Dish(6, "Grilled Salmon", "Atlantic salmon with lemon and herbs", 1890, Category.MAIN),
    Dish(7, "Cheesecake", "Baked cheesecake with berry sauce", 620, Category.DESSERT),
    Dish(8, "Chocolate Cake", "Rich dark chocolate layer cake", 590, Category.DESSERT),
    Dish(9, "Coffee", "Freshly brewed coffee", 290, Category.DRINK),
    Dish(10, "Lemonade", "Homemade lemonade with mint", 350, Category.DRINK),
]


def dish_by_id(dish_id: int) -> Optional[Dish]:
    return next((dish for dish in MENU if dish.id == dish_id), None)


@dataclass
class Order:
    lines: Dict[int, int] = field(default_factory=dict)  # dish id -> quantity, in the order they were added
    address: str = ""
    phone: str = ""
    payment: Optional[Payment] = None
    card_last4: str = ""
    placed_at: Optional[float] = None

    @property
    def placed(self) -> bool:
        return self.placed_at is not None


def set_quantity(order: Order, dish_id: int, quantity: int) -> bool:
    """Set the quantity of a dish. A quantity of 0 removes the dish."""
    if order.placed or dish_by_id(dish_id) is None or not 0 <= quantity <= MAX_QUANTITY:
        return False
    if quantity == 0:
        order.lines.pop(dish_id, None)
    else:
        order.lines[dish_id] = quantity
    return True


def add_dish(order: Order, dish_id: int, quantity: int = 1) -> bool:
    if quantity < 1:
        return False
    return set_quantity(order, dish_id, order.lines.get(dish_id, 0) + quantity)


def item_count(order: Order) -> int:
    return sum(order.lines.values())


def total_cents(order: Order) -> int:
    return sum(dish_by_id(dish_id).price_cents * quantity for dish_id, quantity in order.lines.items())


def validate_address(text: str) -> Optional[str]:
    """Return the trimmed address, or None. A valid address has a street name (letters) and a house number (digits)."""
    clean = text.strip()
    if not 5 <= len(clean) <= 100:
        return None
    if not any(c in DIGITS for c in clean) or not any(c.isalpha() for c in clean):
        return None
    return clean


def set_address(order: Order, text: str) -> bool:
    clean = validate_address(text)
    if order.placed or clean is None:
        return False
    order.address = clean
    return True


def validate_phone(text: str) -> Optional[str]:
    """Return the phone number without spaces, dashes and brackets, or None. Allows a leading '+' and 9-15 digits."""
    stripped = "".join(c for c in text if not c.isspace() and c not in "-()")
    prefix = "+" if stripped.startswith("+") else ""
    digits = stripped[len(prefix):]
    if not 9 <= len(digits) <= 15 or any(c not in DIGITS for c in digits):
        return None
    return prefix + digits


def set_phone(order: Order, text: str) -> bool:
    clean = validate_phone(text)
    if order.placed or clean is None:
        return False
    order.phone = clean
    return True


def card_digits(text: str) -> Optional[str]:
    """Return the digits of a card number (spaces and dashes are ignored), or None if there are not 13-19 of them."""
    digits = "".join(c for c in text if c not in " -")
    if not 13 <= len(digits) <= 19 or any(c not in DIGITS for c in digits):
        return None
    return digits


def luhn_is_valid(number: str) -> bool:
    """Luhn check: from the right, double every second digit (subtract 9 if over 9) and add them all up.
    A real card number gives a total that ends in 0."""
    digits = card_digits(number)
    if digits is None:
        return False
    total = 0
    for index, char in enumerate(reversed(digits)):
        value = int(char)
        if index % 2 == 1:
            value *= 2
            if value > 9:
                value -= 9
        total += value
    return total % 10 == 0


def set_card(order: Order, number: str) -> bool:
    digits = card_digits(number)
    if order.placed or digits is None or not luhn_is_valid(number):
        return False
    order.payment = Payment.CARD
    order.card_last4 = digits[-4:]
    return True


def set_payment(order: Order, payment: Payment) -> bool:
    """Choose PayPal or cash. Cards go through set_card, which checks the number."""
    if order.placed or payment is Payment.CARD:
        return False
    order.payment = payment
    order.card_last4 = ""
    return True


def place_order(order: Order, now: float) -> Optional[str]:
    """Place the order. Returns None on success, otherwise the reason it cannot be placed."""
    if order.placed:
        return "The order has already been placed."
    if not order.lines:
        return "Your order is empty. Add a dish first."
    if not order.address:
        return "Please enter a delivery address."
    if not order.phone:
        return "Please enter a phone number."
    if order.payment is None:
        return "Please choose a payment method."
    order.placed_at = now
    return None


def status_at(elapsed_seconds: float) -> Status:
    """Each status lasts STATUS_STEP_SECONDS. Pure function of the time since the order was placed."""
    statuses = list(Status)
    index = int(elapsed_seconds // STATUS_STEP_SECONDS)
    return statuses[min(max(index, 0), len(statuses) - 1)]
