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
    translator.load(":/lans/zh.qm");
    a.installTranslator(&translator);
    MainWindow mainwindow;
    mainwindow.show();
    return a.exec();
}
