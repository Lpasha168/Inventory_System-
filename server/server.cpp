#include "server.h"
#include "logger.h"
#include <iostream>
#include "httplib.h"
#include "json.hpp"
using json = nlohmann::json;

Server::Server() {
    db.open("inventory.db");

    db.execute(
        "CREATE TABLE IF NOT EXISTS inventory ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "product_name TEXT,"
        "sku TEXT UNIQUE,"
        "quantity INTEGER,"
        "price REAL,"
        "category TEXT,"
        "last_updated DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");"
    );

    inventory = new Inventory(db);
}

void Server::start(int port) {
    httplib::Server svr;

    
    svr.Get("/inventory", [&](const httplib::Request& req, httplib::Response& res) {

    std::string name = req.has_param("name") ? req.get_param_value("name") : "";
    std::string sku = req.has_param("sku") ? req.get_param_value("sku") : "";
    bool lowStock = req.has_param("low_stock");

    Logger::log("GET /inventory");

    auto list = inventory->filter(name, sku, lowStock);

    json arr = json::array();

    for (auto& p : list) {
        arr.push_back({
            {"id", p.id},
            {"name", p.name},
            {"sku", p.sku},
            {"quantity", p.quantity},
            {"price", p.price},
            {"category", p.category}
        });
    }

    res.set_content(arr.dump(), "application/json");
});

    
    svr.Post("/inventory", [&](const httplib::Request& req, httplib::Response& res) {

        Logger::log("POST /inventory");

        try {
            auto body = req.body;

            auto get = [&](std::string key) {
                size_t pos = body.find(key);
                size_t start = body.find(":", pos) + 1;
                size_t end = body.find(",", start);
                return body.substr(start, end - start);
            };

            Product p;
            p.name = get("product_name");
            p.sku = get("sku");
            p.quantity = std::stoi(get("quantity"));
            p.price = std::stod(get("price"));
            p.category = get("category");

            inventory->addProduct(p);

            res.set_content("OK", "text/plain");
        }
        catch (...) {
            res.status = 400;
        }
    });

    
    svr.Delete(R"(/inventory/(\d+))", [&](const httplib::Request& req, httplib::Response& res) {

        int id = std::stoi(req.matches[1]);
        Logger::log("DELETE id=" + std::to_string(id));

        if (inventory->deleteProduct(id)) {
            res.set_content("Deleted", "text/plain");
        } else {
            res.status = 500;
        }
    });

    
    svr.Post(R"(/inventory/(\d+)/restock)", [&](const httplib::Request& req, httplib::Response& res) {

        int id = std::stoi(req.matches[1]);
        int qty = std::stoi(req.get_param_value("quantity"));

        Logger::log("RESTOCK id=" + std::to_string(id));

        inventory->restock(id, qty);

        res.set_content("OK", "text/plain");
    });

    
    svr.Post(R"(/inventory/(\d+)/sell)", [&](const httplib::Request& req, httplib::Response& res) {

        int id = std::stoi(req.matches[1]);
        int qty = std::stoi(req.get_param_value("quantity"));

        Logger::log("SELL id=" + std::to_string(id));
        if (inventory->sell(id, qty)) {
            res.set_content("OK", "text/plain");
        } else {
            res.status = 400;
            res.set_content("Not enough stock", "text/plain");
        }
    });

    std::cout << "Сервер запущен на порту " << port << "\n";
    svr.listen("0.0.0.0", port);
}
