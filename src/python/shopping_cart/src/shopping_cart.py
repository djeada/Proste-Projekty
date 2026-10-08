"""Shopping cart rules: catalog, quantities, discount codes, totals and validation. No I/O here."""

from dataclasses import dataclass
from typing import Optional

MAX_QUANTITY = 99


@dataclass(frozen=True)
class Product:
    name: str
    category: str
    price_cents: int  # money is kept in integer cents: 0.10 cannot be stored exactly as a float


@dataclass(frozen=True)
class DiscountCode:
    code: str
    percent: int = 0  # percentage discount, or 0
    amount_cents: int = 0  # fixed discount, used when percent is 0
    min_order_cents: int = 0


CATALOG = [
    Product("Notebook", "Stationery", 349),
    Product("Pen set", "Stationery", 1290),
    Product("Coffee mug", "Kitchen", 875),
    Product("Water bottle", "Kitchen", 1120),
    Product("Desk lamp", "Home", 2499),
    Product("T-shirt", "Clothing", 1500),
    Product("Running socks", "Clothing", 650),
    Product("Paperback novel", "Books", 999),
]

DISCOUNT_CODES = [
    DiscountCode("SAVE10", percent=10),
    DiscountCode("FLAT5", amount_cents=500, min_order_cents=3000),
]


class CartError(ValueError):
    """Raised when a cart operation or a form field is not allowed."""


class Cart:
    def __init__(self) -> None:
        # quantities[i] is the number of CATALOG[i] items in the cart
        self.quantities = [0] * len(CATALOG)
        self.discount: Optional[DiscountCode] = None

    def add(self, product: int, quantity: int) -> None:
        self._check_product(product)
        if quantity < 1 or self.quantities[product] + quantity > MAX_QUANTITY:
            raise CartError("Quantity is out of range: a product can be in the cart at most 99 times.")
        self.quantities[product] += quantity

    def set_quantity(self, product: int, quantity: int) -> None:
        self._check_product(product)
        if not 0 <= quantity <= MAX_QUANTITY:
            raise CartError("Quantity must be from 0 to 99.")
        self.quantities[product] = quantity

    def apply_code(self, text: str) -> None:
        for candidate in DISCOUNT_CODES:
            if candidate.code == text.upper():
                if self.subtotal() < candidate.min_order_cents:
                    raise CartError("This code needs a higher order value.")
                self.discount = candidate
                return
        raise CartError("Unknown discount code.")

    def clear(self) -> None:
        self.quantities = [0] * len(CATALOG)
        self.discount = None

    def is_empty(self) -> bool:
        return all(quantity == 0 for quantity in self.quantities)

    def subtotal(self) -> int:
        return sum(product.price_cents * quantity for product, quantity in zip(CATALOG, self.quantities))

    def discount_cents(self) -> int:
        code = self.discount
        subtotal = self.subtotal()
        if code is None or subtotal < code.min_order_cents:
            return 0
        if code.percent > 0:
            return (subtotal * code.percent + 50) // 100  # rounded to the nearest cent
        return min(code.amount_cents, subtotal)

    def total(self) -> int:
        return self.subtotal() - self.discount_cents()

    @staticmethod
    def _check_product(product: int) -> None:
        if not 0 <= product < len(CATALOG):
            raise CartError("There is no product with that number.")


def validate_name(name: str) -> None:
    if not 2 <= len(name) <= 40 or not any(char.isalpha() for char in name):
        raise CartError("Name must be 2-40 characters and contain a letter.")


def validate_address(address: str) -> None:
    if not 5 <= len(address) <= 80 or not any(char.isdigit() for char in address):
        raise CartError("Address must be 5-80 characters and contain a house number (a digit).")
