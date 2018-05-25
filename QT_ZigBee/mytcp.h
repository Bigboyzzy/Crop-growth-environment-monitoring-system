#ifndef MYTCP_H
#define MYTCP_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QFile>

class MyTCP : public QObject
{
    Q_OBJECT
public:
    explicit MyTCP(QObject *parent = 0);
    void setFlag(bool flag);
    void queryTime();

signals:
    void connectOK(int flag);
    void  updateUI();




public slots:
    void connectToGateway(QString str);
    void tcp_send(char * str,int len);

    void displayError(QAbstractSocket::SocketError); //显示错误
    void loginSystem();
    void setTime();

private:
    QTcpSocket *tcpSocket; //通信套接字
    QString time_str;

};

#endif // MYTCP_H
