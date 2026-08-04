#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <string>

class Product {
protected:
    int id;
    std::string name;
    int quantity;
    double price;

public:
    Product(int id, std::string name, int quantity, double price);
    virtual ~Product() = default;

    int getId() const;
    std::string getName() const;
    int getQuantity() const;
    double getPrice() const;

    void setQuantity(int q);
    void setPrice(double p);

    virtual void display() const;
};

#endif