
#include "RestaurantSystem.h"

#include <exception>
#include <iostream>

using namespace std;

int main() {
    try {
        RestaurantSystem restaurant;
        restaurant.run();
    } catch (const exception& ex) {
        cerr << "Application could not start: "
             << ex.what() << '\n';
        return 1;
    }

    return 0;
}

