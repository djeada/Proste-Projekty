import pytest

from shopping_cart import CATALOG, MAX_QUANTITY, Cart, CartError, validate_address, validate_name


def test_catalog_has_eight_products():
    assert len(CATALOG) == 8
    assert CATALOG[0].price_cents == 349
    assert CATALOG[4].price_cents == 2499


def test_add_and_subtotal():
    cart = Cart()
    assert cart.is_empty()
    cart.add(0, 2)
    cart.add(2, 1)
    assert cart.subtotal() == 2 * 349 + 875
    assert not cart.is_empty()


def test_add_rejects_bad_input():
    cart = Cart()
    with pytest.raises(CartError):
        cart.add(-1, 1)
    with pytest.raises(CartError):
        cart.add(len(CATALOG), 1)
    with pytest.raises(CartError):
        cart.add(0, 0)
    cart.add(0, MAX_QUANTITY)
    with pytest.raises(CartError):
        cart.add(0, 1)
    assert cart.quantities[0] == MAX_QUANTITY


def test_set_quantity_zero_removes():
    cart = Cart()
    cart.add(1, 3)
    cart.set_quantity(1, 0)
    assert cart.is_empty()
    with pytest.raises(CartError):
        cart.set_quantity(1, 100)


def test_percentage_discount():
    cart = Cart()
    cart.add(4, 1)  # 2499
    cart.add(7, 1)  # 999
    cart.apply_code("save10")
    assert cart.subtotal() == 3498
    assert cart.discount_cents() == 350
    assert cart.total() == 3148


def test_percentage_rounds_to_cent():
    cart = Cart()
    cart.add(0, 1)  # 349 cents: 10% is 34.9, rounds to 35
    cart.apply_code("SAVE10")
    assert cart.discount_cents() == 35
    assert cart.total() == 314


def test_fixed_discount_needs_minimum():
    cart = Cart()
    cart.add(0, 8)  # 2792: below the 30.00 minimum
    with pytest.raises(CartError):
        cart.apply_code("FLAT5")
    assert cart.discount is None

    cart.add(4, 1)  # 5291
    cart.apply_code("FLAT5")
    assert cart.discount_cents() == 500
    assert cart.total() == 5291 - 500


def test_discount_disappears_below_minimum():
    cart = Cart()
    cart.add(4, 2)  # 4998
    cart.apply_code("FLAT5")
    assert cart.discount_cents() == 500
    cart.set_quantity(4, 0)
    assert cart.discount_cents() == 0
    assert cart.total() == 0


def test_unknown_code_is_rejected():
    cart = Cart()
    cart.add(4, 2)
    for text in ("FREE", "SAVE1", ""):
        with pytest.raises(CartError):
            cart.apply_code(text)
    assert cart.discount is None


def test_clear_resets_cart_and_code():
    cart = Cart()
    cart.add(4, 2)
    cart.apply_code("FLAT5")
    cart.clear()
    assert cart.is_empty()
    assert cart.discount is None
    assert cart.total() == 0


@pytest.mark.parametrize("name", ["", "A", "1234"])
def test_invalid_names(name):
    with pytest.raises(CartError):
        validate_name(name)


def test_valid_names():
    validate_name("Anna")
    validate_name("Jan Kowalski")


@pytest.mark.parametrize("address", ["Main St", "12"])
def test_invalid_addresses(address):
    with pytest.raises(CartError):
        validate_address(address)


def test_valid_address():
    validate_address("10 Main Street")
