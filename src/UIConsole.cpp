#include "../include/UIConsole.hpp"
#include <iomanip>

// --- Placeholder Functions (will be replaced by teammates) ---
void addProduct(const std::string& name, double price, int quantity);
void removeProduct(int productId);
void updateStock(int productId, int newQuantity);
void searchInventory(const std::string& searchTerm);

// --- UI Implementation ---

void UIConsole::displayMainMenu() {
    std::cout << "\n" << std::string(40, '=') << std::endl;
    std::cout << "     INVENTORY MANAGEMENT SYSTEM" << std::endl;
    std::cout << std::string(40, '=') << std::endl;
    std::cout << "1. Add Products" << std::endl;
    std::cout << "2. Update Stock" << std::endl;
    std::cout << "3. Remove Products" << std::endl;
    std::cout << "4. Search Inventory" << std::endl;
    std::cout << "5. Exit" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
}

int UIConsole::getMenuChoice() {
    while (true) {
        std::cout << "Enter your choice (1-5): ";
        int choice;
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            clearInputBuffer();
            std::cout << "Invalid input. Please enter a number." << std::endl;
        } else if (choice >= 1 && choice <= 5) {
            clearInputBuffer();
            return choice;
        } else {
            std::cout << "Invalid choice. Please enter a number between 1 and 5." << std::endl;
        }
    }
}

void UIConsole::run() {
    std::cout << "\nInventory Management System Started!" << std::endl;
    
    while (true) {
        displayMainMenu();
        int choice = getMenuChoice();
        
        switch (choice) {
            case 1:
                addProductUI();
                break;
            case 2:
                updateStockUI();
                break;
            case 3:
                removeProductUI();
                break;
            case 4:
                searchInventoryUI();
                break;
            case 5:
                std::cout << "\nExiting Inventory Management System. Goodbye!" << std::endl;
                return;
            default:
                break;
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
        clearInputBuffer();
    }
}

void UIConsole::addProductUI() {
    std::cout << "\nADD NEW PRODUCT" << std::endl;
    std::cout << std::string(30, '-') << std::endl;
    
    std::string name = getStringInput("Enter product name: ");
    double price = getDoubleInput("Enter product price: $");
    int quantity = getIntInput("Enter product quantity: ");
    
    addProduct(name, price, quantity);
    std::cout << "Product added successfully!" << std::endl;
}

void UIConsole::updateStockUI() {
    std::cout << "\nUPDATE STOCK" << std::endl;
    std::cout << std::string(30, '-') << std::endl;
    
    int productId = getIntInput("Enter product ID to update: ");
    int newQuantity = getIntInput("Enter new quantity: ");
    
    updateStock(productId, newQuantity);
    std::cout << "Stock updated successfully!" << std::endl;
}

void UIConsole::removeProductUI() {
    std::cout << "\nREMOVE PRODUCT" << std::endl;
    std::cout << std::string(30, '-') << std::endl;
    
    int productId = getIntInput("Enter product ID to remove: ");
    
    std::cout << "Are you sure you want to remove product ID " << productId << "? (y/n): ";
    std::string confirm;
    std::cin >> confirm;
    clearInputBuffer();
    
    if (confirm == "y" || confirm == "Y" || confirm == "yes" || confirm == "YES") {
        removeProduct(productId);
        std::cout << "Product removed successfully!" << std::endl;
    } else {
        std::cout << "Deletion cancelled." << std::endl;
    }
}

void UIConsole::searchInventoryUI() {
    std::cout << "\nSEARCH INVENTORY" << std::endl;
    std::cout << std::string(30, '-') << std::endl;
    
    std::string searchTerm = getStringInput("Enter search term (name or ID): ");
    searchInventory(searchTerm);
}

int UIConsole::getIntInput(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        int value;
        std::cin >> value;
        
        if (std::cin.fail()) {
            std::cin.clear();
            clearInputBuffer();
            std::cout << "Invalid input. Please enter a valid integer." << std::endl;
        } else {
            clearInputBuffer();
            return value;
        }
    }
}

double UIConsole::getDoubleInput(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        double value;
        std::cin >> value;
        
        if (std::cin.fail()) {
            std::cin.clear();
            clearInputBuffer();
            std::cout << "Invalid input. Please enter a valid number." << std::endl;
        } else if (value < 0) {
            std::cout << "Price cannot be negative. Please enter a positive number." << std::endl;
        } else {
            clearInputBuffer();
            return value;
        }
    }
}

std::string UIConsole::getStringInput(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    
    while (value.empty()) {
        std::cout << "Input cannot be empty. Please enter a value: ";
        std::getline(std::cin, value);
    }
    
    return value;
}

void UIConsole::clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
