#include "rainbowtable.h"
#include "MD5.h"
#include "sha1.hpp"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QTextBrowser>
#include <QString>
#include <QDir>
#include <QFileDialog>
#include "mainwindow.h"
#include <windows.h>

#define lengthOfWord 5

void Rainbowtable::generate(int lowMargin, int upMargin,QString mode,QString container)
{
    SHA1 sha1;
    QString QTransferString;
    QString FString;
    unsigned long timeStart;
    unsigned long timeEnd;

    sendToBrowser(tr("Generation start now:"),1);
    timeStart = GetTickCount();
    collapseTime=0;

    if(whatisstored!=mode)
    {
        frontEndNode.clear( );
        HashFrontEndNode.clear();
        whetherCalculated.clear( );
        HashWhetherCalculated.clear();
        whatisstored=mode;
    }

    for(int i=lowMargin; i<=upMargin; i++)
    {
        FString = QString::number(i);

        if ((whetherCalculated[intToString(i)]==true && container.compare("map")==0)
                ||(HashWhetherCalculated.value(QString::number(i))==true && container.compare("hash")==0))
        {
            QTransferString=FString+tr(" is storaged in the previous deduction");
            sendToBrowser(QTransferString,0);
            collapseTime++;
            continue;
        }
        else
        {
            QTransferString=tr( "  Now the word encrypted is ")+FString ;
            sendToBrowser(QTransferString,0);
            std::stringstream ss;
            ss<<i;
            std::string str = ss.str();
            QString Qstr;
            int j=0;
            std::string rstr="0";
            QString Qrstr;
            std::string hashStr;
            char buf[lengthOfWord];
            while(j<lengthOfChain)//Fixed-length chain mode
            {
                //need to call thread
                SHA1 sha1;
                if(j==0)
                {
                    if(mode=="MD5") hashStr = md5(str);
                    else if (mode=="SHA1")
                    {
                        sha1.update(str);
                        hashStr = sha1.final();
                    }
                    setwhetherCalculated(str,container);
                }
                else
                {
                    if(mode=="MD5") hashStr = md5(rstr);
                    else if (mode=="SHA1")
                    {
                        sha1.update(rstr);
                        hashStr = sha1.final();
                    }
                }
                FString = QString::fromStdString(hashStr);
                //Information displayed on text browser start
                if (j==0)
                {
                    Qstr = QString::fromStdString(str);
                    QTransferString= Qstr+tr("'s hash is ") + FString;
                    sendToBrowser(QTransferString,0);
                }
                else
                {
                    Qrstr = QString::fromStdString(rstr);
                    QTransferString=Qrstr +tr("'s hash is ") + FString;
                    sendToBrowser(QTransferString,0);
                }
                //Information displayed on text browser end
                rstr=R_numberFilter(hashStr,j);
                this->setwhetherCalculated(rstr,container);
                j++;
            }
            setFrontEndNodPair(str,rstr,container);
             // modify demo start
            if(i==lowMargin)
            {
                sendToDemo("FN1",QString::fromStdString(str));
                sendToDemo("EN1",QString::fromStdString(rstr));
            }
            else if(i==lowMargin+1)
            {
                sendToDemo("FN2",QString::fromStdString(str));
                sendToDemo("EN2",QString::fromStdString(rstr));
            }
            else if(i==upMargin)
            {
                sendToDemo("FNE",QString::fromStdString(str));
                sendToDemo("ENE",QString::fromStdString(rstr));
            }
            // modify demo end
        }
    }
    sendToBrowser(tr("generate successfully"),1);
    timeEnd = GetTickCount();
    this->generationTime = timeEnd - timeStart;

    }

void Rainbowtable::setwhetherCalculated(std::string str,QString container)
{
    if(container.compare("map")==0) this->whetherCalculated[str]=true;
    else if(container.compare("hash")==0) this->HashWhetherCalculated[QString::fromStdString(str)]=true;
}

std::string Rainbowtable::intToString(int i)
{
    std::string result;
    std::ostringstream os; //????????????????????????????????????????????????
    os << i; //??????????????????????????????int??????i?????????
    result=os.str();
    return result;
}

