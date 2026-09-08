#ifndef INVENTORY_MANAGER_HPP
#define INVENTORY_MANAGER_HPP

#include "Product.hpp"
#include <vector>
#include <string>
#include "FileHandler.hpp"

class InventoryManager {
private:
    std::vector<Product> inventory;

public:
    // Core Inventory Operations
    void addProduct(const Product& product);
    void removeProduct(int id);
    Product* findProduct(int id);
    const std::vector<Product>&getAllProducts() const;
    
    void saveToFile(const std::string&filename) const;
    void loadFromFile(const std::string&filename);
    
    // Helper Methods
    bool productExists(int id) const;
    int getProductCount() const;
};


#endif