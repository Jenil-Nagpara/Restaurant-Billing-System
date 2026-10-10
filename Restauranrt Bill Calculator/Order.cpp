
#include "Order.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <utility>

using namespace std;

// Default constructor
Order::Order()
    : orderID(0),
      customerID(0),
      orderedItems(nullptr),
      quantities(nullptr),
      itemCount(0),
      capacity(0) {
}

// Parameterized constructor
Order::Order(int id, int custID, int initialCapacity)
    : orderID(id),
      customerID(custID),
      orderedItems(nullptr),
      quantities(nullptr),
      itemCount(0),
      capacity(0) {

    if (id <= 0) {
        throw invalid_argument("Order ID must be positive.");
    }

    if (custID <= 0) {
        throw invalid_argument("Customer ID must be positive.");
    }

    if (initialCapacity <= 0) {
        throw invalid_argument("Capacity must be positive.");
    }

    orderedItems = new MenuItem[initialCapacity];

    try {
        quantities = new int[initialCapacity];
    } catch (...) {
        delete[] orderedItems;
        orderedItems = nullptr;
        throw;
    }

    capacity = initialCapacity;
}

// Destructor
Order::~Order() {
    delete[] orderedItems;
    delete[] quantities;
}

// Expand the arrays when capacity is full
void Order::resizeArrays() {
    int newCapacity = (capacity == 0) ? 5 : capacity * 2;

    MenuItem* newItems = new MenuItem[newCapacity];

    int* newQuantities = nullptr;

    try {
        newQuantities = new int[newCapacity];
    } catch (...) {
        delete[] newItems;
        throw;
    }

    for (int i = 0; i < itemCount; i++) {
        newItems[i] = orderedItems[i];
        newQuantities[i] = quantities[i];
    }

    delete[] orderedItems;
    delete[] quantities;

    orderedItems = newItems;
    quantities = newQuantities;
    capacity = newCapacity;
}

// Add a menu item
void Order::addItem(const MenuItem& item, int quantity) {
    if (item.getItemID() <= 0) {
        throw invalid_argument("Invalid menu item.");
    }

    if (quantity <= 0) {
        throw invalid_argument("Quantity must be positive.");
    }

    // Combine quantities if the item already exists
    for (int i = 0; i < itemCount; i++) {
        if (orderedItems[i].getItemID() == item.getItemID()) {
            if (quantity > 1000000 - quantities[i]) {
                throw overflow_error("Quantity is too large.");
            }

            quantities[i] += quantity;
            return;
        }
    }

    if (itemCount == capacity) {
        resizeArrays();
    }

    orderedItems[itemCount] = item;
    quantities[itemCount] = quantity;
    itemCount++;
}

// Get order information
int Order::getOrderID() const {
    return orderID;
}

int Order::getCustomerID() const {
    return customerID;
}

int Order::getItemCount() const {
    return itemCount;
}

MenuItem Order::getItem(int index) const {
    if (index < 0 || index >= itemCount) {
        throw out_of_range("Invalid order item index.");
    }

    return orderedItems[index];
}

int Order::getQuantity(int index) const {
    if (index < 0 || index >= itemCount) {
        throw out_of_range("Invalid order item index.");
    }

    return quantities[index];
}

// Calculate subtotal
double Order::calculateSubtotal() const {
    double subtotal = 0.0;

    for (int i = 0; i < itemCount; i++) {
        subtotal += orderedItems[i].getPrice() * quantities[i];
    }

    return subtotal;
}

// Display order details
void Order::displayOrder() const {
    cout << "\n========== ORDER DETAILS ==========\n";
    cout << "Order ID: " << orderID << '\n';
    cout << "Customer ID: " << customerID << '\n';

    cout << left
         << setw(8) << "ID"
         << setw(22) << "Item"
         << setw(10) << "Price"
         << setw(10) << "Qty"
         << "Amount\n";

    cout << string(60, '-') << '\n';

    for (int i = 0; i < itemCount; i++) {
        double amount =
            orderedItems[i].getPrice() * quantities[i];

        cout << left
             << setw(8) << orderedItems[i].getItemID()
             << setw(22) << orderedItems[i].getItemName()
             << setw(10) << fixed << setprecision(2)
             << orderedItems[i].getPrice()
             << setw(10) << quantities[i]
             << amount << '\n';
    }

    cout << string(60, '-') << '\n';
    cout << "Subtotal: Rs. "
         << fixed << setprecision(2)
         << calculateSubtotal() << '\n';
}