void Rainbowtable::setFrontEndNodPair(std::string frontNode,std::string endNode,QString container)
{
    qDebug()<<QString::fromStdString(endNode)<<QString::fromStdString(frontNode)<<endl;
    if(container=="map") frontEndNode[endNode]=frontNode;
    else if(container=="hash") HashFrontEndNode[QString::fromStdString(endNode)]=frontNode;
}

std::string Rainbowtable::R_numberFilter(std::string hashStr,int deviation)
{
    char c;
    int counter=0;
    std::string rStr;
    char crStr[lengthOfWord];
    for (std::string::iterator s_iter=hashStr.begin()+deviation; s_iter!=hashStr.end(); ++s_iter)
    {
        c=*s_iter;
        if (c<='9' && c>='0')
        {
            crStr[counter]=c;
            counter++;
            if(counter==lengthofUpperLimit) break;
        }
    }
    crStr[lengthofUpperLimit]='\0';
    rStr=crStr;
    return rStr;
}


void Rainbowtable::chainDeduction(std::string queryHash,std::string frontNode,QString mode)
{
    SHA1 sha1;
    int j=0;
    std::string currentNode;
    std::string hashValue;
    QString QTransferString;
    QString QQueryHash;
    QString QCurrentNode;
    currentNode = frontNode;

    while(true)
    {
        if(mode=="MD5") hashValue = md5(currentNode);
        else if (mode=="SHA1")
        {
            sha1.update(currentNode);
            hashValue=sha1.final();
        }
        if(hashValue==queryHash) break;
        currentNode = R_numberFilter(hashValue,j);
        j++;
    }

    QQueryHash=QString::fromStdString(queryHash);
    QCurrentNode=QString::fromStdString(currentNode);
    QTransferString = tr("The value for Hash ")+QQueryHash+tr(" is ")+QCurrentNode;
    sendToBrowser("Congratulation!",1);
    sendToBrowser(QTransferString,1);
    sendToDemo("RealValue",QCurrentNode);
    sendToDemo("MatchOrNot",tr("Match!"));
}

void Rainbowtable::query(std::string hashValue,QString mode,QString container)
{
    SHA1 sha1;
    std::string tmpHashValue;
    std::string rResult;
    std::string frontNode;
    int counter=0;
    int foundflag=0;
    int i=0;
    const int chainLength=3; //specify the x of Rx function we are using, cause the query process is going backward
    qDebug()<<"?????????"<<QString::fromStdString(hashValue)<<endl;

    while(counter<lengthOfChain)
    {
        if(counter==0) rResult=R_numberFilter(hashValue,chainLength-1);//using the origin value to deduce
        else//using the temp hash value to deduce
        {
            i=0;
            tmpHashValue=hashValue;
            while(i<=counter)
            {
                qDebug()<<"??????R"<<(chainLength-1-counter)+i<<endl;
                rResult=R_numberFilter(tmpHashValue,(chainLength-1-counter)+i);//carefully treat with the relationship of lowmark since R starts from R0,R1..to R chainL-1
                 qDebug()<<"R??????"<<QString::fromStdString(rResult)<<endl;
                if(mode=="MD5") tmpHashValue=md5(rResult);
                else if(mode=="SHA1")
                {
                    sha1.update(rResult);
                    tmpHashValue=sha1.final();
                }
                i++;
            }
            //=R_numberFilter(tmpHashValue,(chainLength-1-counter)+i);
             qDebug()<<"??????R??????"<<QString::fromStdString(rResult)<<endl;

        }
        if((frontEndNode[rResult]!="" && container.compare("map")==0)
                || (HashFrontEndNode.value(QString::fromStdString(rResult))!="" && container.compare("hash")==0))//A successful query
        {
            foundflag=1;
            if (container.compare("map")==0) frontNode = frontEndNode[rResult];
            if (container.compare("hash")==0) frontNode= HashFrontEndNode[QString::fromStdString(rResult)];
            chainDeduction(hashValue,frontNode,mode);
            break;
        }
        else counter++;//query continue
    }
    if(foundflag==0)
    {
        sendToBrowser(tr("Fail to find the password"),1);
    }
}

void Rainbowtable::saveTableinMap(std::string filename)
{
    std::ofstream outfile;
    outfile.open(filename);
    std::map<std::string,std::string>::iterator it;
    qDebug()<<"Map Save Begin"<<endl;
    it = this->frontEndNode.begin();
    while(it != frontEndNode.end())
    {
        outfile<<it->first<<" "<<it->second<<std::endl;
        it ++;
    }
    sendToBrowser("Table saved successfully.",1);
    outfile.close();
}

