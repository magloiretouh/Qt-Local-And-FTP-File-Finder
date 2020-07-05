#include <QApplication>
#include "windowss.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WindowsS w;
    QFile file(":/qss/material.qss");
    file.open(QIODevice::ReadOnly);
    a.setFont(QFont("Century Gothic",10, 60, false));
    a.setStyleSheet(QLatin1String(file.readAll()));
    w.show();

    return a.exec();
}
