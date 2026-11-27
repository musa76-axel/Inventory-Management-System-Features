#ifndef UICONSOLE_HPP
#define UICONSOLE_HPP

#include "InventoryManager.hpp"

class UIConsole 
{
private:
    I
nventoryManager& manager;

public:
    explicit UIConsole(InventoryManager& mgr);
    void run();
};

#endif
