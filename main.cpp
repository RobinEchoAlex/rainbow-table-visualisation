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
    QLocale locale;
    if( locale.language() == QLocale::English )  //Automatically choose the language, in accordance with system enviornment
        {
            translator.load(QString(":/lans/lans/en.qm"));
            a.installTranslator(&translator);
        }
    else if( locale.language() == QLocale::Chinese )
        {
            qDebug() << "中文系统";
            translator.load(QString(":/lans/zh.qm"));
            a.installTranslator(&translator);
        }

    MainWindow mainwindow;
    mainwindow.show();
    return a.exec();
}
