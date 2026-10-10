
#ifndef RESTAURANTSYSTEM_H
#define RESTAURANTSYSTEM_H

#include "MenuItem.h"
#include "Customer.h"
#include "Order.h"
#include "Bill.h"
#include "Payment.h"
#include "FileManager.h"

#include <memory>
#include <string>
#include <vector>

class RestaurantSystem {
private:
    std::vector<MenuItem> menu;
    std::vector<Customer> customers;
    std::vector<std::unique_ptr<Order>> orders;

    int nextMenuItemID() const;
    int nextCustomerID() const;
    int nextOrderID() const;

    void displayMainMenu() const;

    void addMenuItem();
    void displayMenuItems() const;
    void searchMenuItem() const;
    void updateMenuItem();
    void deleteMenuItem();

    int addCustomer();
    void displayCustomers() const;

    void processTransaction();
    void displayReport() const;

    void saveAll();
    void loadAll();
    void saveLoadMenu();

public:
    RestaurantSystem();
    ~RestaurantSystem();

    // Start the application
    void run();
};

#endif
