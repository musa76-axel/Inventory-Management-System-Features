#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP

#include <string>
#include <vector>
#include "Product.hpp"
#include "InventoryManager.hpp"

class FileHandler {
public:
    static void saveToCSV(const std::string& filename, const std::vector<Product>& products);
    static std::vector<Product> loadFromCSV(const std::string& filename);

    // Convenience wrappers for GUI integration
    void saveInventory(const std::string& filename, const std::vector<Product>& products) {
        saveToCSV(filename, products);
    }
    
    void loadInventory(const std::string& filename, InventoryManager& manager) {
        auto products = loadFromCSV(filename);
        for (const auto& p : products) {
            manager.addProduct(p);
        }
    }
};

#endif