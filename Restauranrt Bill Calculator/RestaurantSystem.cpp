
#include "RestaurantSystem.h"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

namespace {

    // Read a complete line, including spaces.
    string readLine(const string& prompt) {
        cout << prompt;

        string value;

        if (!getline(cin, value)) {
            throw runtime_error("Input ended.");
        }

        return value;
    }

    // Read and validate an integer.
    int readInt(const string& prompt) {
        while (true) {
            string input = readLine(prompt);

            try {
                size_t position = 0;
                long long value = stoll(input, &position);

                while (position < input.size() &&
                       isspace(static_cast<unsigned char>(input[position]))) {
                    position++;
                }

                if (position != input.size() ||
                    value < numeric_limits<int>::min() ||
                    value > numeric_limits<int>::max()) {
                    throw invalid_argument("Invalid integer.");
                }

                return static_cast<int>(value);
            } catch (const exception&) {
                cout << "Please enter a valid whole number.\n";
            }
        }
    }

    // Read and validate a finite decimal number.
    double readDouble(const string& prompt) {
        while (true) {
            string input = readLine(prompt);

            try {
                size_t position = 0;
                double value = stod(input, &position);

                while (position < input.size() &&
                       isspace(static_cast<unsigned char>(input[position]))) {
                    position++;
                }

                if (position != input.size() || !isfinite(value)) {
                    throw invalid_argument("Invalid number.");
                }

                return value;
            } catch (const exception&) {
                cout << "Please enter a valid number.\n";
            }
        }
    }

    const MenuItem* findMenuItem(
        const vector<MenuItem>& menu,
        int id
    ) {
        for (const MenuItem& item : menu) {
            if (item.getItemID() == id) {
                return &item;
            }
        }

        return nullptr;
    }

    const Customer* findCustomer(
        const vector<Customer>& customers,
        int id
    ) {
        for (const Customer& customer : customers) {
            if (customer.getCustomerID() == id) {
                return &customer;
            }
        }

        return nullptr;
    }

    int findNextID(int largestID) {
        if (largestID == numeric_limits<int>::max()) {
            throw overflow_error("No more IDs are available.");
        }

        return largestID + 1;
    }

} // end anonymous namespace


// ============================================================
// CONSTRUCTOR AND DESTRUCTOR
// ============================================================

RestaurantSystem::RestaurantSystem() {
    // Ensure the data directory exists before saving files.
    error_code ec;
    filesystem::create_directories("Data", ec);

    if (ec) {
        cout << "Warning: Could not create Data directory: "
             << ec.message() << '\n';
    }

    loadAll();
}

RestaurantSystem::~RestaurantSystem() = default;


// ============================================================
// ID GENERATION
// ============================================================

int RestaurantSystem::nextMenuItemID() const {
    int largestID = 0;

    for (const MenuItem& item : menu) {
        largestID = max(largestID, item.getItemID());
    }

    return findNextID(largestID);
}

int RestaurantSystem::nextCustomerID() const {
    int largestID = 0;

    for (const Customer& customer : customers) {
        largestID = max(largestID, customer.getCustomerID());
    }

    return findNextID(largestID);
}

int RestaurantSystem::nextOrderID() const {
    int largestID = 0;

    for (const auto& order : orders) {
        if (order) {
            largestID = max(largestID, order->getOrderID());
        }
    }

    return findNextID(largestID);
}


// ============================================================
// MAIN MENU
// ============================================================

void RestaurantSystem::displayMainMenu() const {
    cout << "\n========================================\n";
    cout << "       RESTAURANT BILLING SYSTEM\n";
    cout << "========================================\n";
    cout << "1. Add Menu Item\n";
    cout << "2. Display Menu Items\n";
    cout << "3. Search Menu Item\n";
    cout << "4. Update Menu Item\n";
    cout << "5. Delete Menu Item\n";
    cout << "6. Main Transaction\n";
    cout << "7. Report\n";
    cout << "8. Save / Load Data\n";
    cout << "9. Exit\n";
    cout << "========================================\n";
}


// ============================================================
// 1. ADD MENU ITEM
// ============================================================

