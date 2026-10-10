#include "FileManager.h"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cstdio>
#include <unordered_set>
#include <stdexcept>
#include <functional>
#include <utility>
#include <cmath>

using namespace std;

namespace {

    const int MAX_RECORDS = 100000;
    const int MAX_ITEMS_PER_ORDER = 10000;

    // Store an error message if the caller requests one.
    void setError(string* error, const string& message) {
        if (error != nullptr) {
            *error = message;
        }
    }

    // Save to a temporary file before replacing the destination.
    bool writeAtomically(
        const string& filename,
        const function<bool(ostream&)>& writer,
        string* error
    ) {
        const string temporaryFile = filename + ".tmp";
        const string backupFile = filename + ".bak";

        ofstream output(temporaryFile, ios::out | ios::trunc);

        if (!output.is_open()) {
            setError(error, "Cannot open temporary file: " + temporaryFile);
            return false;
        }

        bool written = false;

        try {
            written = writer(output);
        } catch (const exception& ex) {
            setError(error, string("Error while writing data: ") + ex.what());
            output.close();
            remove(temporaryFile.c_str());
            return false;
        }

        output.flush();

        if (!output.good()) {
            written = false;
        }

        output.close();

        if (!written || output.fail()) {
            setError(error, "Failed to write all data to: " + temporaryFile);
            remove(temporaryFile.c_str());
            return false;
        }

        // If no destination exists, move the temporary file into place.
        if (rename(temporaryFile.c_str(), filename.c_str()) == 0) {
            if (error != nullptr) {
                error->clear();
            }
            return true;
        }

        // The destination may already exist. Move it to a backup first.
        if (rename(filename.c_str(), backupFile.c_str()) != 0) {
            setError(
                error,
                "Could not replace destination file: " + filename
            );

            remove(temporaryFile.c_str());
            return false;
        }

        // Try to install the new file.
        if (rename(temporaryFile.c_str(), filename.c_str()) != 0) {
            // Restore the previous file if replacement fails.
            if (rename(backupFile.c_str(), filename.c_str()) != 0) {
                setError(
                    error,
                    "Replacement failed. Previous data may be available in: "
                    + backupFile
                );
            } else {
                setError(error, "Replacement failed; original file restored.");
            }

            remove(temporaryFile.c_str());
            return false;
        }

        // The new file is in place. Remove the old backup.
        remove(backupFile.c_str());

        if (error != nullptr) {
            error->clear();
        }

        return true;
    }

    // Read one complete data file into memory.
    bool readFile(
        const string& filename,
        string& contents,
        string* error
    ) {
        ifstream input(filename);

        if (!input.is_open()) {
            setError(error, "Cannot open file for reading: " + filename);
            return false;
        }

        ostringstream buffer;
        buffer << input.rdbuf();

        if (input.bad()) {
            setError(error, "Error while reading file: " + filename);
            return false;
        }

        contents = buffer.str();

        if (error != nullptr) {
            error->clear();
        }

        return true;
    }

    bool isBlankOrComment(const string& line) {
        istringstream input(line);
        string first;

        if (!(input >> first)) {
            return true;
        }

        return !first.empty() && first[0] == '#';
    }

} // end anonymous namespace


// ============================================================
// MENU: SAVE
// ============================================================

bool FileManager::saveMenu(
    const string& filename,
    const vector<MenuItem>& menu,
    string* error
) {
    if (menu.size() > MAX_RECORDS) {
        setError(error, "Too many menu records.");
        return false;
    }

    return writeAtomically(
        filename,
        [&menu](ostream& output) {
            output << "# Restaurant menu data\n";
            output << fixed << setprecision(2);

            for (const MenuItem& item : menu) {
                if (item.getItemID() <= 0 ||
                    item.getItemName().empty() ||
                    item.getCategory().empty() ||
                    !isfinite(item.getPrice()) ||
                    item.getPrice() < 0.0) {
                    return false;
                }

                output << item.getItemID() << ' '
                       << quoted(item.getItemName()) << ' '
                       << quoted(item.getCategory()) << ' '
                       << item.getPrice() << '\n';

                if (!output.good()) {
                    return false;
                }
            }

            return true;
        },
        error
    );
}


// ============================================================
// MENU: LOAD
// ============================================================

bool FileManager::loadMenu(
    const string& filename,
    vector<MenuItem>& menu,
    string* error
) {
    string contents;

    if (!readFile(filename, contents, error)) {
        return false;
    }

    istringstream input(contents);
    string line;

    vector<MenuItem> loadedMenu;
    unordered_set<int> usedIDs;

    try {
        while (getline(input, line)) {
            if (isBlankOrComment(line)) {
                continue;
            }

            if (loadedMenu.size() >= MAX_RECORDS) {
                throw runtime_error("Too many menu records.");
            }

            istringstream row(line);

            int id;
            string name;
            string category;
            double price;

            if (!(row >> id >> quoted(name)
                      >> quoted(category) >> price)) {
                throw runtime_error("Invalid menu record.");
            }

            row >> ws;

            if (!row.eof()) {
                throw runtime_error("Unexpected data in menu record.");
            }

            if (!usedIDs.insert(id).second) {
                throw runtime_error("Duplicate menu item ID.");
            }

            loadedMenu.emplace_back(id, name, category, price);
        }
    } catch (const exception& ex) {
        setError(error, string("Could not load menu: ") + ex.what());
        return false;
    }

    // Replace existing records only after the entire file is valid.
    menu.swap(loadedMenu);

    if (error != nullptr) {
        error->clear();
    }

    return true;
}


// ============================================================
// CUSTOMERS: SAVE
// ============================================================

