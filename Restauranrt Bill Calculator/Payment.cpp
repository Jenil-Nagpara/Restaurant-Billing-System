
#include "Payment.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <utility>
#include <cmath>

using namespace std;

// Base class constructor
Payment::Payment(int id, int bill, double paymentAmount)
    : paymentID(id),
      billID(bill),
      amount(paymentAmount),
      paymentSuccessful(false) {

    if (id <= 0) {
        throw invalid_argument("Payment ID must be positive.");
    }

    if (bill <= 0) {
        throw invalid_argument("Bill ID must be positive.");
    }

    if (!isfinite(paymentAmount) || paymentAmount <= 0.0) {
        throw invalid_argument("Payment amount must be positive and finite.");
    }
}

// Virtual destructor
Payment::~Payment() {
}

// Base class display function
void Payment::displayPayment() const {
    cout << "Payment ID: " << paymentID << '\n';
    cout << "Bill ID: " << billID << '\n';
    cout << fixed << setprecision(2);
    cout << "Amount: Rs. " << amount << '\n';
    cout << "Status: "
         << (paymentSuccessful ? "Successful" : "Not successful")
         << '\n';
}

// Getter functions
int Payment::getPaymentID() const {
    return paymentID;
}

int Payment::getBillID() const {
    return billID;
}

double Payment::getAmount() const {
    return amount;
}

bool Payment::isPaymentSuccessful() const {
    return paymentSuccessful;
}

// CashPayment constructor
CashPayment::CashPayment(int id, int bill, double paymentAmount,
                         double receivedAmount)
    : Payment(id, bill, paymentAmount),
      cashReceived(receivedAmount) {

    if (!isfinite(receivedAmount) || receivedAmount < 0.0) {
        throw invalid_argument("Cash received cannot be negative.");
    }
}

// Process cash payment
bool CashPayment::processPayment() {
    if (cashReceived < amount) {
        paymentSuccessful = false;
        cout << "Insufficient cash received.\n";
        return false;
    }

    paymentSuccessful = true;

    cout << fixed << setprecision(2);
    cout << "Cash payment successful.\n";
    cout << "Change: Rs. " << cashReceived - amount << '\n';

    return true;
}

// Display cash payment
void CashPayment::displayPayment() const {
    cout << "\n--- CASH PAYMENT ---\n";
    Payment::displayPayment();
    cout << fixed << setprecision(2);
    cout << "Cash received: Rs. " << cashReceived << '\n';
}

// UPIPayment constructor
UPIPayment::UPIPayment(int id, int bill, double paymentAmount,
                       string reference)
    : Payment(id, bill, paymentAmount),
      upiReference(std::move(reference)) {

    if (upiReference.empty()) {
        throw invalid_argument("UPI reference cannot be empty.");
    }
}

// Process simulated UPI payment
bool UPIPayment::processPayment() {
    // Academic simulation only; no real payment is processed.
    paymentSuccessful = true;

    cout << "Simulated UPI payment successful.\n";
    cout << "Reference: " << upiReference << '\n';

    return true;
}

// Display UPI payment
void UPIPayment::displayPayment() const {
    cout << "\n--- UPI PAYMENT (SIMULATION) ---\n";
    Payment::displayPayment();
    cout << "Reference: " << upiReference << '\n';
}
