#include "adminsview.h"
#include "ui_adminsview.h"
#include "imagedelegate.h"

AdminsView::AdminsView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminsView)
{
    ui->setupUi(this);

    artistcreating = new ArtistCreating;
    albomcreating = new AlbomCreating;
    pesnyacreating = new PesnyaCreating;
    updatingview = new UpdatingView;

    connect(artistcreating, &ArtistCreating::mainWindow, this, &AdminsView::show);
    connect(albomcreating, &AlbomCreating::mainWindow, this, &AdminsView::show);
    connect(pesnyacreating, &PesnyaCreating::mainWindow, this, &AdminsView::show);
    connect(updatingview, &UpdatingView::mainWindow, this, &AdminsView::show);

    connect(this, &AdminsView::sendSelectedData, updatingview, &UpdatingView::getSelectedData);

    handleUserLogin();
    handleAdminLogin();
}

void AdminsView::handleUserLogin()
{
    QSqlQuery query;
    model = new QSqlQueryModel;

    query.prepare("SELECT "
                 "Artist.Name, Artist.Data_rozdeniya, Artist.Strana, Artist.Image, "
                 "Albom.Nazvanie, Albom.Data_vipuska, Albom.Image, "
                 "Pesnya.Nazvanie, Pesnya.Dlitelnost, Pesnya.Janr, Pesnya.Price "
                 "FROM Artist "
                 "INNER JOIN Albom ON Artist.Artist_id = Albom.Artist_id "
                 "INNER JOIN Pesnya ON Albom.Albom_id = Pesnya.Albom_id "
                 "ORDER BY Artist.Name ASC");


    setTableView(query);
}

void AdminsView::handleAdminLogin()
{
    QSqlQuery query;
    model = new QSqlQueryModel;

    query.prepare("SELECT "
                 "Klient.Klient_id, Klient.Name, Klient.Telefon, Klient.Adres_mail, "
                 "Zakaz.Zakaz_id, Zakaz.Data_zakaza "
                 "FROM Klient "
                 "INNER JOIN Zakaz ON Klient.Klient_id = Zakaz.Klient_id");


    setTableAdminView(query);
}

AdminsView::~AdminsView()
{
    delete ui;
}

void AdminsView::setTableView(QSqlQuery query)
{
    if (query.exec()) {
        model->setQuery(query);

        //настройка столбцов
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Артист"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Д/Р"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Страна"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Фото артиста"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Альбом"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Дата выпуска"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Фото альбома"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("Песня"));
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("Длительность"));
        model->setHeaderData(9, Qt::Horizontal, QObject::tr("Жанр"));
        model->setHeaderData(10, Qt::Horizontal, QObject::tr("Цена"));

        ui->tableViewFront->setItemDelegateForColumn(3, new ImageDelegate(this));
        ui->tableViewFront->setItemDelegateForColumn(6, new ImageDelegate(this));

        ui->tableViewFront->setModel(model);

        // Обработка данных после установки модели
        if (query.next()) {
            QByteArray artistDate = query.value(1).toByteArray(); // Загрузите данные даты из базы данных
            QByteArray albomDate = query.value(5).toByteArray();
            QByteArray pesnyaTime = query.value(8).toByteArray();

            qint64 iDate_artist = *reinterpret_cast<qint64*>(artistDate.data());
            qint64 iDate_albom = *reinterpret_cast<qint64*>(albomDate.data());
            qint64 iTime_pesnya = *reinterpret_cast<qint64*>(pesnyaTime.data());

            QDate date_artist = QDate::fromJulianDay(iDate_artist);
            QDate date_albom = QDate::fromJulianDay(iDate_albom);
            QTime time = QTime::fromMSecsSinceStartOfDay(iTime_pesnya);

            QString dateString_artist = date_artist.toString("yyyy-MM-dd");
            QString dateString_albom = date_albom.toString("yyyy-MM-dd");
            QString timeString = time.toString("hh:mm:ss");

            // Установите значения ячеек в модели
            model->setData(model->index(0, 1), dateString_artist); // Установите значение ячейки для Artist.Data_rozdeniya
            model->setData(model->index(0, 5), dateString_albom); // Установите значение ячейки для Albom.Data_vipuska
            model->setData(model->index(0, 8), timeString);
            model->setData(model->index(0, 3), query.value(3).toByteArray()); // Установите значение ячейки для изображения артиста
            model->setData(model->index(0, 6), query.value(6).toByteArray());// Установите значение ячейки для Pesnya.Dlitelnost
        }
    }
}

