 HEAD
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

 1cb1946 (feat: implement FileHandler CSV save and load methods)
    file.close();
}

std::vector<Product> FileHandler::loadFromCSV(const std::string& filename) {
 HEAD
    std::vector<Product> items;
    std::ifstream file(filename);
    if (!file.is_open()) {
        return items;
    }

    std::string line;

    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("FileHandler Error: Could not open file for reading: " + filename);
    }

    std::vector<Product> items;
    std::string line;

    // Skip the header row
    if (std::getline(file, line)) {
        // Header consumed
    }

 1cb1946 (feat: implement FileHandler CSV save and load methods)
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
 HEAD
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
 1cb1946 (feat: implement FileHandler CSV save and load methods)
}