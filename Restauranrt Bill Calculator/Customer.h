
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
using namespace std;

class Customer {
private:
    int customerID;
    string customerName;
    string phoneNumber;

public:
    // Default constructor
    Customer();

    // Parameterized constructor
    Customer(int id, string name, string phone);

    // Destructor
    ~Customer();

    // Setter functions
    void setCustomerID(int id);
    void setCustomerName(string name);
    void setPhoneNumber(string phone);

    // Getter functions
    int getCustomerID() const;
    string getCustomerName() const;
    string getPhoneNumber() const;

    // Display customer details
    void displayCustomer() const;
};

#endif