bool FileManager::saveCustomers(
    const string& filename,
    const vector<Customer>& customers,
    string* error
) {
    if (customers.size() > MAX_RECORDS) {
        setError(error, "Too many customer records.");
        return false;
    }

    return writeAtomically(
        filename,
        [&customers](ostream& output) {
            output << "# Restaurant customer data\n";

            for (const Customer& customer : customers) {
                if (customer.getCustomerID() <= 0 ||
                    customer.getCustomerName().empty()) {
                    return false;
                }

                output << customer.getCustomerID() << ' '
                       << quoted(customer.getCustomerName()) << ' '
                       << quoted(customer.getPhoneNumber()) << '\n';

                if (!output.good()) {
                    return false;
                }
            }

            return true;
        },
        error
    );
}


// ============================================================
// CUSTOMERS: LOAD
// ============================================================

bool FileManager::loadCustomers(
    const string& filename,
    vector<Customer>& customers,
    string* error
) {
    string contents;

    if (!readFile(filename, contents, error)) {
        return false;
    }

    istringstream input(contents);
    string line;

    vector<Customer> loadedCustomers;
    unordered_set<int> usedIDs;

    try {
        while (getline(input, line)) {
            if (isBlankOrComment(line)) {
                continue;
            }

            if (loadedCustomers.size() >= MAX_RECORDS) {
                throw runtime_error("Too many customer records.");
            }

            istringstream row(line);

            int id;
            string name;
            string phone;

            if (!(row >> id >> quoted(name) >> quoted(phone))) {
                throw runtime_error("Invalid customer record.");
            }

            row >> ws;

            if (!row.eof()) {
                throw runtime_error("Unexpected data in customer record.");
            }

            if (!usedIDs.insert(id).second) {
                throw runtime_error("Duplicate customer ID.");
            }

            loadedCustomers.emplace_back(id, name, phone);
        }
    } catch (const exception& ex) {
        setError(error, string("Could not load customers: ") + ex.what());
        return false;
    }

    customers.swap(loadedCustomers);

    if (error != nullptr) {
        error->clear();
    }

    return true;
}


// ============================================================
// ORDERS: SAVE
// ============================================================

bool FileManager::saveOrders(
    const string& filename,
    const vector<unique_ptr<Order>>& orders,
    string* error
) {
    if (orders.size() > MAX_RECORDS) {
        setError(error, "Too many order records.");
        return false;
    }

    return writeAtomically(
        filename,
        [&orders](ostream& output) {
            output << "# Restaurant order data\n";
            output << fixed << setprecision(2);

            for (const unique_ptr<Order>& order : orders) {
                if (!order) {
                    return false;
                }

                if (order->getOrderID() <= 0 ||
                    order->getCustomerID() <= 0 ||
                    order->getItemCount() > MAX_ITEMS_PER_ORDER) {
                    return false;
                }

                output << order->getOrderID() << ' '
                       << order->getCustomerID() << ' '
                       << order->getItemCount();

                for (int i = 0; i < order->getItemCount(); i++) {
                    MenuItem item = order->getItem(i);
                    int quantity = order->getQuantity(i);

                    if (item.getItemID() <= 0 ||
                        item.getItemName().empty() ||
                        item.getCategory().empty() ||
                        !isfinite(item.getPrice()) ||
                        item.getPrice() < 0.0 ||
                        quantity <= 0) {
                        return false;
                    }

                    output << ' '
                           << item.getItemID() << ' '
                           << quoted(item.getItemName()) << ' '
                           << quoted(item.getCategory()) << ' '
                           << item.getPrice() << ' '
                           << quantity;
                }

                output << '\n';

                if (!output.good()) {
                    return false;
                }
            }

            return true;
        },
        error
    );
}


// ============================================================
// ORDERS: LOAD
// ============================================================

bool FileManager::loadOrders(
    const string& filename,
    vector<unique_ptr<Order>>& orders,
    string* error
) {
    string contents;

    if (!readFile(filename, contents, error)) {
        return false;
    }

    istringstream input(contents);
    string line;

    vector<unique_ptr<Order>> loadedOrders;
    unordered_set<int> usedOrderIDs;

    try {
        while (getline(input, line)) {
            if (isBlankOrComment(line)) {
                continue;
            }

            if (loadedOrders.size() >= MAX_RECORDS) {
                throw runtime_error("Too many order records.");
            }

            istringstream row(line);

            int orderID;
            int customerID;
            int itemCount;

            if (!(row >> orderID >> customerID >> itemCount)) {
                throw runtime_error("Invalid order record.");
            }

            if (itemCount < 0 || itemCount > MAX_ITEMS_PER_ORDER) {
                throw runtime_error("Invalid number of items in order.");
            }

            if (!usedOrderIDs.insert(orderID).second) {
                throw runtime_error("Duplicate order ID.");
            }

            unique_ptr<Order> order(
                new Order(orderID, customerID)
            );

            for (int i = 0; i < itemCount; i++) {
                int itemID;
                string itemName;
                string category;
                double price;
                int quantity;

                if (!(row >> itemID
                          >> quoted(itemName)
                          >> quoted(category)
                          >> price
                          >> quantity)) {
                    throw runtime_error("Invalid item in order record.");
                }

                MenuItem item(itemID, itemName, category, price);

                order->addItem(item, quantity);
            }

            row >> ws;

            if (!row.eof()) {
                throw runtime_error("Unexpected data in order record.");
            }

            loadedOrders.push_back(std::move(order));
        }
    } catch (const exception& ex) {
        setError(error, string("Could not load orders: ") + ex.what());
        return false;
    }

    orders.swap(loadedOrders);

    if (error != nullptr) {
        error->clear();
    }

    return true;
}