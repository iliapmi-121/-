#ifndef PESNYACREATING_H
#define PESNYACREATING_H

#include <QDialog>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QSqlQuery>
#include <QBuffer>
#include <QByteArray>
#include <QSqlQueryModel>
#include <QSqlError>

namespace Ui {
class PesnyaCreating;
}

class PesnyaCreating : public QDialog
{
    Q_OBJECT

public:
    explicit PesnyaCreating(QWidget *parent = nullptr);
    ~PesnyaCreating();

signals:
    void mainWindow();

private slots:
    void on_btnAddToDB_clicked();

    void on_btnPrev_clicked();

private:
    Ui::PesnyaCreating *ui;
};

#endif // PESNYACREATING_H
