
#ifndef MENUITEM_H
#define MENUITEM_H

#include <string>
using namespace std;

class MenuItem {
private:
    int itemID;
    string itemName;
    string category;
    double price;

public:
    // Default constructor
    MenuItem();

    // Parameterized constructor
    MenuItem(int id, string name, string cat, double itemPrice);

    // Destructor
    ~MenuItem();

    // Setter functions
    void setItemID(int id);
    void setItemName(string name);
    void setCategory(string cat);
    void setPrice(double itemPrice);

    // Getter functions
    int getItemID() const;
    string getItemName() const;
    string getCategory() const;
    double getPrice() const;

    // Display item details
    void displayItem() const;
};

#endif
