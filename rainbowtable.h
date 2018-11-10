#ifndef RAINBOWTABLE_H
#define RAINBOWTABLE_H

#include <string.h>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <stdlib.h>
#include <QObject>
#include <ui_mainwindow.h>
#include <QLabel>

class Rainbowtable: public QObject
{
    Q_OBJECT
public :
    Rainbowtable() {}
    void chainDeduction(std::string queryHash,std::string frontNode,std::string mode);
    std::string R(std::string hashStr);
    void setwhetherCalculated(std::string str);
    static std::string intToString(int i);
    void setFrontEndNodPair(std::string frontNode,std::string endNode);
    void send(QString sendText,int status);

    std::map <std::string, std::string> frontEndNode ;
    std::map <std::string, bool> whetherCalculated;
    int lengthOfChain=3;
    std::string whatisstored;

public slots:
    void loadExistedTable();
    void saveTable();
    void generate(int lowMargin,int upMargin,std::string mode);
    void query(std::string hashvalue,std::string mode);

signals:
    void newText( QString &name,int status);

private:
    Ui::MainWindow ui;
};

#endif // RAINBOWTABLE_H
