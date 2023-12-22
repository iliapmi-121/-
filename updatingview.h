#ifndef UPDATINGVIEW_H
#define UPDATINGVIEW_H

#include <QDialog>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QSqlQuery>
#include <QBuffer>
#include <QByteArray>
#include <QSqlError>

namespace Ui {
class UpdatingView;
}

class UpdatingView : public QDialog
{
    Q_OBJECT

public:
    explicit UpdatingView(QWidget *parent = nullptr);
    ~UpdatingView();

public slots:
    void getSelectedData(QStringList list);

signals:
    void mainWindow();

private slots:
    void on_pushButton_2_clicked();

    void on_btnImgArtist_clicked();

    void on_btnImgAlbom_clicked();

    void on_btnUpdate_clicked();

private:
    Ui::UpdatingView *ui;

    void updatingData(int, int, int);

    QString path_artist, path_albom, currentArtistName, currentAlbomName, currentPesnyaName;
};

#endif // UPDATINGVIEW_H
