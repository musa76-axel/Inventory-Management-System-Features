#include "FileHandler.hpp"
#include <fstream>
#include <sstream>

void FileHandler::saveToCSV(const std::string& filename, const std::vector<Product>& products) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (const auto& prod : products) {
        file << prod.getId() << ","
             << prod.getName() << ","
             << prod.getPrice() << ","
             << prod.getQuantity() << "\n";
    }
    file.close();
}

std::vector<Product> FileHandler::loadFromCSV(const std::string& filename) {
    std::vector<Product> products;
    std::ifstream file(filename);
    if (!file.is_open()) return products;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string idStr, name, priceStr, qtyStr;

        if (std::getline(ss, idStr, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, priceStr, ',') &&
            std::getline(ss, qtyStr)) {
            try {
                int id = std::stoi(idStr);
                double price = std::stod(priceStr);
                int qty = std::stoi(qtyStr);

                // Explicit constructor call prevents MSVC template conversion warnings
                products.push_back(Product(id, name, price, qty));
            } catch (...) {
                // Skip malformed lines
            }
        }
    }
    file.close();
    return products;
}