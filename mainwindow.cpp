#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QDebug>
#include <QTextBrowser>
#include <QButtonGroup>
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "rainbowtable.h"

Rainbowtable *rainbowtable = new Rainbowtable;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);

    QTextBrowser textBrowser;

    setWindowTitle(tr("Main Window"));

    openAction = new QAction(QIcon(":/images/OpenFile"), tr("&Open"), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing rainbow table"));
    connect(openAction, &QAction::triggered,rainbowtable, &Rainbowtable::loadExistedTable);

    saveAction = new QAction(QIcon(":/images/SaveFile"),tr("&Save"),this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the rainbow table"));
    connect(saveAction, &QAction::triggered, rainbowtable, &Rainbowtable::saveTable);

    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(openAction);
    file->addAction(saveAction);

    QToolBar *toolBar = addToolBar(tr("&File"));
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);

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

void MainWindow::test()
{
    qDebug()<<"This connect function works normally"<<endl;
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
