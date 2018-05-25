#ifndef MYTCP_PIC_H
#define MYTCP_PIC_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QFile>
#include <QTimer>

class MyTcp_Pic : public QObject
{
    Q_OBJECT
public:
    explicit MyTcp_Pic(QObject *parent = 0);
    void setFlag(bool flag);

signals:
    void connectOK(int flag);

public slots:
    void connectToGateway(QString str);
    void tcp_send(char * str,int len);
    void startTransfer(QString pic_path,qint64 len);  //发送文件大小等信息
    void updateClientProgress(qint64); //发送数据，更新进度条
    void displayError(QAbstractSocket::SocketError); //显示错误
    void loginSystem();


private:
    QTcpSocket *tcpSocket; //通信套接字
    QFile *localFile;  //要发送的文件
    qint64 totalBytes;  //数据总大小
    qint64 bytesWritten;  //已经发送数据大小
    qint64 bytesToWrite;   //剩余数据大小
    qint64 loadSize;   //每次发送数据的大小
    QString fileName;  //保存文件路径
    QByteArray outBlock;  //数据缓冲区，即存放每次要发送的数据
    bool pic_flag;
    QTimer *timer; //定时器
    void sendData(); //发送文件数据
};

#endif // MYTCP_PIC_H
