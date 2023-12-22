#include "login.h"
#include "usersview.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;

    w.show();
    return a.exec();
}
