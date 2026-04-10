#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class ApiClient : public QObject {
    Q_OBJECT

public:
    explicit ApiClient(QObject *parent = nullptr);

    void getInventory(const QString& query = "");
    void addProduct(const QString& json);
    void deleteProduct(int id);
    void restock(int id, int qty);
    void sell(int id, int qty);

signals:
    void inventoryReceived(const QString& data);
    void changed();
    void errorOccurred(const QString& message);

private:
    QNetworkAccessManager manager;
};