#ifndef INVENTORY_MANAGER_HPP
#define INVENTORY_MANAGER_HPP

#include "Product.hpp"
#include <vector>

class InventoryManager {
private:
    std::vector<Product> inventory;

public:
    // Core Inventory Operations
    void addProduct(const Product& product);
    void removeProduct(int id);
    Product* findProduct(int id);
    const std::vector<Product>& getAllProducts() const;
    
    // Helper Methods
    bool productExists(int id) const;
    int getProductCount() const;
};

#endif // INVENTORY_MANAGER_HPP