#include "updatingview.h"
#include "ui_updatingview.h"

UpdatingView::UpdatingView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdatingView)
{
    ui->setupUi(this);
}

UpdatingView::~UpdatingView()
{
    delete ui;
}

void UpdatingView::getSelectedData(QStringList list)
{
    ui->txtArtist->setText(list.at(0));
    currentArtistName = list.at(0);

    ui->txtStrana->setText(list.at(2));

    QDate date = QDate::fromString(list.at(1), "yyyy-MM-dd");
    ui->dateArtist->setDate(date);

    QByteArray blobData = list.at(3).toUtf8();
    QPixmap pixmap;
    QPixmap pixmap2(":/Images/unknown.png");
    bool loaded = pixmap.loadFromData(blobData, "PNG");
    if (!loaded) {
        pixmap = pixmap2;
        qDebug() << "Failed to load image from BLOB data";
    }
    ui->imgArtist->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio));

    ui->txtAlbom->setText(list.at(4));
    currentAlbomName = list.at(4);

    date = QDate::fromString(list.at(5), "yyyy-MM-dd");
    ui->dateArtist->setDate(date);

    ui->dateAlbom->setDate(date);

    QByteArray blobData2 = list.at(6).toUtf8();
    QPixmap pixmap3;
    QPixmap pixmap4(":/Images/unknown.png");
    loaded = pixmap3.loadFromData(blobData2, "PNG");
    if (!loaded) {
        pixmap3 = pixmap4;
        qDebug() << "Failed to load image from BLOB data";
    }
    ui->imgArtist->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio));

    ui->txtPesnya->setText(list.at(7));
    currentPesnyaName = list.at(7);

    QTime time = QTime::fromString(list.at(8), "hh:mm:ss");
    ui->timePesnya->setTime(time);

    ui->txtjanr->setText(list.at(9));

    ui->spinPrice->setValue(list.at(10).toDouble());

}

void UpdatingView::on_pushButton_2_clicked()
{
    this->hide();
    emit mainWindow();
}


void UpdatingView::on_btnImgArtist_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Выбрать изображение", "C:\\",
                                                "PNG Image (*.png);");
    this->path_artist = path;

    if (path.isEmpty()) {
       QMessageBox::information(this, "Информация", "Файл не был выбран");
       return;
    }

    QPixmap pixmap(path);
    ui->imgArtist->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio));
}


void UpdatingView::on_btnImgAlbom_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "Выбрать изображение", "C:\\",
                                                "PNG Image (*.png);");
    this->path_albom = path;

    if (path.isEmpty()) {
       QMessageBox::information(this, "Информация", "Файл не был выбран");
       return;
    }

    QPixmap pixmap(path);
    ui->imgAlbom->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio));
}

void UpdatingView::on_btnUpdate_clicked()
{
    if (ui->txtArtist->text().isEmpty() || ui->txtStrana->text().isEmpty() || ui->dateArtist->text().isEmpty() || ui->imgArtist->pixmap().isNull()
            || ui->imgAlbom->pixmap().isNull() || ui->txtAlbom->text().isEmpty() || ui->dateAlbom->text().isEmpty() ||
            ui->txtPesnya->text().isEmpty() || ui->timePesnya->text().isEmpty() || ui->txtjanr->text().isEmpty() || ui->spinPrice->text().isEmpty())
    {
        QMessageBox::critical(this, "Ошибка", "Заполните все поля");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT Artist.Artist_id, Albom.Albom_id, Pesnya.Pesnya_id "
                  "FROM Artist "
                  "INNER JOIN Albom ON Artist.Artist_id = Albom.Artist_id "
                  "INNER JOIN Pesnya ON Albom.Albom_id = Pesnya.Albom_id "
                  "WHERE Artist.Name = :0 AND Albom.Nazvanie = :1 AND Pesnya.Nazvanie = :2");

    query.addBindValue(currentArtistName);
    query.addBindValue(currentAlbomName);
    query.addBindValue(currentPesnyaName);

    qDebug() << ui->txtArtist->text() << ui->txtAlbom->text() << ui->txtPesnya->text();


    int val1, val2, val3;

    if (query.exec()) {

        if (query.first()) {

            qDebug() << query.value(0) << query.value(1) << query.value(2);
            val1 = query.value(0).toInt();
            val2 = query.value(1).toInt();
            val3 = query.value(2).toInt();

        }
        else {
            qDebug() << query.lastError().text();
        }
    }
    else {
        qDebug() << query.lastError().text();
    }


    qDebug() << val1 << val2 << val3;

    updatingData(val1, val2, val3);
}

void UpdatingView::updatingData(int val1, int val2, int val3)
{
    //артист
    QSqlQuery upd1;
    upd1.prepare("UPDATE Artist SET Name = :0, Data_rozdeniya = :1, Strana = :2, Image = :3 WHERE Artist_id = :4");
    upd1.addBindValue(ui->txtArtist->text());
    upd1.addBindValue(ui->dateArtist->text());
    upd1.addBindValue(ui->txtStrana->text());

    QByteArray byteArray;
    QPixmap pix(this->path_artist);
    QBuffer buffer(&byteArray);

    if (!pix.save(&buffer, "PNG")) {
       qDebug() << "Error insert image";
    }

    upd1.addBindValue(QVariant(byteArray));
    upd1.addBindValue(val1);
    if (upd1.exec()) {
        qDebug() << "success1";
    }
    else {
        qDebug() << upd1.lastError().text();
    }

    //альбом
    QSqlQuery upd2;
    upd2.prepare("UPDATE Albom SET Nazvanie = :0, Data_vipuska = :1, Artist_id = :2, Image = :3 WHERE Albom_id = :4");
    upd2.addBindValue(ui->txtAlbom->text());
    upd2.addBindValue(ui->dateAlbom->text());
    upd2.addBindValue(val1);

    QByteArray byteArray2;
    QPixmap pix2(this->path_albom);
    QBuffer buffer2(&byteArray2);

    if (!pix2.save(&buffer2, "PNG")) {
       qDebug() << "Error insert image";
    }

    upd2.addBindValue(QVariant(byteArray2));
    upd2.addBindValue(val2);
    if (upd2.exec()) {
        qDebug() << "success2";
    }
    else {
        qDebug() << upd2.lastError().text();
    }

    //песня
    QSqlQuery upd3;
    upd3.prepare("UPDATE Pesnya SET Nazvanie = :0, Albom_id = :1, Dlitelnost = :2, Janr = :3, Price = :4 WHERE Pesnya_id = :5");
    upd3.addBindValue(ui->txtPesnya->text());
    upd3.addBindValue(val2);
    upd3.addBindValue(ui->timePesnya->text());
    upd3.addBindValue(ui->txtjanr->text());
    upd3.addBindValue(ui->spinPrice->text());
    upd3.addBindValue(val3);
    if (upd3.exec()) {
        qDebug() << "success3";
    }
    else {
        qDebug() << upd3.lastError().text();
    }
}

