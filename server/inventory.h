#ifndef INVENTORY_H
#define INVENTORY_H

#include "database.h"
#include <vector>
#include <string>

struct Product {
    int id;
    std::string name;
    std::string sku;
    int quantity;
    double price;
    std::string category;
};

class Inventory {
private:
    Database& db;

public:
    Inventory(Database& database);

    bool addProduct(const Product& p);

    std::vector<Product> getAll();
    std::vector<Product> filter(const std::string& name,
                                const std::string& sku,
                                bool lowStock);

    bool deleteProduct(int id);
    bool updateQuantity(int id, int newQuantity);

    bool restock(int id, int amount);
    bool sell(int id, int amount);
};

#endif