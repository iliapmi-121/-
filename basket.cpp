#include "basket.h"
#include "ui_basket.h"

#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QBuffer>

Basket::Basket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Basket)
{
    ui->setupUi(this);

    order = new Order;

    connect(order, &Order::mainWindow, this, &Basket::show);
    connect(this, &Basket::sendDataToPoziciyaZakaza, order, &Order::getDataForPoziciyZakaza);

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    settingStyleSheet();


}

void Basket::settingStyleSheet()
{

    ui->btnBack->setStyleSheet(
       "QPushButton {"
           "background-color: rgb(0, 170, 127);"
           "font-size: 20px;"
           "border: 2px solid black;"
           "padding: 5px;"
       "}"
       "QPushButton:hover {"
           "background-color: green;"
           "font-size: 20px;"
           "border: 2px solid black;"
           "padding: 5px;"
       "}"
    );
    ui->btnOrder->setStyleSheet(
       "QPushButton {"
           "background-color: rgb(111, 159, 173);"
           "font-size: 20px;"
           "border: 2px solid black;"
           "padding: 5px;"
       "}"
       "QPushButton:hover {"
           "background-color: rgb(66, 118, 133);"
           "font-size: 20px;"
           "border: 2px solid black;"
           "padding: 5px;"
       "}"
    );
    ui->btnClear->setStyleSheet(
       "QPushButton {"
           "background-color: rgb(230, 70, 70);"
           "font-size: 20px;"
           "border: 2px solid black;"
           "padding: 5px;"
       "}"
       "QPushButton:hover {"
           "background-color: rgb(180, 70, 70);"
           "font-size: 20px;"
           "border: 2px solid black;"
           "padding: 5px;"
       "}"
    );

}


Basket::~Basket()
{
    delete ui;

}

void Basket::getModel(QStringList* list)
{

    int currentRowCount = ui->tableView->rowCount();


    int numberOfMatches = 0;

    // Check if the data already exists in the table
    for (int i = 0; i < currentRowCount; i++) {
      for (int j = 0; j < list->size(); j++) {
          QTableWidgetItem* item = ui->tableView->item(i, j);
          if (item && item->text() == list->at(j)) {
              numberOfMatches++;
          }

          if (numberOfMatches == 10) {
              QMessageBox::warning(this, "Error", "This item is already in your cart");
              return;
          }
      }
    }

    ui->tableView->setRowCount(currentRowCount + 1);

    // Set the data for each cell in the new row
    for (int i = 0; i < list->size(); ++i) {
        QTableWidgetItem *item = new QTableWidgetItem(list->at(i));
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableView->setItem(currentRowCount, i, item);
    }

    QByteArray blobData = list->at(3).toUtf8();
    QPixmap pixmap;

    QPixmap pixmap2(":/Images/unknown.png");
    bool loaded = pixmap.loadFromData(blobData, "PNG");
    if (!loaded) {
        pixmap = pixmap2;
        qDebug() << "Failed to load image from BLOB data";
    }
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setIcon(QIcon(pixmap));
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableView->setItem(currentRowCount, 3, item);

    blobData = list->at(6).toUtf8();
    loaded = pixmap.loadFromData(blobData);
    if (!loaded) {
        pixmap = pixmap2;
        qDebug() << "Failed to load image from BLOB data";
    }
    item = new QTableWidgetItem();
    item->setIcon(QIcon(pixmap));
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableView->setItem(currentRowCount, 6, item);

    if (currentRowCount > 1) {
        if (ui->tableView->item(0, 0)->text() == ui->tableView->item(1, 0)->text() && ui->tableView->item(0, 4)->text() == ui->tableView->item(1, 4)->text()
                && ui->tableView->item(0, 7)->text() == ui->tableView->item(1, 7)->text()) {
            ui->tableView->removeRow(0);
        }
    }

}

void Basket::showCart()
{
    int currentRowCount = ui->tableView->rowCount();
    if (currentRowCount > 1) {
        if (ui->tableView->item(0, 0)->text() == ui->tableView->item(1, 0)->text() && ui->tableView->item(0, 4)->text() == ui->tableView->item(1, 4)->text()
                && ui->tableView->item(0, 7)->text() == ui->tableView->item(1, 7)->text()) {
            ui->tableView->removeRow(0);
        }
    }
}

void Basket::on_btnBack_clicked()
{
    this->hide();
    emit mainWindow();
}


void Basket::on_btnClear_clicked()
{
    ui->tableView->setRowCount(0);
}


void Basket::on_btnOrder_clicked()
{
    int currentRowCount = ui->tableView->rowCount();

    if (currentRowCount >= 1) {
        QStringList* list = new QStringList;
        int rowCount = ui->tableView->rowCount();
        for(int i = 0; i < rowCount; ++i)
        {

            QTableWidgetItem *item = ui->tableView->item(i, 4);
            if(item)
            {
                list->append(item->text());
            }

            item = ui->tableView->item(i, 7);
            if(item)
            {
                list->append(item->text());
            }

        }
        qDebug() << list->size();
        emit sendDataToPoziciyaZakaza(list);
        order->show();
        this->hide();
    }

    else {
        QMessageBox::critical(this, "Ошибка", "Корзина не может быть пуста");
    }
}

