# 🍽️ Restaurant Billing System

A console-based restaurant management and billing application developed in **C++** using Object-Oriented Programming (OOP) concepts. The system simplifies restaurant operations by managing menu items, customer information, orders, billing, payments, and sales reports through an interactive menu-driven interface.

## 📌 Project Overview

The Restaurant Billing System is designed to handle basic restaurant activities efficiently. It allows users to manage food items, register customers, process orders, calculate bills with applicable taxes, and maintain records using file handling.

The project demonstrates practical implementation of C++ programming concepts in a real-world restaurant management scenario.

## ✨ Features

- **Menu Management:** Add, display, search, update, and delete menu items.
- **Customer Management:** Register customers and maintain customer information.
- **Order Management:** Select food items and specify quantities for transactions.
- **Automatic Billing:** Calculate item totals, subtotals, applicable taxes, and final bills.
- **Payment Handling:** Support cash and simulated UPI payment options, if enabled in the implementation.
- **Sales Reports:** Generate reports to review transaction and sales information.
- **File Handling:** Store and retrieve menu, customer, and order data using text files.
- **Interactive Console Interface:** Navigate through a simple menu-driven system.
- **Data Persistence:** Retain saved records between program executions through file storage.

## 🛠️ Technologies Used

| Technology | Purpose |
|---|---|
| C++ | Core programming language |
| Object-Oriented Programming | Organizing the application into classes and objects |
| File Handling | Saving and loading application data |
| MinGW / GCC | Compiling the C++ source code |
| Visual Studio Code | Development and testing environment |

## 📂 Project Structure

```text
RestaurantBillingSystem/
│
├── RestaurantBillingSystem.cpp
├── RestaurantBillingSystem.exe
│
├── Data/
│   ├── menu.txt
│   ├── customers.txt
│   └── orders.txt
│
└── README.md
```

*Note: The file structure above is illustrative. Adjust the filenames and folders to match your actual project.*

## ⚙️ Installation and Setup

### Prerequisites

- Visual Studio Code or another C++ editor.
- MinGW-w64 or GCC installed on your system.
- A terminal such as PowerShell or the VS Code integrated terminal.

### Step 1: Clone the Repository

```bash
git clone <your-repository-url>
```

Alternatively, download the project files and extract them into a folder.

### Step 2: Open the Project

Open the project folder in Visual Studio Code.

### Step 3: Compile the Program

Open the terminal in the project directory and run:

```bash
g++ RestaurantBillingSystem.cpp -o RestaurantBillingSystem.exe
```

If your project contains multiple `.cpp` source files, include all required source files in the compilation command.

### Step 4: Run the Application

In Windows PowerShell, execute:

```powershell
.\RestaurantBillingSystem.exe
```

The main menu should appear in the terminal.

## 🖥️ Main Menu

The application provides the following operations:

| Option | Function |
|---|---|
| 1 | Add Menu Item |
| 2 | Display Menu Items |
| 3 | Search Menu Item |
| 4 | Update Menu Item |
| 5 | Delete Menu Item |
| 6 | Main Transaction |
| 7 | Report |
| 8 | Save / Load Data |
| 9 | Exit |

## 🧾 Billing Example

Suppose a customer orders the following items:

| Item | Quantity | Price | Total |
|---|---:|---:|---:|
| Burger | 2 | ₹120 | ₹240 |
| Pizza | 1 | ₹260 | ₹260 |
| **Subtotal** | | | **₹500** |
| Tax (5%) | | | ₹25 |
| **Grand Total** | | | **₹525** |

*This is an illustrative example assuming a 5% tax rate. Actual results depend on the application's configuration.*

## 💾 Data Storage

The application uses file handling to save and retrieve information.

- `menu.txt` — Stores menu item information.
- `customers.txt` — Stores registered customer information.
- `orders.txt` — Stores order or transaction records.

This approach helps preserve records after the application is closed, provided the save operations complete successfully.

## 🧠 Concepts Demonstrated

This project provides practical experience with:

- Classes and objects.
- Encapsulation and data management.
- Inheritance and polymorphism, where implemented.
- Functions and conditional statements.
- Loops and menu-driven programming.
- File input/output operations.
- Data validation and exception or error handling, where implemented.
- Basic billing and arithmetic calculations.

## 🎯 Learning Outcomes

By developing this project, I gained practical experience in designing a menu-driven C++ application, organizing program functionality, managing customer and menu records, processing transactions, calculating bills, and storing data in files.

The project also helped me understand how programming concepts can be applied to automate everyday business operations.

## 🚀 Future Enhancements

Potential improvements include:

- A graphical user interface.
- Database integration using MySQL or SQLite.
- Secure user authentication and role-based access.
- Digital payment gateway integration.
- Printable invoices and PDF receipts.
- Inventory and stock management.
- Improved sales analytics and reporting.

## 👨‍💻 Author

**Your Name**

- Course: Computer Science and Engineering (CSE)
- Project: Restaurant Billing System
- Language: C++

## 📄 License

This project is intended for educational and learning purposes. You may adapt and extend it for academic or personal use.
