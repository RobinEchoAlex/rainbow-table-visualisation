#include "mainwindow.h"
#include "rainbowtable.h"
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QAction>
#include <QTranslator>
#include "ui_mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    translator.load(":/lans/lans/zh.ts");
    MainWindow mainwindow;
    a.installTranslator(&translator);
    mainwindow.show();
    return a.exec();
}
