"""
Shopping cart logic.
"""
from typing import Optional, List, Dict
from dataclasses import dataclass


@dataclass
class Product:
    """Represents a product in the store."""

    id: int
    name: str
    price: float
    stock: int


@dataclass
class CartItem:
    """Represents an item in the cart."""

    product_id: int
    quantity: int


class Store:
    """
    Store with product catalog.
    """

    MAX_PRODUCTS = 100

    def __init__(self) -> None:
        self.products: Dict[int, Product] = {}
        self._next_id = 1

    def add_product(self, name: str, price: float, stock: int) -> int:
        """
        Add a product to the store.

        :param name: Product name
        :param price: Product price
        :param stock: Initial stock quantity
        :return: Product ID
        """
        if len(self.products) >= self.MAX_PRODUCTS:
            return 0

        product_id = self._next_id
        self._next_id += 1
        self.products[product_id] = Product(
            id=product_id, name=name, price=price, stock=stock
        )
        return product_id

    def find_product(self, product_id: int) -> Optional[Product]:
        """Find a product by ID."""
        return self.products.get(product_id)

    def list_products(self) -> str:
        """Get formatted list of all products."""
        if not self.products:
            return "No products available."

        lines = [f"{'ID':<4} {'Product':<30} {'Price':>10} {'Stock':>8}"]
        lines.append("-" * 54)
        for p in self.products.values():
            lines.append(f"{p.id:<4} {p.name:<30} {p.price:>10.2f} {p.stock:>8}")
        return "\n".join(lines)


class Cart:
    """
    Shopping cart.
    """

    MAX_CART_ITEMS = 50

    def __init__(self, store: Store) -> None:
        self.store = store
        self.items: List[CartItem] = []
        self.discount_percent: float = 0.0

    def add_item(self, product_id: int, quantity: int) -> bool:
        """
        Add an item to the cart.

        :param product_id: Product ID
        :param quantity: Quantity to add
        :return: True if successful
        """
        if quantity <= 0:
            return False

        product = self.store.find_product(product_id)
        if not product or product.stock < quantity:
            return False

        # Check if already in cart
        for item in self.items:
            if item.product_id == product_id:
                if product.stock < item.quantity + quantity:
                    return False
                item.quantity += quantity
                return True

        if len(self.items) >= self.MAX_CART_ITEMS:
            return False

        self.items.append(CartItem(product_id=product_id, quantity=quantity))
        return True

    def remove_item(self, product_id: int) -> bool:
        """Remove an item from the cart."""
        for i, item in enumerate(self.items):
            if item.product_id == product_id:
                self.items.pop(i)
                return True
        return False

    def update_quantity(self, product_id: int, quantity: int) -> bool:
        """Update quantity of an item."""
        if quantity <= 0:
            return self.remove_item(product_id)

        product = self.store.find_product(product_id)
        if not product or product.stock < quantity:
            return False

        for item in self.items:
            if item.product_id == product_id:
                item.quantity = quantity
                return True
        return False

    def get_total(self) -> float:
        """Calculate cart total with discount."""
        total = 0.0
        for item in self.items:
            product = self.store.find_product(item.product_id)
            if product:
                total += product.price * item.quantity

        discount = total * self.discount_percent / 100.0
        return total - discount

    def apply_discount(self, percent: float) -> None:
        """Apply a discount percentage."""
        if 0 <= percent <= 100:
            self.discount_percent = percent

    def print_cart(self) -> str:
        """Get formatted cart contents."""
        if not self.items:
            return "Cart is empty."

        lines = [f"{'Product':<30} {'Price':>10} {'Qty':>8} {'Subtotal':>12}"]
        lines.append("-" * 62)

        total = 0.0
        for item in self.items:
            product = self.store.find_product(item.product_id)
            if product:
                subtotal = product.price * item.quantity
                lines.append(
                    f"{product.name:<30} {product.price:>10.2f} "
                    f"{item.quantity:>8} {subtotal:>12.2f}"
                )
                total += subtotal

        lines.append("-" * 62)
        lines.append(f"{'Subtotal:':>50} {total:>12.2f}")

        if self.discount_percent > 0:
            discount = total * self.discount_percent / 100.0
            lines.append(f"{'Discount:':>50} {-discount:>12.2f}")
            lines.append(f"{'Total:':>50} {total - discount:>12.2f}")

        return "\n".join(lines)

    def checkout(self) -> bool:
        """Complete the purchase."""
        # Verify stock
        for item in self.items:
            product = self.store.find_product(item.product_id)
            if not product or product.stock < item.quantity:
                return False

        # Deduct stock
        for item in self.items:
            product = self.store.find_product(item.product_id)
            if product:
                product.stock -= item.quantity

        # Clear cart
        self.items.clear()
        self.discount_percent = 0.0
        return True

    def clear(self) -> None:
        """Clear the cart."""
        self.items.clear()
        self.discount_percent = 0.0
