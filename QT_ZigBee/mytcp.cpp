#include "mytcp.h"
#include <QDebug>
#include <QHostAddress>
#include <QDataStream>
#include "mythread.h"
#include <QTcpSocket>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#define IP ("192.168.1.125")
#define DHT_PORT  9999

MyTCP::MyTCP(QObject *parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);

    qDebug()<<"MyTCP";
    connect(tcpSocket, &QTcpSocket::connected,
                [=]()
                {
                    qDebug()<<"MyTCP*********************OK";
                }
                );


    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,
           SLOT(displayError(QAbstractSocket::SocketError)));


    loginSystem();

    time_str.clear();

}

void  MyTCP::connectToGateway(QString str)
{

    qDebug()<<"connectToGateway*******************************************";
    tcpSocket->connectToHost(QHostAddress(str), DHT_PORT);

    qDebug()<<"connectToGateway*******************************************";
}

void MyTCP::queryTime(){
    char tcp_data[6]={0};
    tcp_data[0] = BASE;
    tcp_data[1] = QUERY_TIME;
    tcp_data[2] = 0;
    tcp_data[3] = 0;
    tcp_data[4] = 0;
    tcp_data[5] = 0;
    tcpSocket->write(tcp_data,6);
    qDebug() << "MyTcp thread query time:========================\r\n\r\n"<< QThread::currentThread() ;
    time_str.clear();
}

void  MyTCP::setTime()
{
    QByteArray buffer;
    //qDebug()<<"\r\n\r\n\r\ntime_str+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n\r\n"<<time_str;
    //读取缓冲区数据
    buffer = tcpSocket->readAll();
    if(!buffer.isEmpty())
    {
        time_str +="date  ";
        time_str +=tr(buffer);
    }
    qDebug()<<"\r\n\r\n\r\ntime_str:"<<time_str<<"\r\n\r\n";
    QByteArray ba = time_str.toLatin1();
    char *str = ba.data();
    system(str);

    //emit updateUI();

}

void MyTCP::setFlag(bool flag)
{
    if(flag == true)
    {
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
    }

}

void MyTCP::loginSystem()
{

    //1. 是否需要断开当前连接
    //if(tcpSocket->state() == QTcpSocket::ConnectedState){
    if(0){
        tcpSocket->close();
    }
    connect(tcpSocket,SIGNAL(disconnected()),//断开连接同时删除套接字

                      tcpSocket,SLOT(deleteLater()));


    tcpSocket->close();
    tcpSocket->disconnectFromHost();//此函数关闭套接字触发disconnected()函数，接着由上面连接触发deleteLater()删除套接字
    //tcpSocket->deleteLater();
    tcpSocket = new QTcpSocket(this);

    //2. 根据新的IP地址进行新的连接
    QFile file("zigbee_config.txt");
    file.open(QIODevice::ReadWrite);
    QByteArray ba=file.read(file.bytesAvailable());
    QString ip_str=QString(ba);
    qDebug()<<ip_str;
    file.close();
    tcpSocket->connectToHost(QHostAddress(ip_str), DHT_PORT,QTcpSocket::ReadWrite);
    tcpSocket->waitForConnected(20);
    connect(tcpSocket,&QTcpSocket::readyRead,this,
           &MyTCP::setTime);

    //3. 连接成功以后发送connectOK信号
    if(tcpSocket->state() == QTcpSocket::ConnectedState){//暂时不能捕捉状态
        emit connectOK(0);
    }else{
        emit connectOK(0);
    }

    char tcp_data[6]={0};
    tcp_data[0] = BASE;
    tcp_data[1] = QUERY_TIME;
    tcp_data[2] = 0;
    tcp_data[3] = 0;
    tcp_data[4] = 0;
    tcp_data[5] = 0;
    tcpSocket->write(tcp_data,6);
    qDebug() << "MyTcp thread query time:========================\r\n\r\n"<< QThread::currentThread() ;
    time_str.clear();

}


void MyTCP::tcp_send(char * str,int len)
{
    //for(int i = 0;i <len;i++)
      //  qDebug("%x,",static_cast<int>(str[i]));
    tcpSocket->write(str,len);

    qDebug()<<"tcp_send*******************************************************************************"<<str;
}


void MyTCP::displayError(QAbstractSocket::SocketError) //显示错误
{
    qDebug() << tcpSocket->errorString()<<"sjfdoijsklfjsdkljfklsdjklfds";
    tcpSocket->close();
    //ui->clientProgressBar->reset();
    //ui->clientStatusLabel->setText(tr("客户端就绪"));
    //ui->sendButton->setEnabled(true);
}
