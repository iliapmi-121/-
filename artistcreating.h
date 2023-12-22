#ifndef ARTISTCREATING_H
#define ARTISTCREATING_H

#include <QDialog>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QSqlQuery>
#include <QBuffer>
#include <QByteArray>
#include <QSqlError>

namespace Ui {
class ArtistCreating;
}

class ArtistCreating : public QDialog
{
    Q_OBJECT

public:
    explicit ArtistCreating(QWidget *parent = nullptr);
    ~ArtistCreating();

signals:
    void mainWindow();

private slots:
    void on_btnOpenImg_clicked();

    void on_btnAddToDB_clicked();

    void on_btnPrev_clicked();

private:
    Ui::ArtistCreating *ui;
    QString path;
};

#endif // ARTISTCREATING_H
