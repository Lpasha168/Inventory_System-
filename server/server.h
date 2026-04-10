#ifndef SERVER_H
#define SERVER_H

#include "database.h"
#include "inventory.h"

class Server {
private:
    Database db;
    Inventory* inventory;

public:
    Server();
    void start(int port);
};

#endif