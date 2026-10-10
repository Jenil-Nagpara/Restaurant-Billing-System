
#ifndef BILL_H
#define BILL_H

#include "Order.h"

class Bill {
private:
    int billID;
    int orderID;
    double taxRate;
    double subtotal;
    double taxAmount;
    double totalAmount;

public:
    // Default constructor
    Bill();

    // Parameterized constructor
    Bill(int id, const Order& order, double taxPercentage = 5.0);

    // Destructor
    ~Bill();

    // Calculate bill amounts
    void calculateBill(const Order& order);

    // Getter functions
    int getBillID() const;
    int getOrderID() const;
    double getSubtotal() const;
    double getTaxRate() const;
    double getTaxAmount() const;
    double getTotalAmount() const;

    // Display bill
    void displayBill() const;
};

#endif
