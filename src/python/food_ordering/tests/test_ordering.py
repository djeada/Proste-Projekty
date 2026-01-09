import unittest
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../src/logic"))
)
from ordering import Menu, OrderSystem, FoodCategory, PaymentMethod, OrderStatus


class TestMenu(unittest.TestCase):
    def test_add_item(self):
        menu = Menu()
        item_id = menu.add_item("Pizza", "Delicious pizza", 12.99, FoodCategory.MAIN_COURSE)
        self.assertEqual(item_id, 1)

    def test_find_item(self):
        menu = Menu()
        item_id = menu.add_item("Pizza", "Delicious pizza", 12.99, FoodCategory.MAIN_COURSE)
        item = menu.find_item(item_id)
        self.assertIsNotNone(item)
        self.assertEqual(item.name, "Pizza")

    def test_find_nonexistent_item(self):
        menu = Menu()
        item = menu.find_item(999)
        self.assertIsNone(item)


class TestOrderSystem(unittest.TestCase):
    def setUp(self):
        self.menu = Menu()
        self.menu.add_item("Pizza", "Delicious pizza", 12.99, FoodCategory.MAIN_COURSE)
        self.menu.add_item("Soda", "Refreshing drink", 2.99, FoodCategory.BEVERAGE)
        self.order_system = OrderSystem(self.menu)

    def test_create_order(self):
        order_id = self.order_system.create_order()
        self.assertEqual(order_id, 1)

    def test_add_item_to_order(self):
        order_id = self.order_system.create_order()
        result = self.order_system.add_item_to_order(order_id, 1, 2)
        self.assertTrue(result)

    def test_set_delivery_address(self):
        order_id = self.order_system.create_order()
        result = self.order_system.set_delivery_address(order_id, "123 Main St")
        self.assertTrue(result)

    def test_calculate_total(self):
        order_id = self.order_system.create_order()
        self.order_system.add_item_to_order(order_id, 1, 2)  # 2x Pizza = 25.98
        self.order_system.add_item_to_order(order_id, 2, 1)  # 1x Soda = 2.99
        total = self.order_system.calculate_total(order_id)
        self.assertAlmostEqual(total, 28.97, places=2)

    def test_confirm_order(self):
        order_id = self.order_system.create_order()
        self.order_system.add_item_to_order(order_id, 1, 1)
        self.order_system.set_delivery_address(order_id, "123 Main St")
        result = self.order_system.confirm_order(order_id)
        self.assertTrue(result)
        order = self.order_system.find_order(order_id)
        self.assertEqual(order.status, OrderStatus.CONFIRMED)

    def test_confirm_order_without_items_fails(self):
        order_id = self.order_system.create_order()
        self.order_system.set_delivery_address(order_id, "123 Main St")
        result = self.order_system.confirm_order(order_id)
        self.assertFalse(result)

    def test_confirm_order_without_address_fails(self):
        order_id = self.order_system.create_order()
        self.order_system.add_item_to_order(order_id, 1, 1)
        result = self.order_system.confirm_order(order_id)
        self.assertFalse(result)


if __name__ == "__main__":
    unittest.main()
