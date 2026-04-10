#include "api_client.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

ApiClient::ApiClient(QObject *parent) : QObject(parent) {}

void ApiClient::getInventory(const QString& query)
{
    QUrl url("http://localhost:8080/inventory");

    if (!query.isEmpty()) {
        QUrlQuery q;
        q.setQuery(query);
        url.setQuery(q);
    }

    QNetworkReply* reply = manager.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray data = reply->readAll();

        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(reply->errorString());
            reply->deleteLater();
            return;
        }

        emit inventoryReceived(QString::fromUtf8(data));
        reply->deleteLater();
    });
}

void ApiClient::addProduct(const QString& json)
{
    QNetworkRequest req(QUrl("http://localhost:8080/inventory"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = manager.post(req, json.toUtf8());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        reply->deleteLater();
        emit changed();
    });
}

void ApiClient::deleteProduct(int id)
{
    QNetworkRequest req(QUrl("http://localhost:8080/inventory/" + QString::number(id)));

    QNetworkReply* reply = manager.deleteResource(req);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        reply->deleteLater();
        emit changed();
    });
}

void ApiClient::restock(int id, int qty)
{
    QUrl url("http://localhost:8080/inventory/" + QString::number(id) + "/restock");

    QUrlQuery q;
    q.addQueryItem("quantity", QString::number(qty));
    url.setQuery(q);

    QNetworkReply* reply = manager.post(QNetworkRequest(url), "");

    connect(reply, &QNetworkReply::finished, this, [=]() {
        reply->deleteLater();
        emit changed();
    });
}

void ApiClient::sell(int id, int qty)
{
    QUrl url("http://localhost:8080/inventory/" + QString::number(id) + "/sell");

    QUrlQuery q;
    q.addQueryItem("quantity", QString::number(qty));
    url.setQuery(q);

    QNetworkReply* reply = manager.post(QNetworkRequest(url), "");

    connect(reply, &QNetworkReply::finished, this, [=]() {
        reply->deleteLater();
        emit changed();
    });
}