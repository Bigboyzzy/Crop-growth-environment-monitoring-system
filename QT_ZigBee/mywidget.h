#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include "searchfilter.h"
#include "imageviewer.h"
#include "frmmain.h"
#include "mythread.h"
#include "headtitle.h"
//include <qwt_dial.h>
#include "qcgaugewidget.h"
#include "qcwmeter.h"
#include "qcwthermometer.h"
#include "mydht.h"
#include "customtablemodel.h"
#include "tablewidget.h"
#include <QSqlDatabase>
#include <QTcpSocket>
#include "mytcp.h"
#include "mytcp_pic.h"

#define NO_SERIAL       0
#define ONLY_ZIGBEE     1
#define ONLY_485        2
#define BEGIN_ZIGBEE    3
#define BEGIN_485       4



namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();
    void detectSerial();//探测系统可用的串口列表
    void setDHTLayout(int num=16);
    void set_one_page(MyDHT * dht_items,QWidget * widget,int num);
    void set_chart1_layout();
    void set_chart2_layout();
    void set_chart3_layout();
    void set_chart4_layout();
    void set_chart5_layout();

signals:
    void load_image(const QString&  file_name);
    void initUart(QSerialPortInfo info);//发送给子线程的串口初始化信号
    void initUart485(QSerialPortInfo info);//发送给子线程的串口初始化信号
    void connectGateway(QString str);
    void loginSys();




public:
    TableWidget *chart1;
    TableWidget *chart2;
    TableWidget *chart3;

    TableWidget *chart4;
    TableWidget *chart5;

    quint16 dht_lose_flag;
    bool dht_no_page2;


public slots:
    void updateDHTSlot(int node, int humiture,int temprature);
    void dht_lose_slot(quint16 flag);
    void gotoSystem(int flag);
    void update_UI();


private slots:
    //void on_pushButton_clicked();
    void dealNum();

    void closeAPP();

    void on_pushButtonDel_clicked();

    void on_pushButtonConnect_clicked();


    void show_next();
    void show_prev();
    void show_image(const QString& picture_path);
    //串口选择下拉框槽函数
    //void on_comboBox_currentIndexChanged(const QString &arg1);
    //处理关闭窗口/应用程序的操作
    //主要是为了结束子线程
    void dealClose();
    //void showImage(QImage image);

    void on_pushButtonExit_clicked();
    void my_Init();
    void change_left();
    void change_right();



private:
    Ui::MyWidget *ui;
    QString resultStr;  //结果数
    QString dir_str;  //目录

    SearchFilter* m_widget_search_filter;
    ImageViewer* m_widget_image_viewr;
    frmMain * diy_control;

    MyThread *myT;//自定义对象指针--将要放入子线程

    QThread *thread;//子线程--负责串口数据的读取

    MyThread *uart485_module;//自定义对象指针--将要放入子线程

    QThread *uart485_thread;//子线程--负责串口数据的读取

    MyTCP *mytcp_obj;
    QThread * mytcp_thread;

    MyTcp_Pic *mytcp_pic_obj;
    QThread * mytcp_pic_thread;


    QStringList filters;
    headtitle * photo_title;
    headtitle * dht_title1;
    headtitle * dht_title2;


    int page_index;
    MyDHT * dht_items1;
    MyDHT * dht_items2;
    MyDHT * dht_items;


    char zigbee_exist;
    char uart485_exist;


};

#endif // MYWIDGET_H
