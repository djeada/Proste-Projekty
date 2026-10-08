"""Tests of the food ordering rules."""
import food_ordering as fo


def new_order_with_delivery() -> fo.Order:
    order = fo.Order()
    fo.add_dish(order, 3)
    fo.set_address(order, "Main Street 12")
    fo.set_phone(order, "123 456 789")
    fo.set_payment(order, fo.Payment.CASH)
    return order


def test_menu_has_ten_dishes_in_four_categories():
    assert len(fo.MENU) == 10
    assert {dish.category for dish in fo.MENU} == set(fo.Category)
    assert fo.dish_by_id(3).name == "Margherita Pizza"
    assert fo.dish_by_id(3).price_cents == 1200
    assert fo.dish_by_id(11) is None


def test_add_change_and_remove_dishes():
    order = fo.Order()
    assert fo.add_dish(order, 1, 2)
    assert fo.add_dish(order, 1)
    assert order.lines == {1: 3}
    assert fo.set_quantity(order, 1, 1)
    assert order.lines == {1: 1}
    assert fo.set_quantity(order, 1, 0)
    assert order.lines == {}


def test_quantity_limits_and_unknown_dishes():
    order = fo.Order()
    assert not fo.add_dish(order, 99)
    assert not fo.add_dish(order, 1, 0)
    assert not fo.set_quantity(order, 1, fo.MAX_QUANTITY + 1)
    assert not fo.set_quantity(order, 1, -1)
    assert fo.set_quantity(order, 1, fo.MAX_QUANTITY)
    assert not fo.add_dish(order, 1)
    assert order.lines == {1: fo.MAX_QUANTITY}


def test_total_is_in_integer_cents():
    order = fo.Order()
    assert fo.total_cents(order) == 0
    fo.add_dish(order, 9, 2)  # 2 x 2.90
    fo.add_dish(order, 6)  # 1 x 18.90
    assert fo.total_cents(order) == 2 * 290 + 1890
    assert fo.item_count(order) == 3


def test_address_validation():
    assert fo.validate_address("  Main Street 12  ") == "Main Street 12"
    assert fo.validate_address("Main St") is None  # no house number
    assert fo.validate_address("12345") is None  # no street name
    assert fo.validate_address("1 A") is None  # too short
    assert fo.validate_address("A" * 101 + " 1") is None  # too long


def test_phone_validation():
    assert fo.validate_phone("+48 (123) 456-789") == "+48123456789"
    assert fo.validate_phone("123456789") == "123456789"
    assert fo.validate_phone("12345") is None  # too short
    assert fo.validate_phone("1234567890123456") is None  # 16 digits
    assert fo.validate_phone("123abc4567") is None
    assert fo.validate_phone("12+3456789") is None  # '+' only at the start


def test_luhn_check_accepts_valid_numbers():
    assert fo.luhn_is_valid("4111 1111 1111 1111")
    assert fo.luhn_is_valid("4539-1488-0343-6467")
    assert fo.luhn_is_valid("378282246310005")


def test_luhn_check_rejects_invalid_numbers():
    assert not fo.luhn_is_valid("4111 1111 1111 1112")  # one digit changed
    assert not fo.luhn_is_valid("1234")  # too short
    assert not fo.luhn_is_valid("4111 1111 1111 abcd")  # not digits


def test_card_payment_keeps_only_last_four_digits():
    order = fo.Order()
    assert not fo.set_card(order, "4111 1111 1111 1112")
    assert order.payment is None
    assert fo.set_card(order, "4111 1111 1111 1111")
    assert order.payment is fo.Payment.CARD
    assert order.card_last4 == "1111"
    assert fo.set_payment(order, fo.Payment.PAYPAL)
    assert order.card_last4 == ""
    assert not fo.set_payment(order, fo.Payment.CARD)


def test_place_order_needs_every_part():
    order = fo.Order()
    assert fo.place_order(order, 100.0) is not None  # empty
    fo.add_dish(order, 3)
    assert fo.place_order(order, 100.0) is not None  # no address
    fo.set_address(order, "Main Street 12")
    assert fo.place_order(order, 100.0) is not None  # no phone
    fo.set_phone(order, "123456789")
    assert fo.place_order(order, 100.0) is not None  # no payment
    fo.set_payment(order, fo.Payment.CASH)
    assert fo.place_order(order, 100.0) is None
    assert order.placed_at == 100.0


def test_placed_order_cannot_change():
    order = new_order_with_delivery()
    fo.place_order(order, 0.0)
    assert fo.place_order(order, 5.0) is not None  # placed twice
    assert not fo.add_dish(order, 1)
    assert not fo.set_address(order, "Other Road 5")
    assert not fo.set_payment(order, fo.Payment.PAYPAL)


def test_status_follows_elapsed_time():
    step = fo.STATUS_STEP_SECONDS
    assert fo.status_at(0) is fo.Status.RECEIVED
    assert fo.status_at(step - 0.5) is fo.Status.RECEIVED
    assert fo.status_at(step) is fo.Status.PREPARING
    assert fo.status_at(2 * step) is fo.Status.ON_THE_WAY
    assert fo.status_at(3 * step) is fo.Status.DELIVERED
    assert fo.status_at(1000) is fo.Status.DELIVERED
    assert fo.status_at(-5) is fo.Status.RECEIVED
