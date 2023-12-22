#ifndef ORDER_H
#define ORDER_H

#include <QDialog>
#include <QRegularExpression>
#include <QMessageBox>
#include <QValidator>
#include <QRegularExpressionValidator>
#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>
#include <QDate>
#include <QList>

namespace Ui {
class Order;
}

class Order : public QDialog
{
    Q_OBJECT

public:
    explicit Order(QWidget *parent = nullptr);
    ~Order();

signals:
    void mainWindow();

public slots:
    void getDataForPoziciyZakaza(QStringList *list);


private slots:
    void on_btnCancel_clicked();

    void on_btnOrder_clicked();

private:
    Ui::Order *ui;
    QValidator *phoneValidator;
    QValidator *emailValidator;
    void sendKlientIdToZakaz();
    void sendDataForPoziciyaZakaza();


    QStringList *names;
};

#endif // ORDER_H