void Rainbowtable::saveTableinHash(std::string filename)
{
    std::ofstream outfile;
    outfile.open(filename);
    QHash<QString,std::string>::const_iterator it = HashFrontEndNode.constBegin();
    qDebug()<<"Hash Save Begin"<<endl;
    while(it != HashFrontEndNode.constEnd())
    {
            outfile<<it.key().toStdString()<<" "<<it.value()<<std::endl;
            ++it;
    }
    sendToBrowser("Table saved successfully.",1);
    outfile.close();
}

void Rainbowtable::loadExistedTable(std::string filename,QString container)
{
    std::ifstream infile;
    std::string endNode;
    std::string frontNode;
    const QString prefix = "Load Node ";
    QString QEndNode;
    QString QFrontNode;
    QString QTransferNode;
    QString Directory;

    qDebug() << "Load Begin" <<endl;
    infile.open(filename);
    if (!infile.is_open()) qDebug()<<"Open failed"<<endl;

    while(infile>>endNode>>frontNode)
    {
        QEndNode=  QString::fromStdString(endNode);
        QFrontNode = QString::fromStdString(frontNode);
        QTransferNode = "Load Node "+ QFrontNode +" "+QEndNode;
        //qDebug() << QTransferNode <<endl;
        sendToBrowser(QTransferNode,0);
         if (container.compare("map")==0) frontEndNode[endNode]=frontNode;
         if (container.compare("hash")==0)  HashFrontEndNode[QString::fromStdString(endNode)]=frontNode;
    }
    sendToBrowser("Load Success",1);
}

void Rainbowtable::sendToBrowser(QString sendText,int status)//send the word to the text display window
{
    //qDebug()<<status<<endl;
    emit newText(sendText,status);
}

void Rainbowtable::sendToDemo(QString label,QString information)
{
    qDebug()<<label<<" "<<information<<endl;
    emit newDemo(information,label);
}

void Rainbowtable::demo(std::string hashValue,QString mode,QString container)
{
    SHA1 sha1;
    std::string tmpHashValue;
    std::string rResult;
    std::string frontNode;
    QString information;
    int counter=0;
    int foundflag=0;
    int i=0;
    const int chainLength=3;

    qDebug()<<"demo is called error-free"<<endl;
    while(counter<lengthOfChain)
    {
        if(counter==0)
        {
            rResult=R_numberFilter(hashValue,chainLength-1);//using the origin value to deduce
            sendToDemo("Value1",QString::fromStdString(rResult));
        }
        else//using the temp hash value to deduce
        {
            i=0;
            tmpHashValue=hashValue;
            while(i<counter)
            {
                rResult=R_numberFilter(tmpHashValue,chainLength-(i+2));//carefully treat with the relationship of lowmark since R starts from R0,R1..to R chainL-1
                if(mode=="MD5") tmpHashValue=md5(rResult);
                else if(mode=="SHA1")
                {
                    sha1.update(rResult);
                    tmpHashValue=sha1.final();
                }
                i++;
            }
            rResult=R_numberFilter(tmpHashValue,chainLength-(i+2));
        }
        if((frontEndNode[rResult]!="" && container.compare("map")==0)
                || (HashFrontEndNode.value(QString::fromStdString(rResult))!="" && container.compare("hash")==0))//A successful query
        {
            if(counter==0) sendToDemo("OKOrNot","OK");
            else sendToDemo("OKOrNot","Fail");
            information = "R"+QString::number(chainLength-(i+1));//since the i has been ++ at the last step, +1 instead of +2
            sendToDemo("OKOrNot2","OK");
            sendToDemo("Rx",information);
            foundflag=1;
            if (container.compare("map")==0) frontNode = frontEndNode[rResult];
            if (container.compare("hash")==0) frontNode= HashFrontEndNode[QString::fromStdString(rResult)];
            chainDeduction(hashValue,frontNode,mode);
            break;
        }
        else counter++;//query continue
    }
    if(foundflag==0)
    {
        sendToDemo("OKOrNot2","Fail");
        sendToDemo("MatchOrNot","Fail");
    }
}
