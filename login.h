#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QSql>
#include <QtSql>
#include <QSqlError>
#include <QFileInfo>
#include <QDebug>
#include <QMessageBox>
#include <QString>

#include "usersview.h"
#include "adminsview.h"
#include "basket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QDialog
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();


private slots:
    void on_btnClear_clicked();

    void on_btnLogin_clicked();

    void on_btnSignin_clicked();


private:
    Ui::Login *ui;
    UsersView* usersview;
    AdminsView* adminsview;
    Basket* basket;

    QSqlDatabase db;
    bool isLogin;
};
#endif // LOGIN_H
