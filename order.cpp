#include "order.h"
#include "ui_order.h"

Order::Order(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Order)
{
    ui->setupUi(this);

    QRegularExpression phoneRegex("^8\\d{10}$");
    phoneValidator = new QRegularExpressionValidator(phoneRegex, this);
    ui->txtPhone->setValidator(phoneValidator);

    QRegularExpression emailRegex("^[a-zA-Z0-9._%-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}");
    emailValidator = new QRegularExpressionValidator(emailRegex, this);
    ui->txtEmail->setValidator(emailValidator);
}

Order::~Order()
{
    delete ui;
}

void Order::getDataForPoziciyZakaza(QStringList *list)
{
    names = new QStringList;
    for (int i = 0; i < list->size(); ++i) {
        names->append(list->at(i));
        qDebug() << list->at(i);
    }


}

void Order::on_btnCancel_clicked()
{
    this->hide();
    emit mainWindow();
}


void Order::on_btnOrder_clicked()
{

    QString text = ui->txtEmail->text();
    int pos = 0;

    if (ui->txtName->text().isEmpty() || ui->txtPhone->text().isEmpty() || ui->txtEmail->text().isEmpty()) {
        QMessageBox::critical(this, "Ошибка", "Все поля должны быть заполнены");
    }
    else if (emailValidator->validate(text, pos) != QValidator::Acceptable) {
        QMessageBox::critical(this, "Ошибка", "Неверный формат почты");
      }
    else {
        // Создаем SQL-запрос для вставки данных
        QSqlQuery query;

        QString sql = "INSERT INTO Klient (Name, Telefon, Adres_mail) VALUES (:name, :phone, :email)";
        query.prepare(sql);

        // Задаем значения для параметров в SQL-запросе
        query.bindValue(":name", ui->txtName->text());
        query.bindValue(":phone", ui->txtPhone->text());
        query.bindValue(":email", ui->txtEmail->text());

        // Выполняем SQL-запрос
        if (!query.exec()) {
           qDebug() << "Ошибка при вставке данных: " << query.lastError().text();
        }

        query.clear();

        this->sendKlientIdToZakaz();
        this->sendDataForPoziciyaZakaza();

        QMessageBox::warning(this, "Спасибо за покупку", "Заказ успешно оформлен");
        this->hide();
        emit mainWindow();
    }

}

void Order::sendKlientIdToZakaz()
{

    QVariant klient_id;
    QSqlQuery query_klient_get;

    if (query_klient_get.exec("SELECT Klient_id FROM Klient ORDER BY Klient_id DESC LIMIT 1")) {

        if (query_klient_get.next()) {
            klient_id = query_klient_get.value(0);
        }
    }

    query_klient_get.clear();

    // Создаем SQL-запрос для вставки данных
    QSqlQuery query_zakaz_send;
    QString sql2 = "INSERT INTO Zakaz (Data_zakaza, Klient_id) VALUES (:data_zakaza, :klient_id)";
    query_zakaz_send.prepare(sql2);

    // Задаем значения для параметров в SQL-запросе
    query_zakaz_send.bindValue(":data_zakaza", QDate::currentDate().toString("yyyy-MM-dd"));
    query_zakaz_send.bindValue(":klient_id", klient_id);

    // Выполняем SQL-запрос
    if (!query_zakaz_send.exec()) {
       qDebug() << "Ошибка при вставке данных: " << query_zakaz_send.lastError().text();
    }
    query_zakaz_send.clear();
}

void Order::sendDataForPoziciyaZakaza()
{
    QVariant zakaz_id;
    QSqlQuery query_zakaz_get;
    if (query_zakaz_get.exec("SELECT Zakaz_id FROM Zakaz ORDER BY Zakaz_id DESC LIMIT 1")) {

        if (query_zakaz_get.next()) {
            zakaz_id = query_zakaz_get.value(0);
        }
    }
    query_zakaz_get.clear();

    for (int i = 0; i < names->size(); i+=2) {

        QSqlQuery queryAlbomName, queryPesnyaName;
        int albom_id, pesnya_id;

        queryAlbomName.prepare("SELECT Albom_id FROM Albom WHERE Nazvanie = :0 LIMIT 1");
        queryAlbomName.addBindValue(names->at(i));

        if (queryAlbomName.exec()) {
            if (queryAlbomName.next()) {
                albom_id = queryAlbomName.value(0).toInt();
            }
            else {
                qDebug() << queryAlbomName.lastError().text();
            }
        }
        else {
            qDebug() << queryAlbomName.lastError().text();
        }

        queryAlbomName.clear();

        queryPesnyaName.prepare("SELECT Pesnya_id FROM Pesnya WHERE Nazvanie = :0 LIMIT 1");
        queryPesnyaName.addBindValue(names->at(i+1));

        if (queryPesnyaName.exec()) {
            if (queryPesnyaName.next()) {
                pesnya_id = queryPesnyaName.value(0).toInt();
            }
            else {
                qDebug() << queryPesnyaName.lastError().text();
            }
        }
        else {
            qDebug() << queryPesnyaName.lastError().text();
        }

        queryPesnyaName.clear();



        // Создаем SQL-запрос для вставки данных
        QSqlQuery query_poziciy_send;
        QString sql2 = "INSERT INTO Poziciy_zakaza (Zakaz_id, Albom_id, Pesnya_id) VALUES (:zakaz, :albom, :pesnya)";
        query_poziciy_send.prepare(sql2);

        // Задаем значения для параметров в SQL-запросе
        query_poziciy_send.bindValue(":zakaz", zakaz_id.toInt());
        query_poziciy_send.bindValue(":albom", albom_id);
        query_poziciy_send.bindValue(":pesnya", pesnya_id);

        // Выполняем SQL-запрос
        if (!query_poziciy_send.exec()) {
           qDebug() << "Ошибка при вставке данных: " << query_poziciy_send.lastError().text();
        }
        query_poziciy_send.clear();

    }



}

