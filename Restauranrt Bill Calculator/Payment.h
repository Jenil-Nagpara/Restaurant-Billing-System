
#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>

class Payment {
protected:
    int paymentID;
    int billID;
    double amount;
    bool paymentSuccessful;

public:
    // Constructor
    Payment(int id, int bill, double paymentAmount);

    // Virtual destructor
    virtual ~Payment();

    // Pure virtual function for runtime polymorphism
    virtual bool processPayment() = 0;

    // Virtual function to display payment information
    virtual void displayPayment() const;

    // Getter functions
    int getPaymentID() const;
    int getBillID() const;
    double getAmount() const;
    bool isPaymentSuccessful() const;
};

// Derived class for cash payments
class CashPayment : public Payment {
private:
    double cashReceived;

public:
    CashPayment(int id, int bill, double paymentAmount,
                double receivedAmount);

    bool processPayment() override;
    void displayPayment() const override;
};

// Derived class for UPI payments
class UPIPayment : public Payment {
private:
    std::string upiReference;

public:
    UPIPayment(int id, int bill, double paymentAmount,
               std::string reference);

    bool processPayment() override;
    void displayPayment() const override;
};

#endif
