
#include "Customer.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <utility>

using namespace std;

// Default constructor
Customer::Customer()
    : customerID(0), customerName(""), phoneNumber("") {
}

// Parameterized constructor
Customer::Customer(int id, string name, string phone)
    : customerID(id),
      customerName(std::move(name)),
      phoneNumber(std::move(phone)) {

    if (id <= 0) {
        throw invalid_argument("Customer ID must be positive.");
    }

    if (customerName.empty()) {
        throw invalid_argument("Customer name cannot be empty.");
    }
}

// Destructor
Customer::~Customer() {
}

// Setter functions
void Customer::setCustomerID(int id) {
    if (id <= 0) {
        throw invalid_argument("Customer ID must be positive.");
    }

    customerID = id;
}

void Customer::setCustomerName(string name) {
    if (name.empty()) {
        throw invalid_argument("Customer name cannot be empty.");
    }

    customerName = std::move(name);
}

void Customer::setPhoneNumber(string phone) {
    phoneNumber = std::move(phone);
}

// Getter functions
int Customer::getCustomerID() const {
    return customerID;
}

string Customer::getCustomerName() const {
    return customerName;
}

string Customer::getPhoneNumber() const {
    return phoneNumber;
}

// Display customer details
void Customer::displayCustomer() const {
    cout << left
         << setw(12) << customerID
         << setw(25) << customerName
         << phoneNumber << '\n';
}
