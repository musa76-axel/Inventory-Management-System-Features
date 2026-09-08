#include "../include/FileHandler.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

void FileHandler::saveToCSV(const std::string& filename, const std::vector<Product>& items) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing.");
    }

    for (const auto& item : items) {
        file << item.getId() << ","
             << item.getName() << ","
             << item.getQuantity() << ","
             << item.getPrice() << "\n";
    }
    file.close();
}

std::vector<Product> FileHandler::loadFromCSV(const std::string& filename) {
    std::vector<Product> items;
    std::ifstream file(filename);
    if (!file.is_open()) {
        return items;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string idStr, name, quantityStr, priceStr;

        if (std::getline(ss, idStr, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, quantityStr, ',') &&
            std::getline(ss, priceStr, ',')) {

            int id = std::stoi(idStr);
            int quantity = std::stoi(quantityStr);
            double price = std::stod(priceStr);

            items.emplace_back(id, name, quantity, price);
        }
    }
    file.close();
    return items;
}