#include <QApplication>
#include "mainwindow.h"
#include "mysqldata.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont f = a.font();
    f.setPointSize(14);
    a.setFont(f);

    MainWindow w;
    w.show();
    return a.exec();
}
