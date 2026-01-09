# Shopping Cart in C

![Build Status](https://github.com/djeada/Proste-Projekty/actions/workflows/ci.yml/badge.svg)
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)

## Table of Contents
- [Project Overview](#project-overview)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Features](#features)
- [Testing](#testing)
- [Linting and Formatting](#linting-and-formatting)
- [Deployment](#deployment)
- [Project Structure](#project-structure)
- [Possible Improvements](#possible-improvements)
- [Contributing](#contributing)
- [License](#license)

## Project Overview
This is a simple console-based shopping cart application in C. It simulates an e-commerce experience with product browsing, cart management, discount codes, and checkout functionality.

## Requirements
- CMake >= 3.10
- C compiler (e.g., gcc, clang)
- clang-tidy, clang-format (optional, for code quality)
- Docker (optional, for containerized builds)

## Installation
Clone the repository and build the project:
```sh
git clone https://github.com/djeada/Proste-Projekty.git
cd Proste-Projekty/src/c/shopping_cart
cmake -S . -B build
cmake --build build
```

## Usage
Run the program:
```sh
./build/main
```

Follow the menu to browse products, add items to cart, and checkout.

### Discount Codes
- `SAVE10` - 10% off
- `SAVE20` - 20% off

## Features
- Browse available products with prices and stock
- Add products to shopping cart
- Update quantities or remove items
- Apply discount codes
- View cart with subtotals and discounts
- Checkout with stock validation

## Testing
Run unit tests using CTest:
```sh
cd build
ctest
```
Or run the test binary directly:
```sh
./build/test_shopping_cart
```

## Linting and Formatting
Check code quality and formatting:
```sh
clang-tidy src/*.c
clang-format -i src/*.c
```

## Deployment
Build and run the project in Docker:
```sh
docker build -t shopping_cart .
docker run -it shopping_cart
```

## Project Structure
```
shopping_cart/
├── src/
│   ├── main.c
│   ├── shopping_cart.c
│   └── shopping_cart.h
├── tests/
│   └── test_shopping_cart.c
├── CMakeLists.txt
├── Dockerfile
├── .clang-tidy
├── .clang-format
├── .editorconfig
├── .github/
│   └── workflows/
│       └── ci.yml
└── README.md
```

## Possible Improvements
- Add product categories and filtering
- Implement persistent storage (database/file)
- Add user accounts and order history
- Create web interface
- Integrate real payment processing

## Contributing
Contributions are welcome! Please open issues or pull requests for improvements, bug fixes, or new features.

## License
This project is licensed under the [MIT License](https://github.com/djeada/Proste-Projekty/blob/main/LICENSE).
