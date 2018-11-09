#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <ui_mainwindow.h>
#include "rainbowtable.h"
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

private:
void openFile();
void saveFile();
void test();
Ui::MainWindow ui;

private slots:
void on_StartGeneration_clicked();
void on_StartCrack_clicked();


};

#endif