void RestaurantSystem::addMenuItem() {
    cout << "\n--- ADD MENU ITEM ---\n";

    int id = nextMenuItemID();

    string name = readLine("Enter item name: ");
    string category = readLine("Enter category: ");
    double price = readDouble("Enter price (Rs.): ");

    MenuItem item(id, name, category, price);

    menu.push_back(item);

    cout << "Menu item added successfully. ID: " << id << '\n';
}


// ============================================================
// 2. DISPLAY MENU ITEMS
// ============================================================

void RestaurantSystem::displayMenuItems() const {
    cout << "\n--- RESTAURANT MENU ---\n";

    if (menu.empty()) {
        cout << "No menu items available.\n";
        return;
    }

    cout << left
         << setw(8) << "ID"
         << setw(25) << "Item Name"
         << setw(18) << "Category"
         << "Price (Rs.)\n";

    cout << string(65, '-') << '\n';

    for (const MenuItem& item : menu) {
        item.displayItem();
    }
}


// ============================================================
// 3. SEARCH MENU ITEM
// ============================================================

void RestaurantSystem::searchMenuItem() const {
    cout << "\n--- SEARCH MENU ITEM ---\n";

    int id = readInt("Enter item ID: ");

    const MenuItem* item = findMenuItem(menu, id);

    if (item == nullptr) {
        cout << "Menu item not found.\n";
        return;
    }

    cout << left
         << setw(8) << "ID"
         << setw(25) << "Item Name"
         << setw(18) << "Category"
         << "Price (Rs.)\n";

    cout << string(65, '-') << '\n';
    item->displayItem();
}


// ============================================================
// 4. UPDATE MENU ITEM
// ============================================================

void RestaurantSystem::updateMenuItem() {
    cout << "\n--- UPDATE MENU ITEM ---\n";

    int id = readInt("Enter item ID to update: ");

    auto it = find_if(
        menu.begin(),
        menu.end(),
        [id](const MenuItem& item) {
            return item.getItemID() == id;
        }
    );

    if (it == menu.end()) {
        cout << "Menu item not found.\n";
        return;
    }

    cout << "Current name: " << it->getItemName() << '\n';
    cout << "Current category: " << it->getCategory() << '\n';
    cout << "Current price: Rs. "
         << fixed << setprecision(2) << it->getPrice() << '\n';

    string name = readLine("Enter new name: ");
    string category = readLine("Enter new category: ");
    double price = readDouble("Enter new price (Rs.): ");

    // Validate before modifying the existing object.
    MenuItem replacement(id, name, category, price);

    *it = replacement;

    cout << "Menu item updated successfully.\n";
}


// ============================================================
// 5. DELETE MENU ITEM
// ============================================================

void RestaurantSystem::deleteMenuItem() {
    cout << "\n--- DELETE MENU ITEM ---\n";

    int id = readInt("Enter item ID to delete: ");

    auto it = find_if(
        menu.begin(),
        menu.end(),
        [id](const MenuItem& item) {
            return item.getItemID() == id;
        }
    );

    if (it == menu.end()) {
        cout << "Menu item not found.\n";
        return;
    }

    string confirmation = readLine(
        "Delete " + it->getItemName() + "? Enter Y to confirm: "
    );

    if (confirmation == "Y" || confirmation == "y") {
        menu.erase(it);
        cout << "Menu item deleted successfully.\n";
        cout << "Existing orders retain their recorded item details.\n";
    } else {
        cout << "Deletion cancelled.\n";
    }
}


// ============================================================
// CUSTOMER MANAGEMENT
// ============================================================

int RestaurantSystem::addCustomer() {
    cout << "\n--- ADD CUSTOMER ---\n";

    int id = nextCustomerID();

    string name = readLine("Enter customer name: ");
    string phone = readLine("Enter phone number: ");

    Customer customer(id, name, phone);
    customers.push_back(customer);

    cout << "Customer added successfully. ID: " << id << '\n';

    return id;
}

void RestaurantSystem::displayCustomers() const {
    cout << "\n--- CUSTOMER LIST ---\n";

    if (customers.empty()) {
        cout << "No customers registered.\n";
        return;
    }

    cout << left
         << setw(12) << "Customer ID"
         << setw(25) << "Name"
         << "Phone\n";

    cout << string(55, '-') << '\n';

    for (const Customer& customer : customers) {
        customer.displayCustomer();
    }
}


