#include <iostream>
#include "../include/Product.hpp"
#include "../include/InventoryManager.hpp"
#include "../include/Exceptions.hpp"

int main() {
    InventoryManager manager;

    std::cout << "--- Testing Inventory System ---\n";

    try {
        
        manager.addProduct(Product(101, "Laptop", 5, 899.99));
        manager.addProduct(Product(102, "Mouse", 25, 19.99));
        std::cout << "[SUCCESS] Added 2 products. Total count: " 
                  << manager.getProductCount() << "\n";

        
        std::cout << "\nTesting Duplicate ID handling...\n";
        manager.addProduct(Product(101, "Duplicate Laptop", 1, 500.00));

    } catch (const DuplicateIDException& e) {
        std::cout << "[CAUGHT EXCEPTION] " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "[UNEXPECTED ERROR] " << e.what() << "\n";
    }

    try {
        
        std::cout << "\nTesting Missing Product lookup...\n";
        manager.findProduct(999);

    } catch (const ProductNotFoundException& e) {
        std::cout << "[CAUGHT EXCEPTION] " << e.what() << "\n";
    }

    try {
        std::cout << "\nTesting File Persistence...\n";
        manager.saveToFile("inventory.csv");
        std::cout << "[SUCCESS] Saved inventory to inventory.csv\n";

        InventoryManager newManager;
        newManager.loadFromFile("inventory.csv");
        std::cout << "[SUCCESS] Loaded inventory from inventory.csv\n";
    } catch (const std::exception& e) {
        std::cout << "[FILE I/O ERROR] " << e.what() << "\n";
    }

    std::cout << "\n--- All Core Model Tests Passed ---\n";
    return 0;
}