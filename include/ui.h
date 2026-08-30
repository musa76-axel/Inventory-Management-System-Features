#ifndef UI_H
#define UI_H

#include <iostream>
#include <string>
#include <limits>

class UI {
public:
    void displayMainMenu();
    int getMenuChoice();
    void run();
    
    void addProductUI();
    void updateStockUI();
    void removeProductUI();
    void searchInventoryUI();
    
private:
    int getIntInput(const std::string& prompt);
    double getDoubleInput(const std::string& prompt);
    std::string getStringInput(const std::string& prompt);
    void clearInputBuffer();
};

#endif