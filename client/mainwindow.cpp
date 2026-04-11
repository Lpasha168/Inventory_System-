#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QInputDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTableWidget>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();

    
    connect(&api, &ApiClient::inventoryReceived,
            this, &MainWindow::onDataReceived);

    
    connect(&api, &ApiClient::changed,
            this, [=]() {
                api.getInventory();
            });

    
    connect(&api, &ApiClient::errorOccurred,
            this, [=](const QString& err) {
                QMessageBox::critical(this, "API Error", err);
            });

    
    api.getInventory();
}

void MainWindow::setupUI()
{
    QWidget* central = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;

    table = new QTableWidget;
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"ID","Name","SKU","Qty","Price"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    nameInput = new QLineEdit;
    nameInput->setPlaceholderText("Name");

    skuInput = new QLineEdit;
    skuInput->setPlaceholderText("SKU");

    quantityInput = new QLineEdit;
    quantityInput->setPlaceholderText("Quantity");

    priceInput = new QLineEdit;
    priceInput->setPlaceholderText("Price");

    categoryInput = new QLineEdit;
    categoryInput->setPlaceholderText("Category");

    QPushButton* addBtn = new QPushButton("Add");
    QPushButton* delBtn = new QPushButton("Delete");
    QPushButton* restockBtn = new QPushButton("Restock");
    QPushButton* sellBtn = new QPushButton("Sell");

    // кнопки
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAdd);
    connect(delBtn, &QPushButton::clicked, this, &MainWindow::onDelete);
    connect(restockBtn, &QPushButton::clicked, this, &MainWindow::onRestock);
    connect(sellBtn, &QPushButton::clicked, this, &MainWindow::onSell);

    // layout
    layout->addWidget(table);
    layout->addWidget(nameInput);
    layout->addWidget(skuInput);
    layout->addWidget(quantityInput);
    layout->addWidget(priceInput);
    layout->addWidget(categoryInput);

    layout->addWidget(addBtn);
    layout->addWidget(delBtn);
    layout->addWidget(restockBtn);
    layout->addWidget(sellBtn);

    central->setLayout(layout);
    setCentralWidget(central);
}

void MainWindow::onDataReceived(QString data)
{
    qDebug() << "RAW JSON:" << data;

    table->setRowCount(0);

    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    if (!doc.isArray()) {
        qDebug() << "ERROR: not array";
        return;
    }

    QJsonArray arr = doc.array();

    double total = 0;
    int lowStockCount = 0;

    for (int i = 0; i < arr.size(); i++) {
        QJsonObject o = arr[i].toObject();

        int id = o["id"].toInt();
        QString name = o["name"].toString();
        QString sku = o["sku"].toString();
        int quantity = o["quantity"].toInt();
        double price = o["price"].toDouble();

        table->insertRow(i);

        table->setItem(i, 0, new QTableWidgetItem(QString::number(id)));
        table->setItem(i, 1, new QTableWidgetItem(name));
        table->setItem(i, 2, new QTableWidgetItem(sku));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(quantity)));
        table->setItem(i, 4, new QTableWidgetItem(QString::number(price)));

        total += quantity * price;

        if (quantity < 10) {
            table->item(i, 3)->setBackground(Qt::red);
            lowStockCount++;
        }
    }

    qDebug() << "Total value:" << total;
    qDebug() << "Low stock:" << lowStockCount;
}



void MainWindow::onAdd()
{
    QString json =
        "{"
        "\"product_name\":\"" + nameInput->text() + "\"," +
        "\"sku\":\"" + skuInput->text() + "\"," +
        "\"quantity\":" + quantityInput->text() + "," +
        "\"price\":" + priceInput->text() + "," +
        "\"category\":\"" + categoryInput->text() + "\""
        "}";

    api.addProduct(json);
}

void MainWindow::onDelete()
{
    int r = table->currentRow();
    if (r < 0) return;

    api.deleteProduct(table->item(r, 0)->text().toInt());
}

void MainWindow::onRestock()
{
    int r = table->currentRow();
    if (r < 0) return;

    int id = table->item(r, 0)->text().toInt();
    int qty = QInputDialog::getInt(this, "Restock", "Quantity:");

    api.restock(id, qty);
}

void MainWindow::onSell()
{
    int r = table->currentRow();
    if (r < 0) return;

    int id = table->item(r, 0)->text().toInt();
    int qty = QInputDialog::getInt(this, "Sell", "Quantity:");

    api.sell(id, qty);
}
