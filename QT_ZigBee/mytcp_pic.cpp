#include "mytcp_pic.h"
#include <QDebug>
#include <QHostAddress>
#include <QDataStream>
#include "mythread.h"
#include <QTcpSocket>
#include <QFileInfo>

#define PIC_PORT 8888

MyTcp_Pic::MyTcp_Pic(QObject *parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    timer = new QTimer(this);
    localFile = new QFile(this);
    loadSize = 4*1024;
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
    pic_flag = false;
    qDebug()<<"MyTCP";
    connect(tcpSocket, &QTcpSocket::connected,
                [=]()
                {
                    qDebug()<<"MyTcp_Pic*********************";
                }
                );


    connect(tcpSocket,SIGNAL(disconnected()),//断开连接同时删除套接字
                      tcpSocket,SLOT(deleteLater()));

    //connect(tcpSocket,SIGNAL(bytesWritten(qint64)),this,
      //     SLOT(updateClientProgress(qint64)));


    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,
           SLOT(displayError(QAbstractSocket::SocketError)));

    connect(timer, &QTimer::timeout,
                [=]()
                {
                    //关闭定时器
                    timer->stop();
                    //timer.deleteLater();

                    //发送文件
                    sendData();
                }

                );


}


void  MyTcp_Pic::connectToGateway(QString str)
{

    qDebug()<<"connectToGateway*******************************************";
    tcpSocket->connectToHost(QHostAddress(str), PIC_PORT);

    qDebug()<<"connectToGateway*******************************************";
}

void MyTcp_Pic::setFlag(bool flag)
{
    if(flag == true)
    {
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
        if(timer != NULL)
            timer->deleteLater();
    }

}

void MyTcp_Pic::loginSystem()
{

    //1. 是否需要断开当前连接
    //if(tcpSocket->state() == QTcpSocket::ConnectedState){
    if(0){
        tcpSocket->close();
    }




    tcpSocket->disconnectFromHost();//此函数关闭套接字触发disconnected()函数，接着由上面连接触发deleteLater()删除套接字
    tcpSocket->close();


    //tcpSocket->deleteLater();
    tcpSocket = new QTcpSocket(this);


    //2. 根据新的IP地址进行新的连接
    QFile file("zigbee_config.txt");
    file.open(QIODevice::ReadWrite);
    QByteArray ba=file.read(file.bytesAvailable());
    QString ip_str=QString(ba);
    qDebug()<<ip_str;
    file.close();
    tcpSocket->connectToHost(QHostAddress(ip_str), PIC_PORT);

    //3. 连接成功以后发送connectOK信号
    if(tcpSocket->state() == QTcpSocket::ConnectedState){//暂时不能捕捉状态
        emit connectOK(0);
    }else{
        emit connectOK(0);
    }

    qDebug() << "MyTcp_Pic thread:========================"<< QThread::currentThread() ;
}


void MyTcp_Pic::tcp_send(char * str,int len)
{
    tcpSocket->write(str,len);
    qDebug()<<"tcp_send*******************************************************************************";
}

