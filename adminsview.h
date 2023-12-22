#ifndef ADMINSVIEW_H
#define ADMINSVIEW_H

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

#include "artistcreating.h"
#include "albomcreating.h"
#include "pesnyacreating.h"
#include "updatingview.h"

namespace Ui {
class AdminsView;
}

class AdminsView : public QDialog
{
    Q_OBJECT

public:
    explicit AdminsView(QWidget *parent = nullptr);
    void handleUserLogin();
    void handleAdminLogin();

    ~AdminsView();

signals:
    void sendSelectedData(QStringList list);

private slots:

    void on_btnAddArtist_clicked();

    void on_btnAddAlbom_clicked();

    void on_btnAddPesnya_clicked();

    void on_btnDelete_clicked();

    void on_btnEdit_clicked();

    void on_btnLoading_clicked();

private:
    Ui::AdminsView *ui;
    void setTableView(QSqlQuery query);
    void setTableAdminView(QSqlQuery query);
    QSqlQueryModel* model;

    QStringList getSelectedData();
    void deleteFromDB(QStringList list);

    ArtistCreating *artistcreating;
    AlbomCreating *albomcreating;
    PesnyaCreating *pesnyacreating;
    UpdatingView *updatingview;

};

#endif // ADMINSVIEW_H
