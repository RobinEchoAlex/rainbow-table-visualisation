#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <ui_mainwindow.h>
#include "rainbowtable.h"
#include <QButtonGroup>
#include <QComboBox>

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
    void demoPrint(QString information,QString label);
    void stringToHtmlFilter(QString &str);
    void stringToHtml(QString &str,QColor crl);

    QAction *openAction;
    QAction *saveAction;
    QAction *aboutAction;
    QAction *languageAction;
    QButtonGroup algorithmGroup;
    QButtonGroup containerGroup;

    QString algorithmList[3];
    QString containerList[3];

private:
void openFile();
void saveFile();
void test();
void load();
void about();
void save();
void languageSelection();
int getLength(int x);
Ui::MainWindow ui;

private slots:
void on_StartGeneration_clicked();
void on_StartCrack_clicked();
void on_OSCD_clicked();
void on_Statistics_clicked();
};

#endif
