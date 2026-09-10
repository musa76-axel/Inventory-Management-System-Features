#ifndef UICONSOLE_HPP
#define UICONSOLE_HPP

#include "InventoryManager.hpp"

class UIConsole {
private:
    InventoryManager& manager;

public:
    explicit UIConsole(InventoryManager& mgr);
    void run();
};

#endif