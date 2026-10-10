
#include "Bill.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

// Default constructor
Bill::Bill()
    : billID(0),
      orderID(0),
      taxRate(5.0),
      subtotal(0.0),
      taxAmount(0.0),
      totalAmount(0.0) {
}

// Parameterized constructor
Bill::Bill(int id, const Order& order, double taxPercentage)
    : billID(id),
      orderID(order.getOrderID()),
      taxRate(taxPercentage),
      subtotal(0.0),
      taxAmount(0.0),
      totalAmount(0.0) {

    if (id <= 0) {
        throw invalid_argument("Bill ID must be positive.");
    }

    if (taxPercentage < 0.0 || taxPercentage > 100.0) {
        throw invalid_argument("Tax rate must be between 0 and 100.");
    }

    calculateBill(order);
}

// Destructor
Bill::~Bill() {
}

// Calculate bill amounts
void Bill::calculateBill(const Order& order) {
    if (order.getOrderID() <= 0) {
        throw invalid_argument("Invalid order.");
    }

    subtotal = order.calculateSubtotal();
    taxAmount = subtotal * taxRate / 100.0;
    totalAmount = subtotal + taxAmount;
}

// Getter functions
int Bill::getBillID() const {
    return billID;
}

int Bill::getOrderID() const {
    return orderID;
}

double Bill::getSubtotal() const {
    return subtotal;
}

double Bill::getTaxRate() const {
    return taxRate;
}

double Bill::getTaxAmount() const {
    return taxAmount;
}

double Bill::getTotalAmount() const {
    return totalAmount;
}

// Display bill
void Bill::displayBill() const {
    cout << "\n========================================\n";
    cout << "             RESTAURANT BILL             \n";
    cout << "========================================\n";

    cout << "Bill ID: " << billID << '\n';
    cout << "Order ID: " << orderID << '\n';

    cout << fixed << setprecision(2);

    cout << "Subtotal:      Rs. " << subtotal << '\n';
    cout << "Tax (" << taxRate << "%):    Rs. " << taxAmount << '\n';

    cout << "----------------------------------------\n";
    cout << "TOTAL AMOUNT:  Rs. " << totalAmount << '\n';
    cout << "========================================\n";
}

