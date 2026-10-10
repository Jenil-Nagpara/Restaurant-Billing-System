
#ifndef ORDER_H
#define ORDER_H

#include "MenuItem.h"
#include <string>

class Order {
private:
    int orderID;
    int customerID;

    MenuItem* orderedItems;
    int* quantities;

    int itemCount;
    int capacity;

    void resizeArrays();

public:
    // Default constructor
    Order();

    // Parameterized constructor
    Order(int id, int custID, int initialCapacity = 5);

    // Destructor
    ~Order();

    // Prevent accidental shallow copies of dynamically allocated arrays
    Order(const Order&) = delete;
    Order& operator=(const Order&) = delete;

    // Add a menu item and its quantity
    void addItem(const MenuItem& item, int quantity);

    // Get order information
    int getOrderID() const;
    int getCustomerID() const;
    int getItemCount() const;

    MenuItem getItem(int index) const;
    int getQuantity(int index) const;

    // Calculate order subtotal
    double calculateSubtotal() const;

    // Display order details
    void displayOrder() const;
};

#endif
