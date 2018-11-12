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
#include <QHash>
#include <QThread>

class Rainbowtable: public QObject
{
    Q_OBJECT
public :
    Rainbowtable() {}
    void chainDeduction(std::string queryHash,std::string frontNode,QString mode);
    std::string R_numberFilter(std::string hashStr,int deviation);
    void setwhetherCalculated(std::string str,QString container);
    static std::string intToString(int i);
    void setFrontEndNodPair(std::string frontNode,std::string endNode,QString container);
    void sendToBrowser(QString sendText,int status);
    void sendToDemo(QString label,QString information);
    void demo(std::string hashValue,QString mode,QString container);

    std::map <std::string, std::string> frontEndNode ;
    std::map <std::string, bool> whetherCalculated;
    QHash <QString,std::string> HashFrontEndNode;
    QHash <QString,bool > HashWhetherCalculated;
    int lengthOfChain=3;
    QString whatisstored;
    int lengthofUpperLimit;
    int lengthofLowerLimit;
    unsigned generationTime;

public slots:
    void loadExistedTable(std::string filename,QString container);
    void saveTableinMap(std::string filename);
    void saveTableinHash(std::string filename);
    void generate(int lowMargin,int upMargin,QString mode,QString container);
    void query(std::string hashvalue,QString mode,QString container);

signals:
    void newText( QString &name,int status);
    void newDemo( QString &information,QString &label);

private:
    Ui::MainWindow ui;
};

#endif // RAINBOWTABLE_H
