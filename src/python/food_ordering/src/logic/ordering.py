"""
Food ordering system logic.
"""
from typing import Optional, List, Dict
from dataclasses import dataclass, field
from enum import Enum, auto


class FoodCategory(Enum):
    """Food category enumeration."""

    APPETIZER = auto()
    MAIN_COURSE = auto()
    DESSERT = auto()
    BEVERAGE = auto()
    OTHER = auto()


class PaymentMethod(Enum):
    """Payment method enumeration."""

    CASH = auto()
    CARD = auto()
    PAYPAL = auto()


class OrderStatus(Enum):
    """Order status enumeration."""

    PENDING = auto()
    CONFIRMED = auto()
    PREPARING = auto()
    OUT_FOR_DELIVERY = auto()
    DELIVERED = auto()
    CANCELLED = auto()


@dataclass
class MenuItem:
    """Represents a menu item."""

    id: int
    name: str
    description: str
    price: float
    category: FoodCategory
    available: bool = True


@dataclass
class OrderItem:
    """Represents an item in an order."""

    menu_item_id: int
    quantity: int


@dataclass
class Order:
    """Represents a customer order."""

    id: int
    items: List[OrderItem] = field(default_factory=list)
    delivery_address: str = ""
    payment_method: PaymentMethod = PaymentMethod.CASH
    status: OrderStatus = OrderStatus.PENDING
    total: float = 0.0


class Menu:
    """
    Restaurant menu management.
    """

    def __init__(self) -> None:
        self.items: Dict[int, MenuItem] = {}
        self._next_id = 1

    def add_item(
        self,
        name: str,
        description: str,
        price: float,
        category: FoodCategory,
    ) -> int:
        """Add a menu item."""
        item_id = self._next_id
        self._next_id += 1
        self.items[item_id] = MenuItem(
            id=item_id,
            name=name,
            description=description,
            price=price,
            category=category,
        )
        return item_id

    def find_item(self, item_id: int) -> Optional[MenuItem]:
        """Find a menu item by ID."""
        return self.items.get(item_id)

    def display(self) -> str:
        """Get formatted menu display."""
        if not self.items:
            return "Menu is empty."

        lines = [f"{'ID':<4} {'Item':<25} {'Price':>8} {'Category':<15}"]
        lines.append("-" * 55)

        for item in sorted(self.items.values(), key=lambda x: (x.category.value, x.id)):
            status = "" if item.available else " (unavailable)"
            lines.append(
                f"{item.id:<4} {item.name:<25} ${item.price:>7.2f} "
                f"{self.category_to_string(item.category):<15}{status}"
            )
        return "\n".join(lines)

    def display_by_category(self, category: FoodCategory) -> str:
        """Display menu items by category."""
        items = [i for i in self.items.values() if i.category == category]
        if not items:
            return f"No items in {self.category_to_string(category)}."

        lines = [f"=== {self.category_to_string(category)} ==="]
        for item in items:
            lines.append(f"  {item.id}. {item.name} - ${item.price:.2f}")
            lines.append(f"     {item.description}")
        return "\n".join(lines)

    @staticmethod
    def category_to_string(category: FoodCategory) -> str:
        """Convert category to string."""
        return {
            FoodCategory.APPETIZER: "Appetizer",
            FoodCategory.MAIN_COURSE: "Main Course",
            FoodCategory.DESSERT: "Dessert",
            FoodCategory.BEVERAGE: "Beverage",
            FoodCategory.OTHER: "Other",
        }.get(category, "Unknown")


