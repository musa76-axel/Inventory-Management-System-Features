#pragma once

#include <string>
#include <vector>
#include "Product.hpp"

class FileHandler {
public:
    // Saves the current product inventory list to a CSV file
    static void saveToCSV(const std::string& filename, const std::vector<Product>& products);

    // Loads products from a CSV file into a vector
    static std::vector<Product> loadFromCSV(const std::string& filename);
};