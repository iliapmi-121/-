#ifndef ALBOMCREATING_H
#define ALBOMCREATING_H

#include <QDialog>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QSqlQuery>
#include <QBuffer>
#include <QByteArray>
#include <QSqlError>
#include <QComboBox>
#include <QSqlQueryModel>

namespace Ui {
class AlbomCreating;
}

class AlbomCreating : public QDialog
{
    Q_OBJECT

public:
    explicit AlbomCreating(QWidget *parent = nullptr);
    ~AlbomCreating();

signals:
    void mainWindow();

private slots:
    void on_btnOpenImg_clicked();

    void on_btnAddToDB_clicked();

    void on_btnPrev_clicked();

private:
    Ui::AlbomCreating *ui;
    QString path;
};

#endif // ALBOMCREATING_H
