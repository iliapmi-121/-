#ifndef BASKET_H
#define BASKET_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include <QByteArray>
#include <QDate>
#include <QDebug>
#include <QTime>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPixmap>
#include <QSqlQueryModel>
#include <QSqlTableModel>

#include "order.h"

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>
#include <QDebug>
#include <QModelIndexList>
#include <QItemSelectionModel>
#include <QTableWidgetItem>
#include <QStringList>

namespace Ui {
class Basket;
}

class Basket : public QDialog
{
    Q_OBJECT

public:
    explicit Basket(QWidget *parent = nullptr);
    void settingStyleSheet();
    ~Basket();

signals:
    void mainWindow();
    void sendDataToPoziciyaZakaza(QStringList* list);

public slots:
    void getModel(QStringList *list);
    void showCart();

private slots:
    void on_btnBack_clicked();

    void on_btnClear_clicked();

    void on_btnOrder_clicked();

private:
    Ui::Basket *ui;
    QSqlQueryModel* model;
    Order* order;
};

#endif // BASKET_H
