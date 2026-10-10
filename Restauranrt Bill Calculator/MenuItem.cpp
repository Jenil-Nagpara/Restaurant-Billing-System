
#include "MenuItem.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <utility>

using namespace std;

// Default constructor
MenuItem::MenuItem()
    : itemID(0), itemName(""), category(""), price(0.0) {
}

// Parameterized constructor
MenuItem::MenuItem(int id, string name, string cat, double itemPrice)
    : itemID(id), itemName(std::move(name)),
      category(std::move(cat)), price(itemPrice) {

    if (id <= 0) {
        throw invalid_argument("Item ID must be positive.");
    }

    if (itemName.empty()) {
        throw invalid_argument("Item name cannot be empty.");
    }

    if (category.empty()) {
        throw invalid_argument("Category cannot be empty.");
    }

    if (itemPrice < 0.0) {
        throw invalid_argument("Price cannot be negative.");
    }
}

// Destructor
MenuItem::~MenuItem() {
}

// Setter functions
void MenuItem::setItemID(int id) {
    if (id <= 0) {
        throw invalid_argument("Item ID must be positive.");
    }
    itemID = id;
}

void MenuItem::setItemName(string name) {
    if (name.empty()) {
        throw invalid_argument("Item name cannot be empty.");
    }
    itemName = std::move(name);
}

void MenuItem::setCategory(string cat) {
    if (cat.empty()) {
        throw invalid_argument("Category cannot be empty.");
    }
    category = std::move(cat);
}

void MenuItem::setPrice(double itemPrice) {
    if (itemPrice < 0.0) {
        throw invalid_argument("Price cannot be negative.");
    }
    price = itemPrice;
}

// Getter functions
int MenuItem::getItemID() const {
    return itemID;
}

string MenuItem::getItemName() const {
    return itemName;
}

string MenuItem::getCategory() const {
    return category;
}

double MenuItem::getPrice() const {
    return price;
}

// Display item details
void MenuItem::displayItem() const {
    cout << left
         << setw(8) << itemID
         << setw(25) << itemName
         << setw(18) << category
         << right << fixed << setprecision(2)
         << price << '\n';
}
