#include "../include/UIConsole.hpp"
#include "../include/Exceptions.hpp"
#include <iostream>
#include <limits>
#include <string>

UIConsole::UIConsole(InventoryManager& mgr) : manager(mgr) {}

void UIConsole::run() {
    // Attempt loading existing file on startup
    try {
        manager.loadFromFile("inventory.csv");
        std::cout << "[INFO] Loaded existing inventory data.\n";
    } catch (...) {
        std::cout << "[INFO] Starting with an empty inventory.\n";
    }

    int choice = 0;
    while (choice != 5) {
        std::cout << "\n=================================\n";
        std::cout << "  INVENTORY MANAGEMENT SYSTEM    \n";
        std::cout << "=================================\n";
        std::cout << "1. Add Product\n";
        std::cout << "2. Display All Products\n";
        std::cout << "3. Find Product by ID\n";
        std::cout << "4. Save & Exit\n";
        std::cout << "5. Exit Without Saving\n";
        std::cout << "Enter choice: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a valid number.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                int id, qty;
                std::string name;
                double price;

                std::cout << "Enter Product ID: ";
                std::cin >> id;
                std::cout << "Enter Product Name: ";
                std::cin.ignore(10000, '\n');
                std::getline(std::cin, name);
                std::cout << "Enter Quantity: ";
                std::cin >> qty;
                std::cout << "Enter Price: ";
                std::cin >> price;

                try {
                    manager.addProduct(Product(id, name, qty, price));
                    std::cout << "[SUCCESS] Product added.\n";
                } catch (const std::exception& e) {
                    std::cout << "[ERROR] " << e.what() << "\n";
                }
                break;
            }
            
case 2: {
            const auto& products = manager.getAllProducts();
            if (products.empty()) {
                std::cout << "[INFO] Inventory is currently empty.\n";
            } else {
                std::cout << "\n--- Current Inventory ---\n";
                for (const auto& p : products) {
                    std::cout << "ID: " << p.getId()
                              << " | Name: " << p.getName()
                              << " | Qty: " << p.getQuantity()
                              << " | Price: $" << p.getPrice() << "\n";
                }
            }
            break;
        }

            case 3: {
                int id;
                std::cout << "Enter Product ID to search: ";
                std::cin >> id;
                try {
                    Product* p = manager.findProduct(id);
                    std::cout << "[FOUND] ID: " << p->getId()
                              << " | Name: " << p->getName()
                              << " | Qty: " << p->getQuantity()
                              << " | Price: $" << p->getPrice() << "\n";
                } catch (const std::exception& e) {
                    std::cout << "[ERROR] " << e.what() << "\n";
                }
                break;
            }
            case 4:
                try {
                    manager.saveToFile("inventory.csv");
                    std::cout << "[SUCCESS] Data saved to inventory.csv. Exiting...\n";
                    return;
                } catch (const std::exception& e) {
                    std::cout << "[ERROR] Could not save data: " << e.what() << "\n";
                }
                break;
            case 5:
                std::cout << "Exiting without saving changes...\n";
                break;
            default:
                std::cout << "Invalid choice. Please enter a number between 1 and 5.\n";
                break;
        }
    }
}