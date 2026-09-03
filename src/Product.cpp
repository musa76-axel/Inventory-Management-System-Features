#include "../include/Product.hpp"
#include "../include/Exceptions.hpp"
 HEAD
 HEAD
#include <iostream>

 30e5304 (Implement Product entity class and exception integration)

#include <iostream>
 8fb1909 (fix: implement Product::display and fix setPrice syntax error)


Product::Product(int id, std::string name, int quantity, double price)
    : id(id), name(name) {
    setQuantity(quantity);
    setPrice(price);
}


int Product::getId() const { return id; }
std::string Product::getName() const { return name; }
int Product::getQuantity() const { return quantity; }
double Product::getPrice() const { return price; }


void Product::setQuantity(int q) {
    if (q < 0) {
        throw InvalidQuantityException("Quantity cannot be negative.");
    }
    quantity = q;
}

void Product::setPrice(double p) {
    if (p < 0.0) {
        throw InventoryException("Price cannot be negative.");
    }
    price = p;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 8fb1909 (fix: implement Product::display and fix setPrice syntax error)
}
void Product::display() const {
    std::cout << "ID: " << id 
                << " | Name: " << name 
                << " | Quantity: " << quantity 
                << " | Price: $" << price << "\n";
}

<<<<<<< HEAD
=======
}
>>>>>>> 30e5304 (Implement Product entity class and exception integration)
=======
>>>>>>> 8fb1909 (fix: implement Product::display and fix setPrice syntax error)
