#include "FileHandler.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

void FileHandler::saveToCSV(const std::string& filename, const std::vector<Product>& products) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("FileHandler Error: Could not open file for writing: " + filename);
    }

    // Write CSV column headers
    file << "ID,Name,Category,Price,Quantity\n";

    // Write product data lines
    for (const auto& item : products) {
        file << item.getId() << ","
             << item.getName() << ","
             << item.getCategory() << ","
             << item.getPrice() << ","
             << item.getQuantity() << "\n";
    }

    file.close();
}

std::vector<Product> FileHandler::loadFromCSV(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("FileHandler Error: Could not open file for reading: " + filename);
    }

    std::vector<Product> loadedProducts;
    std::string line;

    // Skip the header row
    if (std::getline(file, line)) {
        // Header consumed
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string idStr, name, category, priceStr, qtyStr;

        if (std::getline(ss, idStr, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, category, ',') &&
            std::getline(ss, priceStr, ',') &&
            std::getline(ss, qtyStr, ',')) {

            int id = std::stoi(idStr);
            double price = std::stod(priceStr);
            int quantity = std::stoi(qtyStr);

            loadedProducts.emplace_back(id, name, category, price, quantity);
        }
    }

    file.close();
    return loadedProducts;
}