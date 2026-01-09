# Food Ordering System in C

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
This is a console-based food ordering system in C. Users can browse a menu of available dishes organized by categories, add items to their order, specify delivery address, select a payment method, and track order status.

## Requirements
- CMake >= 3.10
- C compiler (e.g., gcc, clang)
- clang-tidy, clang-format (optional, for code quality)
- Docker (optional, for containerized builds)

## Installation
Clone the repository and build the project:
```sh
git clone https://github.com/djeada/Proste-Projekty.git
cd Proste-Projekty/src/c/food_ordering
cmake -S . -B build
cmake --build build
```

## Usage
Run the program:
```sh
./build/main
```

### Menu Options
1. View Menu - Display all available dishes
2. View Menu by Category - Filter by Appetizer, Main Course, Dessert, or Beverage
3. Create New Order - Start a new order
4. Add Item to Order - Add menu items by ID
5. Remove Item from Order - Remove items from current order
6. Set Delivery Address - Enter your delivery address
7. Set Payment Method - Choose Cash, Card, or PayPal
8. View Order - See current order summary with total
9. Confirm Order - Submit order for preparation
10. Track Order Status - Check status of any order
11. Cancel Order - Cancel current order
0. Exit - Close the application

### Sample Menu
- **Appetizers**: Spring Rolls, Garlic Bread, Soup of the Day
- **Main Courses**: Grilled Salmon, Beef Steak, Chicken Pasta, Pizza
- **Desserts**: Chocolate Cake, Ice Cream, Cheesecake
- **Beverages**: Coffee, Soft Drinks, Fresh Juice, Water

## Features
- Browse menu organized by food categories
- Create and manage orders
- Add/remove items with quantities
- Calculate order totals
- Multiple payment methods (Cash, Card, PayPal)
- Delivery address management
- Order status tracking (Pending, Confirmed, Preparing, Out for Delivery, Delivered)
- Order cancellation

## Testing
Run unit tests using CTest:
```sh
cd build
ctest
```
Or run the test binary directly:
```sh
./build/test_food_ordering
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
docker build -t food_ordering .
docker run -it food_ordering
```

## Project Structure
```
food_ordering/
├── src/
│   ├── main.c
│   ├── food_ordering.c
│   └── food_ordering.h
├── tests/
│   └── test_food_ordering.c
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
- Add persistent storage (database/file)
- Implement user accounts and order history
- Add special offers and discount codes
- Real-time order tracking simulation
- Integration with payment APIs
- Web interface
- Restaurant management features

## Contributing
Contributions are welcome! Please open issues or pull requests for improvements, bug fixes, or new features.

## License
This project is licensed under the [MIT License](https://github.com/djeada/Proste-Projekty/blob/main/LICENSE).
