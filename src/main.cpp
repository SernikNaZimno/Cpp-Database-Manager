#include <QApplication>
#include "../include/MainWindow.h"
#include <iostream>

int main(int argc, char *argv[]) {
    #ifdef Q_OS_WIN
        system("chcp 65001"); // Zmienia stronę kodową konsoli Windows na UTF-8
    #endif
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}