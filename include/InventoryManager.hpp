#ifndef INVENTORY_MANAGER_HPP
#define INVENTORY_MANAGER_HPP

#include "Product.hpp"
#include <vector>
<<<<<<< HEAD
#include <string>
#include "FileHandler.hpp"
=======
>>>>>>> 010db06 (Add InventoryManager header and implementation)

class InventoryManager {
private:
    std::vector<Product> inventory;

public:
    // Core Inventory Operations
    void addProduct(const Product& product);
    void removeProduct(int id);
    Product* findProduct(int id);
<<<<<<< HEAD
    const std::vector<Product>&getAllProducts() const;
    
    void saveToFile(const std::string&filename) const;
    void loadFromFile(const std::string&filename);
=======
    const std::vector<Product>& getAllProducts() const;
>>>>>>> 010db06 (Add InventoryManager header and implementation)
    
    // Helper Methods
    bool productExists(int id) const;
    int getProductCount() const;
};

<<<<<<< HEAD

#endif
=======
#endif // INVENTORY_MANAGER_HPP
>>>>>>> 010db06 (Add InventoryManager header and implementation)
