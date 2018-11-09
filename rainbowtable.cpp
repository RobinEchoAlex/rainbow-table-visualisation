#include "rainbowtable.h"
#include "MD5.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QTextBrowser>
#include <QString>
#include "mainwindow.h"

#define lengthOfword 5

void Rainbowtable::generate(int lowMargin, int upMargin)
{
    QString QTransferString;
    QString FString;
    send("Generation start now:",1);
    for(int i=lowMargin; i<=upMargin; i++)
        {
            FString = QString::number(i);
            QTransferString= "Now the word encrypted is "+FString ;
            send(QTransferString,0);
            if (whetherCalculated[intToString(i)]==1)
            {
                continue;
            }
            else
            {
                std::stringstream ss;
                ss<<i;
                std::string str = ss.str();

                int j=0;
                std::string rstr="0";
                std::string hashStr;
                char buf[lengthOfword];

                while(j<lengthOfChain)//Fixed-length chain mode
                {
                    if(j==0)
                    {
                        hashStr = md5(str);
                        setwhetherCalculated(str);
                    }
                    else hashStr = md5(rstr);
                    FString = QString::fromStdString(hashStr);
                    QTransferString= "Its hash String is " + FString;
                    send(QTransferString,0);
                    //cout<<"rstr is"<<rstr<<endl;
                    rstr=R(hashStr);
                    this->setwhetherCalculated(rstr);
                    //cout<<"rstr passed back is "<<rstr<<endl;
                    j++;
                }
                setFrontEndNodPair(str,rstr);
            }
        }
        send("generate successfully",1);
}

void Rainbowtable::setwhetherCalculated(std::string str)
{
    this->whetherCalculated[str]=true;
}

std::string Rainbowtable::intToString(int i)
{
    std::string result;
    std::ostringstream os; //构造一个输出字符串流，流内容为空
    os << i; //向输出字符串流中输出int整数i的内容
    result=os.str();
    return result;
}

void Rainbowtable::setFrontEndNodPair(std::string frontNode,std::string endNode)
{
    /*istringstream iss1(frontNode);
    int intFrontNode;
    iss1 >> intFrontNode;
    istringstream iss2(endNode);
    int intEndNode;
    iss2 >> intEndNode;*/
    frontEndNode[endNode]=frontNode;
}

std::string Rainbowtable::R(std::string hashStr)
{
    char c;
    int counter=0;
    std::string rStr;
    char crStr[lengthOfword];
    for (std::string::iterator s_iter=hashStr.begin(); s_iter!=hashStr.end(); ++s_iter)
    {
        c=*s_iter;
        if (c<='9' && c>='0')
        {
            crStr[counter]=c;
            counter++;
            if(counter==lengthOfword) break;
        }
    }
    crStr[lengthOfword]='\0';
    rStr=crStr;
    //cout<<"rstr is "<<rStr<<endl;
    return rStr;
}


void Rainbowtable::chainDeduction(std::string queryHash,std::string frontNode)
{
    std::string currentNode;
    std::string hashValue;
    QString QTransferString;
    QString QQueryHash;
    QString QCurrentNode;

    currentNode = frontNode;
    //cout<<" the front node is "<<frontNode<<endl;
    while(true)
    {
        hashValue = md5(currentNode);
        if(hashValue==queryHash) break;
        currentNode = R(hashValue);
    }

    QQueryHash=QString::fromStdString(queryHash);
    QCurrentNode=QString::fromStdString(currentNode);
    QTransferString = "The value for Hash "+QQueryHash+" is "+QCurrentNode;
    send("Congratulation!",1);
    send(QTransferString,1);
}

void Rainbowtable::query(std::string hashValue)
{
    std::string tmpHashValue;
    std::string rResult;
    std::string frontNode;
    int counter=0;
    int foundflag=0;
    int i=0;
    while(counter<=lengthOfChain)
    {
        if(counter==0) rResult=R(hashValue);
        else
        {
            i=0;
            tmpHashValue=hashValue;
            while(i<counter)
            {
                rResult=R(tmpHashValue);
                tmpHashValue=md5(rResult);
                i++;
            }
            rResult=R(tmpHashValue);
        }
        if(frontEndNode[rResult]!="")//A successful query
        {
            foundflag=1;
            frontNode = frontEndNode[rResult];
            chainDeduction(hashValue,frontNode);
            break;
        }
        else counter++;
    }
    if(foundflag==0) send("fail to find the password",1);
}

void Rainbowtable::saveTable()
{
    std::ofstream outfile;
    outfile.open("rainbowTable.dat");//TO-DO to specify the name of the database
    std::map<std::string,std::string>::iterator it;
    qDebug()<<"Save Begin"<<endl;
    it = this->frontEndNode.begin();
    while(it != frontEndNode.end())
    {
        outfile<<it->first<<" "<<it->second<<std::endl;
        it ++;
    }
    send("Table saved successfully.",1);
    outfile.close();
}

void Rainbowtable::loadExistedTable()
{
    std::ifstream infile;
    std::string endNode;
    std::string frontNode;
    QString prefix = "Load Node ";
    QString QEndNode;
    QString QFrontNode;
    QString QTransferNode;

    qDebug() << "Load Begin" <<endl;
    infile.open("C:\\Users\\Robin\\Documents\\QtProject\\RainbowTable\\rainbowTable.dat");
    if (!infile.is_open()) qDebug()<<"Open failed"<<endl;

    while(infile>>endNode>>frontNode)
    {
        QEndNode=  QString::fromStdString(endNode);
        QFrontNode = QString::fromStdString(frontNode);
        QTransferNode = "Load Node "+ QFrontNode +" "+QEndNode;
        qDebug() << QTransferNode <<endl;
        send(QTransferNode,0);
        frontEndNode[endNode]=frontNode;
    }
    send("Load Success",1);
}

void Rainbowtable::send(QString sendText,int status)
{
    qDebug()<<status<<endl;
    emit newText(sendText,status);
}

