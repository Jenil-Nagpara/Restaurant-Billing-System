#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "MenuItem.h"
#include "Customer.h"
#include "Order.h"

#include <string>
#include <vector>
#include <memory>

class FileManager {
public:
    // Save and load menu items
    static bool saveMenu(
        const std::string& filename,
        const std::vector<MenuItem>& menu,
        std::string* error = nullptr
    );

    static bool loadMenu(
        const std::string& filename,
        std::vector<MenuItem>& menu,
        std::string* error = nullptr
    );

    // Save and load customers
    static bool saveCustomers(
        const std::string& filename,
        const std::vector<Customer>& customers,
        std::string* error = nullptr
    );

    static bool loadCustomers(
        const std::string& filename,
        std::vector<Customer>& customers,
        std::string* error = nullptr
    );

    // Save and load orders
    static bool saveOrders(
        const std::string& filename,
        const std::vector<std::unique_ptr<Order>>& orders,
        std::string* error = nullptr
    );

    static bool loadOrders(
        const std::string& filename,
        std::vector<std::unique_ptr<Order>>& orders,
        std::string* error = nullptr
    );
};

#endif