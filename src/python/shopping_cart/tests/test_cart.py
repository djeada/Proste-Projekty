import unittest
import sys
import os

sys.path.insert(
    0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../src/logic"))
)
from cart import Store, Cart


class TestStore(unittest.TestCase):
    def test_add_product(self):
        store = Store()
        product_id = store.add_product("Apple", 1.00, 10)
        self.assertEqual(product_id, 1)

    def test_find_product(self):
        store = Store()
        product_id = store.add_product("Apple", 1.00, 10)
        product = store.find_product(product_id)
        self.assertIsNotNone(product)
        self.assertEqual(product.name, "Apple")

    def test_find_nonexistent_product(self):
        store = Store()
        product = store.find_product(999)
        self.assertIsNone(product)


class TestCart(unittest.TestCase):
    def test_add_item(self):
        store = Store()
        product_id = store.add_product("Apple", 1.00, 10)
        cart = Cart(store)
        result = cart.add_item(product_id, 5)
        self.assertTrue(result)
        self.assertEqual(len(cart.items), 1)

    def test_add_item_exceeds_stock(self):
        store = Store()
        product_id = store.add_product("Apple", 1.00, 5)
        cart = Cart(store)
        result = cart.add_item(product_id, 10)
        self.assertFalse(result)

    def test_remove_item(self):
        store = Store()
        product_id = store.add_product("Apple", 1.00, 10)
        cart = Cart(store)
        cart.add_item(product_id, 5)
        result = cart.remove_item(product_id)
        self.assertTrue(result)
        self.assertEqual(len(cart.items), 0)

    def test_get_total(self):
        store = Store()
        p1 = store.add_product("Apple", 1.00, 10)
        p2 = store.add_product("Banana", 0.50, 10)
        cart = Cart(store)
        cart.add_item(p1, 2)  # 2.00
        cart.add_item(p2, 3)  # 1.50
        self.assertEqual(cart.get_total(), 3.50)

    def test_apply_discount(self):
        store = Store()
        product_id = store.add_product("Apple", 10.00, 10)
        cart = Cart(store)
        cart.add_item(product_id, 1)
        cart.apply_discount(10)  # 10%
        self.assertEqual(cart.get_total(), 9.00)

    def test_checkout(self):
        store = Store()
        product_id = store.add_product("Apple", 1.00, 10)
        cart = Cart(store)
        cart.add_item(product_id, 5)
        result = cart.checkout()
        self.assertTrue(result)
        self.assertEqual(len(cart.items), 0)
        product = store.find_product(product_id)
        self.assertEqual(product.stock, 5)


if __name__ == "__main__":
    unittest.main()
