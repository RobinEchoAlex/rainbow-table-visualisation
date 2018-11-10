#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <ui_mainwindow.h>
#include "rainbowtable.h"
#include <QButtonGroup>

namespace Ui
{
class MainWindow;
}
class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void open();
    void openFile(Rainbowtable rainbowtable);
    void print( QString & name,int status);
    void stringToHtmlFilter(QString &str);
    void stringToHtml(QString &str,QColor crl);

    QAction *openAction;
    QAction *saveAction;
    QAction *aboutAction;
    QButtonGroup group;

private:
void openFile();
void saveFile();
void test();
void load();
void about();
void save();
Ui::MainWindow ui;

private slots:
void on_StartGeneration_clicked();
void on_StartCrack_clicked();

};

#endif
