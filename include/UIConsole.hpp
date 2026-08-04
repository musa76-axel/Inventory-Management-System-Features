#ifndef UICONSOLE_HPP
#define UICONSOLE_HPP

#include "InventoryManager.hpp"
#include "Product.hpp"

class UIConsole {
public:
    UIConsole() = default;
    void startMenu(InventoryManager<Product*>& manager);
};

#endif