void AdminsView::setTableAdminView(QSqlQuery query)
{
    if (query.exec()) {
        model->setQuery(query);

        //настройка столбцов
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Клиент ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Клиент"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Телефон"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Почта"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Заказ ID"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Дата заказа"));

        ui->tableViewBack->setModel(model);

        // Обработка данных после установки модели
        if (query.next()) {
            QByteArray zakazDate = query.value(5).toByteArray(); // Загрузите данные даты из базы данных

            qint64 iDate_zakaz = *reinterpret_cast<qint64*>(zakazDate.data());

            QDate date_zakaz = QDate::fromJulianDay(iDate_zakaz);

            QString dateString_zakaz = date_zakaz.toString("yyyy-MM-dd");

            // Установите значения ячеек в модели
            model->setData(model->index(0, 5), dateString_zakaz); // Установите значение ячейки для Zakaz.Data_rozdeniya
        }
    }
}

QStringList AdminsView::getSelectedData()
{
    QStringList list;

    QItemSelectionModel *selectionModel = ui->tableViewFront->selectionModel();

    if (selectionModel->hasSelection()) {
        QModelIndexList selectedIndexes = selectionModel->selectedRows();

        if (!selectedIndexes.isEmpty()) {
            for (QModelIndex index : selectedIndexes) {
                for (int column = 0; column < ui->tableViewFront->model()->columnCount(); ++column) {
                    QString data = ui->tableViewFront->model()->data(ui->tableViewFront->model()->index(index.row(), column)).toString();
                    list.append(data);
                }
            }
        }
    }

    return list;
}

void AdminsView::deleteFromDB(QStringList list)
{

    if (list.size() < 11) {
        QMessageBox::critical(this, "Ошибка", "Выделите весь ряд");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT Artist.Artist_id, Albom.Albom_id, Pesnya.Pesnya_id "
                  "FROM Artist "
                  "INNER JOIN Albom ON Artist.Artist_id = Albom.Artist_id "
                  "INNER JOIN Pesnya ON Albom.Albom_id = Pesnya.Albom_id "
                  "WHERE Artist.Name = :0 AND Albom.Nazvanie = :1 AND Pesnya.Nazvanie = :2");

    query.addBindValue(list.at(0));
    query.addBindValue(list.at(4));
    query.addBindValue(list.at(7));


    if (query.exec()) {
        if (query.first()) {
            QSqlQuery del1;
            del1.prepare("DELETE FROM Artist WHERE Artist_id = :0");
            del1.addBindValue(query.value(0).toInt());
            del1.exec();

            QSqlQuery del2;
            del2.prepare("DELETE FROM Albom WHERE Albom_id = :0");
            del2.addBindValue(query.value(1).toInt());
            del2.exec();

            QSqlQuery del3;
            del3.prepare("DELETE FROM Pesnya WHERE Pesnya_id = :0");
            del3.addBindValue(query.value(2).toInt());
            del3.exec();
        }
        else {
            qDebug() << "err";
        }
    }
    else {
        qDebug() << query.lastError().text();
    }

    handleUserLogin();
}


void AdminsView::on_btnAddArtist_clicked()
{
    this->hide();
    artistcreating->show();
}


void AdminsView::on_btnAddAlbom_clicked()
{
    this->hide();
    albomcreating->show();
}


void AdminsView::on_btnAddPesnya_clicked()
{
    this->hide();
    pesnyacreating->show();
}


void AdminsView::on_btnDelete_clicked()
{
    deleteFromDB(getSelectedData());
}


void AdminsView::on_btnEdit_clicked()
{
    if (getSelectedData().size() < 11) {
        QMessageBox::critical(this, "Ошибка", "Выделите весь ряд");
        return;
    }

    emit sendSelectedData(getSelectedData());
    this->hide();
    updatingview->show();
}

void AdminsView::on_btnLoading_clicked()
{
    handleUserLogin();
}