// ============================================================
// 6. MAIN TRANSACTION
// ============================================================

void RestaurantSystem::processTransaction() {
    cout << "\n--- MAIN TRANSACTION ---\n";

    if (menu.empty()) {
        cout << "The menu is empty. Add menu items first.\n";
        return;
    }

    // Select an existing customer or create a new one.
    int customerID;

    if (customers.empty()) {
        cout << "No registered customers. Please add one.\n";
        customerID = addCustomer();
    } else {
        displayCustomers();

        customerID = readInt(
            "Enter customer ID, or 0 to add a new customer: "
        );

        if (customerID == 0) {
            customerID = addCustomer();
        } else if (findCustomer(customers, customerID) == nullptr) {
            cout << "Customer ID not found. Transaction cancelled.\n";
            return;
        }
    }

    int orderID = nextOrderID();

    unique_ptr<Order> order(
        new Order(orderID, customerID)
    );

    cout << "\nOrder ID: " << orderID << '\n';
    displayMenuItems();

    cout << "\nEnter item IDs and quantities.\n";
    cout << "Enter item ID 0 when you have finished adding items.\n";

    while (true) {
        int itemID = readInt("Item ID (0 to finish): ");

        if (itemID == 0) {
            break;
        }

        const MenuItem* item = findMenuItem(menu, itemID);

        if (item == nullptr) {
            cout << "Item ID not found. Try again.\n";
            continue;
        }

        int quantity = readInt("Enter quantity: ");

        try {
            order->addItem(*item, quantity);
            cout << "Item added to order.\n";
        } catch (const exception& ex) {
            cout << "Could not add item: " << ex.what() << '\n';
        }
    }

    if (order->getItemCount() == 0) {
        cout << "No items were ordered. Transaction cancelled.\n";
        saveAll();
        return;
    }

    order->displayOrder();

    // Use the order ID as the bill ID in this simple application.
    Bill bill(orderID, *order);

    bill.displayBill();

    cout << "\nSelect payment method:\n";
    cout << "1. Cash\n";
    cout << "2. UPI (simulation)\n";

    int paymentChoice;

    do {
        paymentChoice = readInt("Choice: ");

        if (paymentChoice != 1 && paymentChoice != 2) {
            cout << "Please select 1 or 2.\n";
        }
    } while (paymentChoice != 1 && paymentChoice != 2);

    unique_ptr<Payment> payment;

    if (paymentChoice == 1) {
        double received = readDouble("Cash received (Rs.): ");

        payment.reset(
            new CashPayment(
                orderID,
                bill.getBillID(),
                bill.getTotalAmount(),
                received
            )
        );
    } else {
        string reference = readLine("Enter UPI reference for simulation: ");

        payment.reset(
            new UPIPayment(
                orderID,
                bill.getBillID(),
                bill.getTotalAmount(),
                reference
            )
        );
    }

    // Runtime polymorphism: the derived implementation is called
    // through a pointer to the abstract Payment base class.
    bool successful = payment->processPayment();

    payment->displayPayment();

    if (successful) {
        orders.push_back(std::move(order));
        cout << "\nOrder recorded successfully.\n";
    } else {
        cout << "\nPayment failed. The order was not recorded.\n";
    }

    // Save changes, including a newly registered customer.
    saveAll();
}


// ============================================================
// 7. REPORT
// ============================================================

