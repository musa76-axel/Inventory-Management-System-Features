#include <iostream>
#include "../include/InventoryManager.hpp"
#include "../include/UIConsole.hpp"

int main() {
    InventoryManager manager;
    UIConsole ui(manager);

    ui.run();

    return 0;
}