#include "usersview.h"
#include "ui_usersview.h"
#include "imagedelegate.h"

UsersView::UsersView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsersView)
{
    ui->setupUi(this);

    basket = new Basket;
    connect(this, &UsersView::sendModel, basket, &Basket::getModel);
    connect(this, &UsersView::showCart, basket, &Basket::showCart);
    connect(basket, &Basket::mainWindow, this, &UsersView::show);

    handleUserLogin();
    settingStyleSheet();
}

UsersView::~UsersView()
{
    delete ui;
}

void UsersView::handleUserLogin()
{
   QSqlQuery query;
   model = new QSqlQueryModel;

   ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
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


void UsersView::settingStyleSheet()
{
    ui->btnAdd->setStyleSheet(
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
    ui->btnBasket->setStyleSheet(
       "QPushButton {"
           "background-color: rgb(116, 165, 204);"
           "font-size: 20px;"
           "border: 2px solid black;"
           "padding: 5px;"
       "}"
       "QPushButton:hover {"
           "background-color: rgb(64, 108, 143);"
           "font-size: 20px;"
           "border: 2px solid black;"
           "padding: 5px;"
       "}"
    );
}

void UsersView::setTableView(QSqlQuery query)
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

        ui->tableView->setItemDelegateForColumn(3, new ImageDelegate(this));
        ui->tableView->setItemDelegateForColumn(6, new ImageDelegate(this));

        ui->tableView->setModel(model);

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


void UsersView::on_btnAdd_clicked()
{
    QStringList* list = new QStringList;

    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
    int currentRow = 0;

    if (selectionModel->hasSelection()) {
        QModelIndexList selectedIndexes = selectionModel->selectedRows();

        if (!selectedIndexes.isEmpty()) {
            for (QModelIndex index : selectedIndexes) {
                for (int column = 0; column < ui->tableView->model()->columnCount(); ++column) {
                    QString data = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), column)).toString();
                    list->append(data);
                }
            }
            currentRow++;
        }
        emit sendModel(list);

    }
}


void UsersView::on_btnBasket_clicked()
{
    emit showCart();
    basket->show();
    this->hide();
}