void RestaurantSystem::displayReport() const {
    cout << "\n========================================\n";
    cout << "          RESTAURANT REPORT\n";
    cout << "========================================\n";

    cout << "Total menu items: " << menu.size() << '\n';
    cout << "Total customers:  " << customers.size() << '\n';
    cout << "Completed orders: " << orders.size() << '\n';

    double subtotalRevenue = 0.0;

    for (const auto& order : orders) {
        if (order) {
            subtotalRevenue += order->calculateSubtotal();
        }
    }

    const double taxRate = 5.0;
    double tax = subtotalRevenue * taxRate / 100.0;

    cout << fixed << setprecision(2);
    cout << "Order subtotals:  Rs. " << subtotalRevenue << '\n';
    cout << "Estimated tax:    Rs. " << tax << '\n';
    cout << "Subtotal + tax:   Rs. "
         << subtotalRevenue + tax << '\n';

    cout << "========================================\n";

    if (!orders.empty()) {
        cout << "\n--- ORDER SUMMARY ---\n";

        for (const auto& order : orders) {
            if (order) {
                cout << "Order ID: " << order->getOrderID()
                     << " | Customer ID: " << order->getCustomerID()
                     << " | Subtotal: Rs. "
                     << order->calculateSubtotal() << '\n';
            }
        }
    }
}


// ============================================================
// 8. SAVE AND LOAD
// ============================================================

void RestaurantSystem::saveAll() {
    cout << "\nSaving application data...\n";

    string error;
    bool allSaved = true;

    if (!FileManager::saveMenu("Data/menu.txt", menu, &error)) {
        cout << "Menu save failed: " << error << '\n';
        allSaved = false;
    }

    if (!FileManager::saveCustomers(
            "Data/customers.txt", customers, &error)) {
        cout << "Customer save failed: " << error << '\n';
        allSaved = false;
    }

    if (!FileManager::saveOrders(
            "Data/orders.txt", orders, &error)) {
        cout << "Order save failed: " << error << '\n';
        allSaved = false;
    }

    if (allSaved) {
        cout << "All application data saved successfully.\n";
    } else {
        cout << "One or more files could not be saved. "
             << "Check the errors above.\n";
    }
}

void RestaurantSystem::loadAll() {
    string error;

    // Missing files are normal on the first launch.
    if (filesystem::exists("Data/menu.txt")) {
        if (FileManager::loadMenu("Data/menu.txt", menu, &error)) {
            cout << "Menu data loaded.\n";
        } else {
            cout << "Menu load warning: " << error << '\n';
        }
    }

    if (filesystem::exists("Data/customers.txt")) {
        if (FileManager::loadCustomers(
                "Data/customers.txt", customers, &error)) {
            cout << "Customer data loaded.\n";
        } else {
            cout << "Customer load warning: " << error << '\n';
        }
    }

    if (filesystem::exists("Data/orders.txt")) {
        if (FileManager::loadOrders(
                "Data/orders.txt", orders, &error)) {
            cout << "Order data loaded.\n";
        } else {
            cout << "Order load warning: " << error << '\n';
        }
    }
}

void RestaurantSystem::saveLoadMenu() {
    cout << "\n--- SAVE / LOAD DATA ---\n";
    cout << "1. Save all data\n";
    cout << "2. Load data from files\n";
    cout << "0. Return to main menu\n";

    int choice = readInt("Enter choice: ");

    switch (choice) {
        case 1:
            saveAll();
            break;

        case 2:
            loadAll();
            break;

        case 0:
            cout << "Returning to main menu.\n";
            break;

        default:
            cout << "Invalid choice.\n";
    }
}


// ============================================================
// RUN APPLICATION
// ============================================================

void RestaurantSystem::run() {
    bool running = true;

    while (running) {
        try {
            displayMainMenu();

            int choice = readInt("Enter your choice (1-9): ");

            try {
                switch (choice) {
                    case 1:
                        addMenuItem();
                        break;

                    case 2:
                        displayMenuItems();
                        break;

                    case 3:
                        searchMenuItem();
                        break;

                    case 4:
                        updateMenuItem();
                        break;

                    case 5:
                        deleteMenuItem();
                        break;

                    case 6:
                        processTransaction();
                        break;

                    case 7:
                        displayReport();
                        break;

                    case 8:
                        saveLoadMenu();
                        break;

                    case 9:
                        saveAll();
                        cout << "Thank you for using the "
                             << "Restaurant Billing System.\n";
                        running = false;
                        break;

                    default:
                        cout << "Invalid choice. Enter a number from 1 to 9.\n";
                }
            } catch (const exception& ex) {
                cout << "Operation failed: " << ex.what() << '\n';
            }

        } catch (const exception&) {
            cout << "\nInput ended. Exiting application.\n";
            running = false;
        }
    }
}
