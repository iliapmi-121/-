#include "pesnyacreating.h"
#include "ui_pesnyacreating.h"

PesnyaCreating::PesnyaCreating(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PesnyaCreating)
{
    ui->setupUi(this);

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT Nazvanie FROM Albom ORDER BY Nazvanie ASC");
    ui->comboBox->setModel(model);
}

PesnyaCreating::~PesnyaCreating()
{
    delete ui;
}

void PesnyaCreating::on_btnAddToDB_clicked()
{
    if (ui->txtNazvanie->text().isEmpty() || ui->timeEdit->text().isEmpty() || ui->txtJanr->text().isEmpty() || ui->priceSpinBox->text().isEmpty()) {
        QMessageBox::critical(this, "Ошибка", "Заполните все поля");
        return;
    }

    QSqlQuery get_albom_id;
    int id = 0;
    get_albom_id.prepare("SELECT Albom_id FROM Albom WHERE Nazvanie = :nazvanie");
    get_albom_id.bindValue(":nazvanie", ui->comboBox->currentText());

    if (get_albom_id.exec()) {


        if (get_albom_id.next()) {
            id = get_albom_id.value(0).toInt();
        }
        else {
            qDebug() << get_albom_id.lastError().text();
        }

    }
    else {
        qDebug() << get_albom_id.lastError().text();
    }

    get_albom_id.clear();


    QSqlQuery query;
    QString sql = "INSERT INTO Pesnya (Nazvanie, Albom_id, Dlitelnost, Janr, Price) VALUES (:nazvanie, :albom_id, :dlitelnost, :janr, :price)";
    query.prepare(sql);

    // Задаем значения для параметров в SQL-запросе
    query.bindValue(":nazvanie", ui->txtNazvanie->text());
    query.bindValue(":albom_id", id);
    query.bindValue(":dlitelnost", ui->timeEdit->text());
    query.bindValue(":janr", ui->txtJanr->text());
    query.bindValue(":price", ui->priceSpinBox->text());


    if (!query.exec()) {
       qDebug() << "Ошибка при вставке данных: " << query.lastError().text();
    }

    QMessageBox::information(this, "Успех", "Новая песня добавлена");
}


void PesnyaCreating::on_btnPrev_clicked()
{
    this->hide();
    emit mainWindow();
}

