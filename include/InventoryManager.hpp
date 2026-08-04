#ifndef INVENTORYMANAGER_HPP
#define INVENTORYMANAGER_HPP

#include <vector>
#include "Exceptions.hpp"

template <typename T>
class InventoryManager {
private:
    std::vector<T> items;

public:
    InventoryManager() = default;
    ~InventoryManager() = default;

    void add(const T& item);
    bool removeById(int id);
    T findById(int id) const;
    void updateQuantity(int id, int newQty);

    const std::vector<T>& getAll() const { return items; }
};

#endif