class OrderSystem:
    """
    Order management system.
    """

    def __init__(self, menu: Menu) -> None:
        self.menu = menu
        self.orders: Dict[int, Order] = {}
        self._next_order_id = 1

    def create_order(self) -> int:
        """Create a new order."""
        order_id = self._next_order_id
        self._next_order_id += 1
        self.orders[order_id] = Order(id=order_id)
        return order_id

    def add_item_to_order(
        self, order_id: int, menu_item_id: int, quantity: int
    ) -> bool:
        """Add an item to an order."""
        order = self.orders.get(order_id)
        if not order or order.status != OrderStatus.PENDING:
            return False

        menu_item = self.menu.find_item(menu_item_id)
        if not menu_item or not menu_item.available:
            return False

        # Check if already in order
        for item in order.items:
            if item.menu_item_id == menu_item_id:
                item.quantity += quantity
                return True

        order.items.append(OrderItem(menu_item_id=menu_item_id, quantity=quantity))
        return True

    def remove_item_from_order(self, order_id: int, menu_item_id: int) -> bool:
        """Remove an item from an order."""
        order = self.orders.get(order_id)
        if not order or order.status != OrderStatus.PENDING:
            return False

        for i, item in enumerate(order.items):
            if item.menu_item_id == menu_item_id:
                order.items.pop(i)
                return True
        return False

    def set_delivery_address(self, order_id: int, address: str) -> bool:
        """Set delivery address for an order."""
        order = self.orders.get(order_id)
        if not order or order.status != OrderStatus.PENDING:
            return False
        order.delivery_address = address
        return True

    def set_payment_method(self, order_id: int, method: PaymentMethod) -> bool:
        """Set payment method for an order."""
        order = self.orders.get(order_id)
        if not order or order.status != OrderStatus.PENDING:
            return False
        order.payment_method = method
        return True

    def calculate_total(self, order_id: int) -> float:
        """Calculate order total."""
        order = self.orders.get(order_id)
        if not order:
            return 0.0

        total = 0.0
        for item in order.items:
            menu_item = self.menu.find_item(item.menu_item_id)
            if menu_item:
                total += menu_item.price * item.quantity

        order.total = total
        return total

    def confirm_order(self, order_id: int) -> bool:
        """Confirm an order."""
        order = self.orders.get(order_id)
        if not order or order.status != OrderStatus.PENDING:
            return False
        if not order.items:
            return False
        if not order.delivery_address:
            return False

        self.calculate_total(order_id)
        order.status = OrderStatus.CONFIRMED
        return True

    def update_status(self, order_id: int, status: OrderStatus) -> bool:
        """Update order status."""
        order = self.orders.get(order_id)
        if not order:
            return False
        order.status = status
        return True

    def find_order(self, order_id: int) -> Optional[Order]:
        """Find an order by ID."""
        return self.orders.get(order_id)

    def display_order(self, order_id: int) -> str:
        """Get formatted order display."""
        order = self.orders.get(order_id)
        if not order:
            return "Order not found."

        lines = [f"Order #{order.id}"]
        lines.append(f"Status: {self.status_to_string(order.status)}")
        lines.append(f"Delivery: {order.delivery_address or 'Not set'}")
        lines.append(f"Payment: {self.payment_to_string(order.payment_method)}")
        lines.append("-" * 40)

        for item in order.items:
            menu_item = self.menu.find_item(item.menu_item_id)
            if menu_item:
                subtotal = menu_item.price * item.quantity
                lines.append(
                    f"  {menu_item.name} x{item.quantity} - ${subtotal:.2f}"
                )

        lines.append("-" * 40)
        lines.append(f"Total: ${order.total:.2f}")
        return "\n".join(lines)

    @staticmethod
    def status_to_string(status: OrderStatus) -> str:
        """Convert status to string."""
        return {
            OrderStatus.PENDING: "Pending",
            OrderStatus.CONFIRMED: "Confirmed",
            OrderStatus.PREPARING: "Preparing",
            OrderStatus.OUT_FOR_DELIVERY: "Out for Delivery",
            OrderStatus.DELIVERED: "Delivered",
            OrderStatus.CANCELLED: "Cancelled",
        }.get(status, "Unknown")

    @staticmethod
    def payment_to_string(method: PaymentMethod) -> str:
        """Convert payment method to string."""
        return {
            PaymentMethod.CASH: "Cash",
            PaymentMethod.CARD: "Card",
            PaymentMethod.PAYPAL: "PayPal",
        }.get(method, "Unknown")
