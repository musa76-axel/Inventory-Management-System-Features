#include "../include/InventoryManager.hpp"
#include "../include/Exceptions.hpp"
<<<<<<< HEAD
#include "../include/FileHandler.hpp"
=======
>>>>>>> 010db06 (Add InventoryManager header and implementation)
#include <algorithm>

void InventoryManager::addProduct(const Product& product) {
    if (productExists(product.getId())) {
        throw DuplicateIDException("Product with this ID already exists.");
    }
    inventory.push_back(product);
}

void InventoryManager::removeProduct(int id) {
    auto it = std::remove_if(inventory.begin(), inventory.end(),
        [id](const Product& p) { return p.getId() == id; });

    if (it == inventory.end()) {
        throw ProductNotFoundException("Cannot remove: Product ID not found.");
    }

    inventory.erase(it, inventory.end());
}

Product* InventoryManager::findProduct(int id) {
    for (auto& product : inventory) {
        if (product.getId() == id) {
            return &product;
        }
    }
    throw ProductNotFoundException("Product ID not found.");
}

const std::vector<Product>& InventoryManager::getAllProducts() const {
    return inventory;
}

bool InventoryManager::productExists(int id) const {
    for (const auto& product : inventory) {
        if (product.getId() == id) {
            return true;
        }
    }
    return false;
}

int InventoryManager::getProductCount() const {
    return static_cast<int>(inventory.size());
<<<<<<< HEAD
}

void InventoryManager::saveToFile(const std::string& filename) const {
    FileHandler::saveToCSV(filename, inventory);
}

void InventoryManager::loadFromFile(const std::string& filename) {
    inventory = FileHandler::loadFromCSV(filename);
=======
>>>>>>> 010db06 (Add InventoryManager header and implementation)
}