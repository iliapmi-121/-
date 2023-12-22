#include "albomcreating.h"
#include "ui_albomcreating.h"

AlbomCreating::AlbomCreating(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlbomCreating)
{
    ui->setupUi(this);

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT Name FROM Artist ORDER BY Name ASC");
    ui->comboBox->setModel(model);
}

AlbomCreating::~AlbomCreating()
{
    delete ui;
}

void AlbomCreating::on_btnOpenImg_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Выбрать изображение", "C:\\",
                                                "PNG Image (*.png);");
    this->path = path;

    if (path.isEmpty()) {
       QMessageBox::information(this, "Информация", "Файл не был выбран");
       return;
    }

    QPixmap pixmap(path);
    ui->labelImg->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio));
}


void AlbomCreating::on_btnAddToDB_clicked()
{
    if (ui->txtNazvanie->text().isEmpty() || ui->dateAlbom->text().isEmpty() || ui->labelImg->pixmap().isNull()) {
        QMessageBox::critical(this, "Ошибка", "Заполните все поля");
        return;
    }

    QSqlQuery get_artist_id;
    int id = 0;
    get_artist_id.prepare("SELECT Artist_id FROM Artist WHERE Name = :name");

    get_artist_id.bindValue(":name", ui->comboBox->currentText());

    if (get_artist_id.exec()) {


        if (get_artist_id.next()) {
            id = get_artist_id.value(0).toInt();
        }
        else {
            qDebug() << get_artist_id.lastError().text();
        }

    }
    else {
        qDebug() << get_artist_id.lastError().text();
    }

    get_artist_id.clear();


    QSqlQuery query;
    QString sql = "INSERT INTO Albom (Nazvanie, Data_vipuska, Artist_id, Image) VALUES (:nazvanie, :data_vipuska, :artist_id, :image)";
    query.prepare(sql);

    // Задаем значения для параметров в SQL-запросе
    query.bindValue(":nazvanie", ui->txtNazvanie->text());
    query.bindValue(":data_vipuska", ui->dateAlbom->text());

    QByteArray byteArray;
    QPixmap pix(this->path);
    QBuffer buffer(&byteArray);

    if (!pix.save(&buffer, "PNG")) {
       qDebug() << "Error insert image";
    }

    query.bindValue(":image", QVariant(byteArray));

    query.bindValue(":artist_id", id);

    if (!query.exec()) {
       qDebug() << "Ошибка при вставке данных: " << query.lastError().text();
    }

    QMessageBox::information(this, "Успех", "Новый альбом добавлен");
}


void AlbomCreating::on_btnPrev_clicked()
{
    this->hide();
    emit mainWindow();
}

