#ifndef USERSVIEW_H
#define USERSVIEW_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>
#include <QDebug>
#include <QModelIndexList>
#include <QItemSelectionModel>
#include <QStringList>
#include <QByteArray>
#include <QDate>
#include <QDebug>
#include <QTime>
#include <QString>

#include "basket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UsersView; }
QT_END_NAMESPACE

class UsersView : public QDialog
{
    Q_OBJECT

public:
    explicit UsersView(QWidget *parent = nullptr);
    void handleUserLogin();
    void settingStyleSheet();
    void setTableView(QSqlQuery query);
    ~UsersView();
signals:
    void sendModel(QStringList* list);
    void showCart();

private slots:
    void on_btnAdd_clicked();
    void on_btnBasket_clicked();

private:
    Ui::UsersView *ui;
    Basket* basket;
    QSqlQueryModel* model;
};

#endif // USERSVIEW_H
