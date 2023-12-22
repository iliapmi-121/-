#include "login.h"
#include "ui_login.h"

#define path_to_db ".//musicstore.sqlite3"


Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    isLogin = true;

    usersview = new UsersView;

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_to_db);

    QFileInfo checkFile(path_to_db);


    if (checkFile.isFile()) {

        if (db.open()) {
            ui->labelStatus->setText("[+]Connected to Database File " + db.databaseName());
        }

    }

    else {
        ui->labelStatus->setText("[!]Database File doesnot exist");
    }
}

Login::~Login()
{
    delete ui;
    qDebug() << "Clossing the connection to Database file on exit";
    db.close();
}

void Login::on_btnClear_clicked()
{
    ui->txtUser->setText("");
    ui->txtPass->setText("");
}


void Login::on_btnLogin_clicked()
{
    QString username, password;
    username = ui->txtUser->text();
    password = ui->txtPass->text();

    if (!db.isOpen()) {
        qDebug() << "No connection to Database";
        return;
    }

    QSqlQuery qry;

    //если пользователь залогинен, выбрана кнопка login
    if (isLogin) {

        if (qry.exec("SELECT Username, Password, Role From users WHERE Username=\'" + username + "\' AND Password=\'" + password + "\'" )) {

            if (qry.next()) {
                ui->labelStatus->setText("[+]Valid Username and Password");

                QString msg = "Логин = " + qry.value(0).toString() + "\n" +
                              "Пароль = " + qry.value(1).toString() + "\n";

                QMessageBox::warning(this, "Вход успешно выполнен", msg);

                usersview = new UsersView;
                adminsview = new AdminsView;

                this->hide();
                if (qry.value(2).toString() == "user") {
                    usersview->show();
                }
                else {
                    adminsview->show();
                }


            }
            else {
                ui->labelStatus->setText("[-]Wrong Username or Password");
            }

        }

    }
    //если не залогинен
    else {

        if (!username.isEmpty() && !password.isEmpty()) {

            //проверка на уже существующего пользователя
            //если такой никнейм есть в базе, то не регистрируем нового
            qry.prepare("SELECT 1 FROM users WHERE Username = :Username");
            qry.bindValue(":Username", username);

            if (!qry.exec() || !qry.next()) {
                qry.prepare("INSERT INTO users (Username, Password, Role) VALUES (:Username, :Password, :Role)");
                qry.bindValue(":Username", username);
                qry.bindValue(":Password", password);
                qry.bindValue(":Role", "user");
                qry.exec();

                ui->labelStatus->setText("[+]Valid Username and Password");



                QString msg = "Пользователь был успешно создан\n "
                              "Логин = " + username + "\n" +
                              "Пароль = " + password + "\n";

                QMessageBox::warning(this, "Регистрация успешно выполнена", msg);
                isLogin = true;
                ui->btnLogin->setText("Login");
                ui->btnSignin->setText("Signin");
            }
            else {
                QMessageBox::critical(this, "Ошибка", "Пользователь с таким именем уже зарегистрировпн");
            }

        }
        else {
            ui->labelStatus->setText("[-]Wrong Username or Password");
            QMessageBox::critical(this, "Ошибка", "Имя пользователя или пароль не могут быть пустыми");
        }
    }


}


void Login::on_btnSignin_clicked()
{
    isLogin = !isLogin;
    if (!isLogin) {
        ui->btnLogin->setText("Signin");
        ui->btnSignin->setText("Login");
    }
    else {
        ui->btnLogin->setText("Login");
        ui->btnSignin->setText("Signin");
    }
}

