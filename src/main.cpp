
#include "../include/UIConsole.hpp"

// --- PLACEHOLDER FUNCTIONS (to be replaced by teammates) ---
void addProduct(const std::string& name, double price, int quantity) {
    std::cout << "[PLACEHOLDER] Adding product: " << name 
              << " (Price: $" << price << ", Qty: " << quantity << ")" << std::endl;
}

void removeProduct(int productId) {
    std::cout << "[PLACEHOLDER] Removing product with ID: " << productId << std::endl;
}

void updateStock(int productId, int newQuantity) {
    std::cout << "[PLACEHOLDER] Updating stock for product ID " << productId 
              << " to " << newQuantity << std::endl;
}

void searchInventory(const std::string& searchTerm) {
    std::cout << "[PLACEHOLDER] Searching for: " << searchTerm << std::endl;
}

int main() {
    UI ui;
    ui.run();
    return 0;
}