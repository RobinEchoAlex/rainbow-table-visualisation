#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QDebug>
#include <QTextBrowser>
#include <QButtonGroup>
#include <QDir>
#include <QTranslator>
#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>
#include <string>
#include <windows.h>
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "rainbowtable.h"

Rainbowtable *rainbowtable = new Rainbowtable;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
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

    languageAction = new QAction(QIcon(":/icon/images/Information.png"),tr("&Language"),this);
    languageAction->setShortcut(QKeySequence::Backspace);
    languageAction->setStatusTip((tr("Choose language")));
    connect(languageAction,&QAction::triggered,this,&MainWindow::languageSelection);

    QMenu *file = menuBar()->addMenu(tr("&File"));
    QMenu *settingMenu = menuBar()->addMenu(tr("&Setting"));
    file->addAction(openAction);
    file->addAction(saveAction);
    file->addAction(aboutAction);
    settingMenu->addAction(languageAction);

    QToolBar *toolBar = addToolBar(tr("&File"));
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addAction(aboutAction);

    connect(rainbowtable, &Rainbowtable::newText, this, &MainWindow::print);
    connect(rainbowtable,&Rainbowtable::newDemo,this,&MainWindow::demoPrint);

    //here is the accordance relationship between int label and string argument
    algorithmGroup.addButton(ui.MD5Sel,0);//set label for algorithm radio button
    algorithmGroup.addButton(ui.SHA1Sel,1);
    ui.MD5Sel->setChecked(true);
    ui.SHA1Sel->setChecked(true);
    containerGroup.addButton(ui.MapSel,0);//set label for container radio button
    containerGroup.addButton(ui.HashSel,1);
    ui.MapSel->setChecked(true);
    ui.HashSel->setChecked(true);
    //Remember to reset the size of relationship array in the header
    algorithmList[0]="MD5";//uppercase for algorithm
    algorithmList[1]="SHA1";
    containerList[0]="map";//lowercase for container
    containerList[1]="hash";

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
    int c;

    lowMargin=ui.lowMargin->value();
    upMargin=ui.upMargin->value();
    a=algorithmGroup.checkedId();
    c=containerGroup.checkedId();

    rainbowtable->lengthofUpperLimit=this->getLength(upMargin);
    rainbowtable->lengthofLowerLimit=this->getLength(lowMargin);
    rainbowtable->generate(lowMargin,upMargin,algorithmList[a],containerList[c]);
}

void MainWindow::on_StartCrack_clicked()
{
    std::string Hash;
    QString QHash;
    QString Information;
    int a;
    int c;

    a=algorithmGroup.checkedId();
    c=containerGroup.checkedId();
    QHash = ui.HashValue->text();
    QHash = QHash.simplified();//Remove whiteSpace at the end of the qhash
    Hash = QHash.toStdString();

    Information= tr("Please enter a valid hash in the box");
    if(Hash.length()<=2)
    {
        print(Information,0);//too short to be a hash
        return;
    }
        rainbowtable->query(Hash,algorithmList[a],containerList[c]);
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
    int c;
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

    if (QfileName.isEmpty()) //user cancelled the request
    {
        information="Open Rainbow table cancelled";
        this->print(information,0);
        return;
    }
    else
    {
        c = containerGroup.checkedId();
        information = "File "+QfileName+" open";
        this->print(information,0);
        rainbowtable->loadExistedTable(fileName,containerList[c]);
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
        if (containerGroup.checkedId()==0) rainbowtable->saveTableinMap(fileName);
        else if(containerGroup.checkedId()==1) rainbowtable->saveTableinHash(fileName);
    }
}

int MainWindow::getLength(int x)
{
    int length=0;
    while(x)
    {
        x/=10;
        length++;
    }
    return length;
}

void MainWindow::on_OSCD_clicked()
{
    std::string Hash;
    QString QHash;
    int a;
    int c;

    a=algorithmGroup.checkedId();
    c=containerGroup.checkedId();
    QHash = ui.HashValue->text();
    QHash = QHash.simplified();//Remove whiteSpace at the end of the qhash
    Hash = QHash.toStdString();

    rainbowtable->demo(Hash,algorithmList[a],containerList[c]);
}

void MainWindow::demoPrint(QString information,QString label)
{
    qDebug()<<"demo was called error-freee"<<endl;
    if(label.compare("Value1")==0)
    {
        ui.Value1->setText(information);
    }
    else if(label.compare("Rx")==0)
    {
        ui.Rx->setText(information);
    }
    else if(label.compare("RealValue")==0)
    {
        ui.RealValue->setText(information);
    }
}

void MainWindow::on_Statistics_clicked()
{
    QMessageBox::information(this, tr("About..."), tr("Current chain length:3"));
}

void MainWindow::languageSelection()
{

    QTranslator translator;
    bool ok;
    QStringList languages;
    QString languageSelect;
    languages<<tr("English")<<tr("简体中文");//update needed if new languages added
    languageSelect = QInputDialog::getItem(
                          this,
                          tr("Input the language you want to use:"),
                          tr("Language available:English,简体中文"),
                          languages,
                          0,
                          false,
                          &ok);
    if(languageSelect.compare("简体中文")==0)//load successful or not
    {
        if (translator.load(":/lans/zh.qm"))
        {
            qDebug()<<"ts file loaded successfully"<<endl;
            qApp->installTranslator(&translator);//what is qApp?
            ui.retranslateUi(this);
        }
    }
    else if(languageSelect.compare("English")==0)
    {
        if (translator.load(":/lans/lans/en.qm"));
        {
            qApp->installTranslator(&translator);
            ui.retranslateUi(this);
        }
    }
}