void MyTcp_Pic::startTransfer(QString pic_path,qint64 len)  //实现文件大小等信息的发送
{
    qDebug() <<"startTransfer\r\n\r\n\r\n"<<pic_path; //获取文件大小

#if 1
    fileName = pic_path;
    QFileInfo info(pic_path);
    fileName = info.fileName(); //获取文件名字
    qDebug() <<len<<"  "<<info.size(); //获取文件大小

    localFile->setFileName(pic_path);

    //打开文件
    bool isOk = localFile->open(QIODevice::ReadOnly);
    if(false == isOk)
    {
        qDebug() << "只读方式打开文件失败 106";
    }


    /*localFile = new QFile(fileName);
    if(!localFile->open(QFile::ReadOnly))
    {
       qDebug() << "open file error!";
       return;
    }*/

    //文件总大小
    totalBytes = localFile->size();
    qDebug() <<totalBytes; //获取文件大小
    outBlock.resize(0);
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    //sendOut.setVersion(QDataStream::Qt_4_6);
    QString currentFileName = fileName.right(fileName.size()
    - fileName.lastIndexOf('/')-1);



    qDebug()<<currentFileName<<"\r\n\r\n\r\n\r\n\r\n\r\n";
    qDebug()<<currentFileName.size();
    char*  ch;

    QByteArray ba = currentFileName.toLatin1();

    ch=ba.data();
    qDebug()<<totalBytes;

    //依次写入总大小信息空间，文件名大小信息空间，文件名
    sendOut <<qint8(0)<<qint8(0)<<qint8(0)<<qint8(0)<< qint64(0) << qint64(0) << ch;

    //这里的总大小是文件名大小等信息和实际文件大小的总和
    totalBytes += outBlock.size();
    len +=outBlock.size();
    qDebug()<<outBlock.size();

    sendOut.device()->seek(0);
    //返回outBolock的开始，用实际的大小信息代替两个qint64(0)空间
    sendOut<<qint8(BASE)<<qint8(PHOTO_TYPE)<<qint8(1)<<qint8(outBlock.size()-sizeof(qint8)*4)<<len<<qint64((outBlock.size() - sizeof(qint64)*2-sizeof(qint8)*4));

    //发送完头数据后剩余数据的大小

    pic_flag = true;

    int tmp_len =tcpSocket->write(outBlock);
    if(tmp_len > 0)//头部信息发送成功
    {
        //发送真正的文件信息
        //防止TCP黏包
        //需要通过定时器延时 20 ms
        timer->start(20);


    }else{
        qDebug() << "startTransfer err";
    }
    bytesToWrite = totalBytes -tmp_len;

    //ui->clientStatusLabel->setText(tr("已连接"));
    totalBytes -= outBlock.size();
    outBlock.resize(0);

#endif
}
void MyTcp_Pic::sendData()
{

    qDebug() << "sendData \r\n\r\n\r\n";
    bytesWritten = 0;
    qint64 len = 0;

     do
     {
        //每次发送数据的大小
        char buf[4*1024] = {0};
        len = 0;

        //往文件中读数据
        len = localFile->read(buf, sizeof(buf));

        if(len <= 0)
            break;
        //发送数据，读多少，发多少
        len = tcpSocket->write(buf, len);
        qDebug()<<len;
        //发送的数据需要累积
        bytesWritten += len;

     }while(len > 0);
     qDebug()<<bytesWritten<<" "<<localFile->size();
     localFile->close(); //如果没有发送任何数据，则关闭文件
     //localFile->deleteLater();
     pic_flag = false;
     if(bytesWritten == localFile->size()){
        qDebug() << "send pic OK\r\n\r\n\r\n!";
     }


}

//更新进度条，实现文件的传送
void MyTcp_Pic::updateClientProgress(qint64 numBytes)
{
#if 0
    qDebug()<<"updateClientProgress"<<"\r\n\r\n\r\n\r\n\r\n\r\n";
    if(pic_flag == false)
        return ;
    qDebug()<<"updateClientProgress"<<"\r\n\r\n\r\n\r\n\r\n\r\n";
    //已经发送数据的大小
    bytesWritten += (int)numBytes;

    if(bytesToWrite > 0) //如果已经发送了数据
    {
   //每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，
   //就发送剩余数据的大小
       outBlock = localFile->read(qMin(bytesToWrite,loadSize));

       //发送完一次数据后还剩余数据的大小
       bytesToWrite -= (int)tcpSocket->write(outBlock);

       //清空发送缓冲区
       outBlock.resize(0);

    } else {
       localFile->close(); //如果没有发送任何数据，则关闭文件
       pic_flag = false;
       bytesWritten = 0;
    }

    //更新进度条
    //ui->clientProgressBar->setMaximum(totalBytes);
    //ui->clientProgressBar->setValue(bytesWritten);
    qDebug() << "send pic "<<bytesWritten<<"now :"<<numBytes;

    if(bytesWritten == totalBytes) //发送完毕
    {
     //ui->clientStatusLabel->setText(tr("传送文件 %1 成功")
//.arg(fileName));
       localFile->close();
       pic_flag = false;
       bytesWritten = 0;
       qDebug() << "send pic OK\r\n\r\n\r\n!";
       //tcpClient->close();
    }

#endif

}
void MyTcp_Pic::displayError(QAbstractSocket::SocketError) //显示错误
{
    qDebug() << tcpSocket->errorString();
    tcpSocket->close();
    //ui->clientProgressBar->reset();
    //ui->clientStatusLabel->setText(tr("客户端就绪"));
    //ui->sendButton->setEnabled(true);
}
