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
template <typename T>
void InventoryManager<T>::add(const T& item)
{
    for (const auto& existingItem : items)
    {
        if (existingItem.getId() == item.getId())
        {
            throw DuplicateIDException("Product ID already exists.");
        }
    }

    items.push_back(item);
}
template <typename T>
bool InventoryManager<T>::removeById(int id)
{
    for (auto it = items.begin(); it != items.end(); ++it)
    {
        if (it->getId() == id)
        {
            items.erase(it);
            return true;
        }
    }

    throw ProductNotFoundException("Product not found.");
}

#endif
