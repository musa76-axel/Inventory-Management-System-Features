#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

class InventoryException : public std::runtime_error {
public:
    explicit InventoryException(const std::string& msg) : std::runtime_error(msg) {}
};

class ProductNotFoundException : public InventoryException {
public:
    explicit ProductNotFoundException(const std::string& msg) : InventoryException(msg) {}
};

class DuplicateIDException : public InventoryException {
public:
    explicit DuplicateIDException(const std::string& msg) : InventoryException(msg) {}
};

class InvalidQuantityException : public InventoryException {
public:
    explicit InvalidQuantityException(const std::string& msg) : InventoryException(msg) {}
};

#endif