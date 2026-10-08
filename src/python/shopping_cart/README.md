# Shopping Cart (Python)

A terminal shopping cart for a small online shop. You browse an 8-product catalog, put items in the cart, apply a discount code, and check out with a name and a delivery address. The Polish name of the project is *Koszyk z zakupami*.

![Screenshot](screenshot.png)

## Features

- A built-in catalog of 8 products with a name, a category and a price
- Add a product, change its quantity (0 removes it), or remove it
- The cart shows every line with its line total, the subtotal, the discount and the total
- Two discount codes: `SAVE10` takes 10% off any order, `FLAT5` takes 5.00 off orders of 30.00 or more
- Codes are not case-sensitive; an unknown code, or a code whose minimum is not reached, is rejected
- Checkout checks the name and the address, prints an order summary and empties the cart
- Money is stored as whole cents, so the totals are exact

## How to use

The program shows a numbered menu. Type the number and press Enter:

```
=== Shopping Cart ===
1. Show catalog
2. Add product to cart
3. Change quantity
4. Remove product
5. Show cart
6. Apply discount code
7. Checkout
0. Quit
```

A product is chosen by its catalog number (1-8). When you add a product, an empty quantity means 1. Press Ctrl+D to quit at any prompt.

Example session (three notebooks with the `SAVE10` code, menus left out):

```
Product number (1-8): 1
Quantity [1]: 3
Discount code (try SAVE10 or FLAT5): SAVE10
Discount applied.

Product           Qty      Price       Line
Notebook            3      $3.49     $10.47

Subtotal                             $10.47
Discount (SAVE10)                    -$1.05
Total                                 $9.42
```

The screenshot shows a larger cart with four products and the same code.

## How it works

**Data.** `CATALOG` is a list of `Product` dataclasses (name, category, price). `DISCOUNT_CODES` is a list of `DiscountCode` dataclasses: a percentage, or a fixed amount with a minimum order value. A `Cart` object keeps `quantities`, one number per catalog product, and `discount`, the applied code or `None`. A cart never holds more than 99 of one product.

**Money in cents.** Every price and total is an `int` that counts cents: `349` means $3.49. Binary floating point cannot store most decimal fractions exactly. For example, `0.1 + 0.2` gives `0.30000000000000004` in Python, and summing many prices as floats gives small errors that can show up on a receipt. Integers are exact, and `money()` in `main.py` only turns the number into text for printing.

**Rules** (`src/shopping_cart.py`):

1. `Cart.subtotal()` adds `price_cents * quantity` for each product.
2. A percentage discount is `(subtotal * percent + 50) // 100`. The `+ 50` rounds half up to the nearest cent, so 10% of $3.49 is 35 cents.
3. A fixed discount is the amount, but never more than the subtotal. It is applied only when the subtotal reaches the code's minimum. If the cart later drops below the minimum, `discount_cents()` returns 0.
4. `Cart.total()` is the subtotal minus the discount.
5. `validate_name` and `validate_address` raise `CartError` when a field is invalid. Names need 2-40 characters and a letter; addresses need 5-80 characters and a digit (the house number).

The logic never prints or reads input; it raises `CartError` with a message that the interface shows.

**The user interface loop** (`src/main.py`). `main()` prints the menu and reads the choice with `input()`. A dictionary `ACTIONS` maps each choice to a function that changes the cart. `ask_valid` asks for the name and the address again until each one passes its check; then `checkout` prints the summary and calls `cart.clear()`. Ctrl+D raises `EOFError`, which `main()` catches to end the program cleanly.

## Project layout

```
README.md                      this file
screenshot.png                 the terminal screenshot above
requirements.txt               pytest (the only dependency, used for tests)
pyproject.toml                 tells pytest where the source files are
.flake8                        the line length limit for flake8
.editorconfig                  editor settings (indentation, line endings)
src/shopping_cart.py           the rules: catalog, Cart class, discount codes, validation
src/main.py                    the terminal menu: reading input and printing tables
tests/test_shopping_cart.py    tests of the rules
```

## Requirements

- Python 3.8 or newer (standard library only for the program)
- pytest, only to run the tests

## Run

```sh
python3 src/main.py
```

## Test

```sh
pip install -r requirements.txt
pytest
```

The tests check the catalog, adding and removing items, the quantity limit, both kinds of discount (including rounding and the minimum order), unknown codes, clearing the cart, and the name and address rules. They do not need a terminal.

## Comparison with the other versions

- [C](../../c/shopping_cart)
- [JavaScript](../../vanilla_js/shopping_cart)

| | C | Python | JavaScript |
|---|---|---|---|
| Interface | terminal menu | terminal menu | browser page |
| Lines of logic | 163 | 81 | 90 |
| Lines of interface | 265 | 130 | 205 |
| Tests | 12 | 17 | 12 |

Python keeps the cart as a list and a small class, and it reports mistakes with exceptions, so the rules read almost like the description. Integers never overflow in Python, so the cents logic needs no size checks, and the `//` operator makes the rounding explicit. C needs a fixed array, manual string handling and an error code returned from each function. Both languages keep the same rules and the same numbers, which the tests check.

## Ideas for extensions

- Save the cart to a JSON file, so it survives restarting the program
- Add a stock count per product and refuse orders that exceed it
- Support several discount codes at once, with a rule for which one wins
- Add shipping costs that depend on the order value
- Print a numbered receipt with a date and an order id
