#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QLabel>
#include "api_client.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    ApiClient api;

    QTableWidget* table;

    QLineEdit* nameInput;
    QLineEdit* skuInput;
    QLineEdit* quantityInput;
    QLineEdit* priceInput;
    QLineEdit* categoryInput;

    

    QLabel* totalValueLabel;
    QLabel* lowStockLabel;

    void setupUI();
    void loadData(const QString& query = "");

private slots:
    void onDataReceived(QString data);

    void onAdd();
    void onDelete();
    void onRestock();
    void onSell();
    
};

#endif