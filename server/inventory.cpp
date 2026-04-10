#include "inventory.h"
#include <iostream>

Inventory::Inventory(Database& database) : db(database) {}

bool Inventory::addProduct(const Product& p) {
    const char* sql =
        "INSERT INTO inventory (product_name, sku, quantity, price, category) "
        "VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db.getDB(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, p.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, p.sku.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, p.quantity);
    sqlite3_bind_double(stmt, 4, p.price);
    sqlite3_bind_text(stmt, 5, p.category.c_str(), -1, SQLITE_STATIC);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);
    return ok;
}

std::vector<Product> Inventory::getAll() {
    return filter("", "", false);
}

std::vector<Product> Inventory::filter(const std::string& name,
                                       const std::string& sku,
                                       bool lowStock) {
    std::vector<Product> list;

    std::string sql = "SELECT * FROM inventory WHERE 1=1 ";

    if (!name.empty())
        sql += "AND product_name LIKE ? ";

    if (!sku.empty())
        sql += "AND sku LIKE ? ";

    if (lowStock)
        sql += "AND quantity < 10 ";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db.getDB(), sql.c_str(), -1, &stmt, nullptr);

    int index = 1;

    if (!name.empty()) {
        std::string val = "%" + name + "%";
        sqlite3_bind_text(stmt, index++, val.c_str(), -1, SQLITE_STATIC);
    }

    if (!sku.empty()) {
        std::string val = "%" + sku + "%";
        sqlite3_bind_text(stmt, index++, val.c_str(), -1, SQLITE_STATIC);
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Product p;
        p.id = sqlite3_column_int(stmt, 0);
        p.name = (char*)sqlite3_column_text(stmt, 1);
        p.sku = (char*)sqlite3_column_text(stmt, 2);
        p.quantity = sqlite3_column_int(stmt, 3);
        p.price = sqlite3_column_double(stmt, 4);
        p.category = (char*)sqlite3_column_text(stmt, 5);

        list.push_back(p);
    }

    sqlite3_finalize(stmt);
    return list;
}

bool Inventory::deleteProduct(int id) {
    const char* sql = "DELETE FROM inventory WHERE id = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db.getDB(), sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);
    return ok;
}

bool Inventory::updateQuantity(int id, int newQuantity) {
    const char* sql = "UPDATE inventory SET quantity = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db.getDB(), sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, newQuantity);
    sqlite3_bind_int(stmt, 2, id);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);
    return ok;
}

bool Inventory::restock(int id, int amount) {
    const char* sql =
        "UPDATE inventory SET quantity = quantity + ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db.getDB(), sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, amount);
    sqlite3_bind_int(stmt, 2, id);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);
    return ok;
}

bool Inventory::sell(int id, int amount) {
    const char* sql =
        "UPDATE inventory SET quantity = quantity - ? WHERE id = ? AND quantity >= ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db.getDB(), sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, amount);
    sqlite3_bind_int(stmt, 2, id);
    sqlite3_bind_int(stmt, 3, amount);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);
    return ok;
}