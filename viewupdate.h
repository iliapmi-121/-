#ifndef VIEWUPDATE_H
#define VIEWUPDATE_H

#include <QDialog>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QSqlQuery>
#include <QBuffer>
#include <QByteArray>
#include <QSqlError>

namespace Ui {
class updatingview;
}

class updatingview : public QDialog
{
    Q_OBJECT

public:
    explicit updatingview(QWidget *parent = nullptr);
    ~updatingview();

signals:
    void mainWindow();

private slots:
    void on_btnImgArtist_clicked();

    void on_btnImgAlbom_clicked();

    void on_btnUpdate_clicked();

    void on_btnPrev_clicked();

private:
    Ui::updatingview *ui;
};

#endif // VIEWUPDATE_H
