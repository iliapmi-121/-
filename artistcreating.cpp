#include "artistcreating.h"
#include "ui_artistcreating.h"

ArtistCreating::ArtistCreating(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArtistCreating)
{
    ui->setupUi(this);
}

ArtistCreating::~ArtistCreating()
{
    delete ui;
}

void ArtistCreating::on_btnOpenImg_clicked()
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


void ArtistCreating::on_btnAddToDB_clicked()
{
    if (ui->txtArtist->text().isEmpty() || ui->txtStrana->text().isEmpty() || ui->dateArtist->text().isEmpty() || ui->labelImg->pixmap().isNull()) {
        QMessageBox::critical(this, "Ошибка", "Заполните все поля");
        return;
    }

    QSqlQuery query;
    QString sql = "INSERT INTO Artist (Name, Data_rozdeniya, Strana, Image) VALUES (:name, :data_rozhdeniya, :strana, :image)";
    query.prepare(sql);

    // Задаем значения для параметров в SQL-запросе
    query.bindValue(":name", ui->txtArtist->text());
    query.bindValue(":data_rozhdeniya", ui->dateArtist->text());
    query.bindValue(":strana", ui->txtStrana->text());

    QByteArray byteArray;
    QPixmap pix(this->path);
    QBuffer buffer(&byteArray);

    if (!pix.save(&buffer, "PNG")) {
       qDebug() << "Error insert image";
    }

    query.bindValue(":image", QVariant(byteArray));

    if (!query.exec()) {
       qDebug() << "Ошибка при вставке данных: " << query.lastError().text();
    }

    QMessageBox::information(this, "Успех", "Новый артист добавлен");
 }


void ArtistCreating::on_btnPrev_clicked()
{
    this->hide();
    emit mainWindow();
}

