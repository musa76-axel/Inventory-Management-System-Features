#include "../include/Product.hpp"
#include "../include/Exceptions.hpp"


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
}