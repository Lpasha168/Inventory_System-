#include "database.h"
#include <iostream>

Database::Database() : db(nullptr) {}

Database::~Database() {
    close();
}

bool Database::open(const std::string& filename) {
    if (sqlite3_open(filename.c_str(), &db)) {
        std::cout << "Ошибка открытия БД\n";
        return false;
    }
    return true;
}

void Database::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool Database::execute(const std::string& sql) {
    char* errMsg = nullptr;

    if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg)) {
        std::cout << "SQL ошибка: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

sqlite3* Database::getDB() {
    return db;
}