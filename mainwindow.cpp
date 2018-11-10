#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QDebug>
#include <QTextBrowser>
#include <QButtonGroup>
#include <QDir>
#include <QFileDialog>
#include <string>
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "rainbowtable.h"

Rainbowtable *rainbowtable = new Rainbowtable;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);

    QTextBrowser textBrowser;
    QIcon icon(":/icon/images/Rainbow.png");

    setWindowTitle(tr("Rainbow Table"));
    setFixedSize(980,620);
    setWindowIcon(icon);

    openAction = new QAction(QIcon(":/icon/images/Open.png"), tr("&Open"), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing rainbow table"));
    connect(openAction, &QAction::triggered,this, &MainWindow::load);
    //QString Directory = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this,tr("load file"),QDir::currentPath()));


    saveAction = new QAction(QIcon(":/icon/images/Save.png"),tr("&Save"),this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the rainbow table"));
    connect(saveAction, &QAction::triggered, this, &MainWindow::save);

    aboutAction = new QAction(QIcon(":/icon/images/Information.png"),tr("&Information"),this);
    aboutAction->setShortcut(QKeySequence::HelpContents);
    aboutAction->setStatusTip(tr("Information about this program"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);

    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(openAction);
    file->addAction(saveAction);
    file->addAction(aboutAction);

    QToolBar *toolBar = addToolBar(tr("&File"));
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addAction(aboutAction);

    connect(rainbowtable, &Rainbowtable::newText, this, &MainWindow::print);

    group.addButton(ui.MD5Sel,0);//set label for radio button
    group.addButton(ui.SHA1Sel,1);
    ui.MD5Sel->setChecked(true);
    ui.SHA1Sel->setChecked(true);

    qDebug() << "Mainwindow(QWidget *parent) End" <<endl;
    statusBar() ;
}

MainWindow::~MainWindow()
{

}

void MainWindow::open()
{
    QMessageBox::information(this,tr("Information"),tr("Open"));
}

void MainWindow::print(QString & name,int status)
{
    if(status==1)
    {
        QColor  clrR(255,0,0);//Colour is set to red
        stringToHtmlFilter(name);
        stringToHtml(name,clrR);
        ui.textBrowser->insertHtml("<br />");
        ui.textBrowser->insertHtml(name);
    }
    else
    {
        QColor clrR(0,0,0);//Colour is set to black
        stringToHtmlFilter(name);
        stringToHtml(name,clrR);
        ui.textBrowser->insertHtml("<br />");
        ui.textBrowser->insertHtml(name);
    }
    ui.textBrowser->moveCursor(QTextCursor::End);
}

void MainWindow::stringToHtmlFilter(QString &str)
{
    str.replace("&","&amp;");
    str.replace(">","&gt;");
    str.replace("<","&lt;");
    str.replace("\"","&quot;");
    str.replace("\'","&#39;");
    str.replace(" ","&nbsp;");
    str.replace("\n","<br>");
    str.replace("\r","<br>");
}

void MainWindow::stringToHtml(QString &str,QColor crl)
{
    QByteArray array;
    array.append(crl.red());
    array.append(crl.green());
    array.append(crl.blue());
    QString strC(array.toHex());
    str = QString("<spanT style=\" color:#%1;\">%2</span>").arg(strC).arg(str);
}
void MainWindow::on_StartGeneration_clicked()
{
    int lowMargin;
    int upMargin;
    int a;

    lowMargin=ui.lowMargin->value();
    upMargin=ui.upMargin->value();
    a=group.checkedId();

    if (a==0) rainbowtable->generate(lowMargin,upMargin,"MD5");
    else if (a==1) rainbowtable->generate(lowMargin,upMargin,"SHA1");
}

void MainWindow::on_StartCrack_clicked()
{
    std::string Hash;
    QString QHash;
    int a;

    a=group.checkedId();
    QHash = ui.HashValue->text();
    QHash = QHash.simplified();//Remove whiteSpace at the end of the qhash
    Hash = QHash.toStdString();

    if (a==0) rainbowtable->query(Hash,"MD5");
    else if (a==1) rainbowtable->query(Hash,"SHA1");
}

void MainWindow::about()
{
    QMessageBox::information(this, tr("About..."), tr("developed by gitee:robinalex\n"
                                                      "copyright information:\n"
                                                      "SHA1 Algorithm is in public domain\n"
                                                      "MD5 Algorithm is Copyright (C) 1991-2, RSA Data Security\n"
                                                      "All other creations can be distributed under CC BY-SA 4.0\n"))
            ;
}

void MainWindow::load()
{
    QString QfileName;
    QString information;
    std::string fileName;
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Load File"));
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setFileMode(QFileDialog::AnyFile);
    fileDialog->setViewMode(QFileDialog::Detail);

    fileDialog->setGeometry(10,30,300,200);
    fileDialog->setDirectory(".");
    QfileName = fileDialog->getOpenFileName(
                                 this,
                                 tr("Open File"),
                                 "/home",
                                 tr("Data base (*.dat)"));
    #ifdef WIN32
    QfileName.replace("/", "\\");    //change format of filename when operate under Windows
    #endif
    qDebug()<<"filename is" <<QfileName<<endl;
    fileName= QfileName.toStdString();
    if (QfileName.isEmpty())
    {
        information="Open Rainbow table cancelled";
        this->print(information,0);
        return;
    }
    else
    {
        information = "File "+QfileName+" open";
        this->print(information,0);
        rainbowtable->loadExistedTable(fileName);
    }
}

void MainWindow::save()
{
    QString QfileName;
    QString information;
    std::string fileName;
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Save File"));
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setFileMode(QFileDialog::AnyFile);
    fileDialog->setViewMode(QFileDialog::Detail);
    fileDialog->setGeometry(10,30,300,200);
    fileDialog->setDirectory(".");
    QfileName = fileDialog->getSaveFileName(
                                 this,
                                 tr("Save Database"),
                                 "/home",
                                 tr("Data base (*.dat)"));
    #ifdef WIN32
    QfileName.replace("/", "\\");
    #endif
    qDebug()<<"filename is" <<QfileName<<endl;
    fileName= QfileName.toStdString();
    if (QfileName.isEmpty())
    {
        information="Saving Rainbow table cancelled";
        this->print(information,0);
        return;
    }
    else
    {
        information = "Save database in "+QfileName;
        this->print(information,0);
        rainbowtable->saveTable(fileName);
    }
}
