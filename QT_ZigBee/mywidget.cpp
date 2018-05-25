#include "mywidget.h"
#include "ui_mywidget.h"
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include "frmmain.h"
#include "tablewidget.h"
#include "customtablemodel.h"
#include <QString>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

typedef QMap<QDateTime,qreal> PH_MAP;

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{


    mytcp_obj = NULL;//dht+ph+light+cundoctive
    mytcp_thread = NULL;

    mytcp_pic_obj = NULL;//picture
    mytcp_pic_thread = NULL;

    mytcp_obj =new MyTCP;
    mytcp_thread = new QThread(this);
    //mytcp_obj->queryTime();
    mytcp_obj->moveToThread(mytcp_thread);
    mytcp_thread->start();



    sleep(1);
    qDebug()<<"lzjsqn\r\n";
    system("hwclock -w");
    system("hwclock -s");
    system("hwclock -w");
    system("date");
    qDebug()<<"lzjsqn\r\n";


    ui->setupUi(this);
    ui->login_lable->adjustSize();
    page_index = 0;
    qDebug()<<ui->stackedWidget->indexOf(ui->LoginPage);

    dir_str = QDir::currentPath();
    dir_str += "/";
    qDebug()<<dir_str;

    filters << "*.jpg";

    dht_title1 = new headtitle(QString("://src_img/r_left.png"),QString("节点温湿度数据"),QString("://src_img/g_right.png"),0);
    dht_title2 = new headtitle(QString("://src_img/g_left.png"),QString("节点温湿度数据"),QString("://src_img/p_right.png"),0);

    connect(dht_title1,&headtitle::left,this,&MyWidget::change_left);
    connect(dht_title1,&headtitle::right,this,&MyWidget::change_right);
    connect(dht_title2,&headtitle::left,this,&MyWidget::change_left);
    connect(dht_title2,&headtitle::right,this,&MyWidget::change_right);


    diy_control = new frmMain();
    ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //ui->label->setDisabled(true);

    connect(ui->pushButton0, &QPushButton::clicked, this, &MyWidget::dealNum);
    connect(ui->pushButton1, &QPushButton::clicked, this, &MyWidget::dealNum);
    connect(ui->pushButton2, &QPushButton::clicked, this, &MyWidget::dealNum);
    connect(ui->pushButton3, &QPushButton::clicked, this, &MyWidget::dealNum);
    connect(ui->pushButton4, &QPushButton::clicked, this, &MyWidget::dealNum);
    connect(ui->pushButton5, &QPushButton::clicked, this, &MyWidget::dealNum);
    connect(ui->pushButton6, &QPushButton::clicked, this, &MyWidget::dealNum);
    connect(ui->pushButton7, &QPushButton::clicked, this, &MyWidget::dealNum);
    connect(ui->pushButton8, &QPushButton::clicked, this, &MyWidget::dealNum);
    connect(ui->pushButton9, &QPushButton::clicked, this, &MyWidget::dealNum);
    connect(ui->pushButtonDot, &QPushButton::clicked, this, &MyWidget::dealNum);

    diy_control->setBtnQss(ui->pushButton1, "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");
    diy_control->setBtnQss(ui->pushButton2, "#1ABC9C", "#E6F8F5", "#2EE1C1", "#FFFFFF", "#16A086", "#A7EEE6");
    diy_control->setBtnQss(ui->pushButton3, "#3498DB", "#FFFFFF", "#5DACE4", "#E5FEFF", "#2483C7", "#A0DAFB");
    //diy_control->setBtnQss(ui->label, "#E74C3C", "#FFFFFF", "#EC7064", "#FFF5E7", "#DC2D1A", "#F5A996");
    diy_control->setBtnQss(ui->pushButtonExit, "#E74C3C", "#FFFFFF", "#EC7064", "#FFF5E7", "#DC2D1A", "#F5A996");
    diy_control->setBtnQss(ui->pushButton5, "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");
    diy_control->setBtnQss(ui->pushButton6, "#1ABC9C", "#E6F8F5", "#2EE1C1", "#FFFFFF", "#16A086", "#A7EEE6");
    diy_control->setBtnQss(ui->pushButton9, "#3498DB", "#FFFFFF", "#5DACE4", "#E5FEFF", "#2483C7", "#A0DAFB");
    diy_control->setBtnQss(ui->pushButton7, "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");
    diy_control->setBtnQss(ui->pushButton8, "#1ABC9C", "#E6F8F5", "#2EE1C1", "#FFFFFF", "#16A086", "#A7EEE6");
    diy_control->setBtnQss(ui->pushButton4, "#3498DB", "#FFFFFF", "#5DACE4", "#E5FEFF", "#2483C7", "#A0DAFB");
    diy_control->setBtnQss(ui->pushButtonDel, "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");
    diy_control->setBtnQss(ui->pushButtonDot, "#1ABC9C", "#E6F8F5", "#2EE1C1", "#FFFFFF", "#16A086", "#A7EEE6");
    diy_control->setBtnQss(ui->pushButton0, "#3498DB", "#FFFFFF", "#5DACE4", "#E5FEFF", "#2483C7", "#A0DAFB");
    diy_control->setTextEdit(ui->textEdit,"#DCE4EC", "#34495E");
    diy_control->setBtnQss(ui->pushButtonConnect, "#1ABC9C", "#E6F8F5", "#2EE1C1", "#FFFFFF", "#16A086", "#A7EEE6");

    m_widget_search_filter = new SearchFilter();  //文件浏览
    m_widget_image_viewr = new ImageViewer();  //图片浏览

    m_widget_search_filter->setMinimumWidth(400);
    m_widget_search_filter->setMaximumWidth(400);

    photo_title = new headtitle(QString("://src_img/p_left.png"),QString("图片浏览"),QString("://src_img/r_right.png"),0);

    QHBoxLayout *view_layout = new QHBoxLayout();
    view_layout->addWidget(m_widget_search_filter);
    view_layout->addWidget(m_widget_image_viewr);
    view_layout->setMargin(0);
    view_layout->setSpacing(0);

    QVBoxLayout* main_layout = new QVBoxLayout();
    main_layout->addWidget(photo_title);
    main_layout->addLayout(view_layout);
    main_layout->setMargin(0);
    main_layout->setSpacing(0);


    ui->PhotoPage->setLayout(main_layout);

    connect(photo_title,&headtitle::left,this,&MyWidget::change_left);
    connect(photo_title,&headtitle::right,this,&MyWidget::change_right);
    m_widget_search_filter->Init(dir_str, filters);

    connect(m_widget_search_filter, &SearchFilter::signal_current_select_file, this, &MyWidget::show_image);
    connect(m_widget_image_viewr, &ImageViewer::signal_next, this,  &MyWidget::show_next);
    connect(m_widget_image_viewr, &ImageViewer::signal_prev, this,  &MyWidget::show_prev);
    connect(m_widget_image_viewr, &ImageViewer::signal_close, this,  &MyWidget::closeAPP);
    connect(this, &MyWidget::load_image, this, &MyWidget::show_image);


    zigbee_exist = 0;
    uart485_exist = 0;
    dht_lose_flag = 0;


    //对于子线程的东西（将被移入子线程的自定义对象以及线程对象），最好定义为指针
    myT = NULL;//将被子线程处理的自定义对象不能在主线程初始化的时候指定父对象
    thread = NULL;//初始化子线程线程

    uart485_module = NULL;
    uart485_thread = NULL;

    /****************************/



    chart1 = NULL;
    chart2 = NULL;
    chart3 = NULL;
    chart4 = NULL;
    chart5 = NULL;

    //connect(mytcp_obj,&MyTCP::updateUI,this,&MyWidget::update_UI);


    mytcp_pic_obj =new MyTcp_Pic;//picture
    mytcp_pic_thread = new QThread(this);
    mytcp_pic_obj->moveToThread(mytcp_pic_thread);
    mytcp_pic_thread->start();

    //1. 获取host IP

    QFile file("zigbee_config.txt");
    file.open(QIODevice::ReadWrite);
    QByteArray ba=file.read(file.bytesAvailable());
    QString ip_str=QString(ba);
    qDebug()<<ip_str;
    ui->textEdit->setText(ip_str);
    file.close();

    //2. 连接到host
    connect(this,&MyWidget::connectGateway,mytcp_obj,&MyTCP::connectToGateway);
    connect(this,&MyWidget::connectGateway,mytcp_pic_obj,&MyTcp_Pic::connectToGateway);
    //emit connectGateway(ip_str);


    //绑定登录系统按钮和断开当前tcp连接的的信号与槽
    connect(this,&MyWidget::loginSys,mytcp_obj,&MyTCP::loginSystem);
    connect(this,&MyWidget::loginSys,mytcp_pic_obj,&MyTcp_Pic::loginSystem);

    connect(mytcp_obj,&MyTCP::connectOK,this,&MyWidget::gotoSystem);
    connect(mytcp_pic_obj,&MyTcp_Pic::connectOK,this,&MyWidget::gotoSystem);


    qDebug()<<"mytcp*******************************************";

    uart485_module = new MyThread;//将被子线程处理的自定义对象不能在主线程初始化的时候指定父对象
    qRegisterMetaType<QMap<QDateTime,qreal>>("QMap<QDateTime,qreal>");

    set_chart1_layout();
    set_chart2_layout();
    set_chart3_layout();
    set_chart4_layout();
    set_chart5_layout();


    //绑定/连接关闭应用程序窗口的信号和主线程的dealClose槽函数
    connect(this, &MyWidget::destroyed, this, &MyWidget::dealClose);
    detectSerial();//探测当前系统可用的串口列表

    dht_no_page2 = false;
    setDHTLayout(DHT_NUMBERS);

    ui->stackedWidget->setCurrentIndex(0);
    qDebug() << "main thread:========================"<< QThread::currentThread() ;


    //emit loginSys();
    //emit loginSys();


    connect(uart485_module,&MyThread::UpdateTime,chart3, &TableWidget::ShowTime);

}
void MyWidget::update_UI()
{

#if 0
    delete chart1;
    delete chart2;
    delete chart3;

    chart1 = new TableWidget ();
    chart2 = new TableWidget ();
    chart3 = new TableWidget ();

    set_chart1_layout();
    set_chart2_layout();
    set_chart3_layout();
#endif

    //chart1->model->InitData();
    qDebug()<<"jkskjkjkjkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk\r\n";
}

void MyWidget::set_one_page(MyDHT * dht_items,QWidget * widget,int num)
{
     QHBoxLayout *line1_dht = new QHBoxLayout();
     QHBoxLayout *line2_dht= new QHBoxLayout();
     QVBoxLayout *v_layout= new  QVBoxLayout();
     QVBoxLayout *main_layout= new  QVBoxLayout();
     QWidget * tmp = new QWidget();

    int line2_cnt = num /2;
    int line1_cnt = num - line2_cnt;

    for(int i = 0; i < line1_cnt;i++)
    {
        line1_dht->addWidget(dht_items + i);
    }

    for(int i = line1_cnt; i < num;i++)
    {

        line2_dht->addWidget(dht_items + i);
    }


    v_layout->addLayout(line1_dht);
    v_layout->addLayout(line2_dht);

    tmp->setLayout(v_layout);
    tmp->setMinimumHeight(530);

    if(widget == ui->DHTPage)
        main_layout->addWidget(dht_title1);
    else
        main_layout->addWidget(dht_title2);


    main_layout->addWidget(tmp);
    main_layout->setMargin(0);
    main_layout->setSpacing(0);

    widget->setLayout(main_layout);
}

void MyWidget::setDHTLayout(int num)
{

    int count2 = 0;
    int count1 = 0;

    if(num <= 0)
    {
        qDebug()<<"dht nodes numbers err....";
        return;
    }

    if(num > 8){
        count2 = num / 2;
    }else {
        count2 = 0;
    }
    count1 = num -count2;

    dht_items = new MyDHT[num];
    dht_items1 = dht_items;

    set_one_page(dht_items1,ui->DHTPage,count1);


    if(count2 > 0)
    {
        dht_items2 = dht_items + count1;
        set_one_page(dht_items2,ui->DHTPage2,count2);

    }else{
        dht_no_page2 = true;
    }

    for(int i = 0;i < dht_items->dht_count();i++)
    {
        (dht_items+i)->meter->setValue(10+i);
        (dht_items+i)->thermometer->setValue(10+2*i);
        (dht_items + i)->set_info(QString("节点 %1").arg(i+1));
        //printf("(init+%d) : %0X\n",i,(dht_items+i));
    }

}

void MyWidget::updateDHTSlot(int node, int humiture,int temprature)
{
    if((dht_items +node)->blink_flag == true)
    {
        (dht_items+node)->meter->setBackground(Qt::black);
        (dht_items+node)->thermometer->setBackground(Qt::blue);
    }


    (dht_items +node)->blink_flag = true;//避免在写入新数据的时候，触摸发生引起冲突
    (dht_items+node)->meter->setValue(humiture);
    (dht_items+node)->thermometer->setValue(temprature);
     dht_lose_flag &= ~(1 << node);
    (dht_items +node)->blink_flag = false;

    (dht_items +node)->setBlink(false);

}



void MyWidget::dht_lose_slot(quint16 index)
{

    //1. 把温湿度都设置为0，再调整背景改变
    (dht_items+index)->meter->setValue(0);
    (dht_items+index)->thermometer->setValue(0);


    qDebug()<<"dht_lose_slot in ++++++++++++++++++++++++++++++++++++++++++++";
    if((dht_items +index)->blink_flag == true)
        return;
    qDebug()<<"set in ++++++++++++++++++++++++++++++++++++++++++++++++++++++";
    (dht_items +index)->setBlink(true);
    qDebug()<<"set out +++++++++++++++++++++++++++++++++++++++++++++++++++++";
}


void MyWidget::set_chart1_layout()
{
    chart1 = new TableWidget ();
    chart1->axisY->setRange(4,9);
    chart1->setTitle(PH_TABLE);
    QVBoxLayout * main_layout = new QVBoxLayout();
    headtitle *chart_title = new headtitle(QString("://src_img/p_left.png"),QString("PH值"),QString("://src_img/g_right.png"),0);
    connect(chart_title,&headtitle::left,this,&MyWidget::change_left);
    connect(chart_title,&headtitle::right,this,&MyWidget::change_right);
    connect(chart1,&TableWidget::changeShowDate,uart485_module,&MyThread::resolveDateChange);
    connect(uart485_module,&MyThread::DynamicShowPH,chart1,&TableWidget::updateMVC_PH,Qt::QueuedConnection);
    connect(uart485_module,&MyThread::updateComboxPH,chart1,&TableWidget::updateCombox,Qt::QueuedConnection);

    main_layout->addWidget(chart_title);
    main_layout->addWidget(chart1);
    main_layout->setMargin(0);
    main_layout->setSpacing(0);
    ui->Qchart1->setLayout(main_layout);

    qDebug()<<QSqlDatabase::drivers();
}
void MyWidget::set_chart2_layout()
{
    chart2 = new TableWidget ();
    chart2->axisY->setRange(0,100000);//默认是0---200000
    chart2->axisY->setLabelFormat("%d");
    chart2->setTitle(LIGHT_TABLE);
    QVBoxLayout * main_layout = new QVBoxLayout();
    headtitle *chart_title = new headtitle(QString("://src_img/g_left.png"),QString("光照强度"),QString("://src_img/r_right.png"),0);
    connect(chart_title,&headtitle::left,this,&MyWidget::change_left);
    connect(chart_title,&headtitle::right,this,&MyWidget::change_right);
    connect(chart2,&TableWidget::changeShowDate,uart485_module,&MyThread::resolveDateChange);
    connect(uart485_module,&MyThread::DynamicShowLight,chart2,&TableWidget::updateMVC_PH,Qt::QueuedConnection);
    connect(uart485_module,&MyThread::updateComboxLight,chart2,&TableWidget::updateCombox,Qt::QueuedConnection);
    main_layout->addWidget(chart_title);
    main_layout->addWidget(chart2);
    main_layout->setMargin(0);
    main_layout->setSpacing(0);
    ui->Qchart2->setLayout(main_layout);

}
void MyWidget::set_chart3_layout()
{
    chart3 = new TableWidget ();
    chart3->axisY->setRange(0,1000);//默认是0---1000
    chart3->axisY->setLabelFormat("%d");
    chart3->setTitle(CONDUCT_TABLE);
    QVBoxLayout * main_layout = new QVBoxLayout();
    headtitle *dht_title = new headtitle(QString("://src_img/r_left.png"),QString("电导率"),QString("://src_img/p_right.png"),0);
    connect(dht_title,&headtitle::left,this,&MyWidget::change_left);
    connect(dht_title,&headtitle::right,this,&MyWidget::change_right);

    //未生效
    connect(chart3,&TableWidget::changeShowDate,uart485_module,&MyThread::resolveDateChange);


    connect(uart485_module,&MyThread::DynamicShowEC,chart3,&TableWidget::updateMVC_PH,Qt::QueuedConnection);

    //未生效
    connect(uart485_module,&MyThread::updateComboxEC,chart3,&TableWidget::updateCombox,Qt::QueuedConnection);

    main_layout->addWidget(dht_title);
    main_layout->addWidget(chart3);
    main_layout->setMargin(0);
    main_layout->setSpacing(0);
    ui->Qchart3->setLayout(main_layout);

}

void MyWidget::set_chart4_layout()
{
    chart4 = new TableWidget ();
    chart4->axisY->setRange(0,40);//默认是0---1000
    chart4->axisY->setLabelFormat("%d");
    chart4->setTitle(WENDU_TABLE);
    QVBoxLayout * main_layout = new QVBoxLayout();
    headtitle *dht_title = new headtitle(QString("://src_img/r_left.png"),QString("温度"),QString("://src_img/p_right.png"),0);
    connect(dht_title,&headtitle::left,this,&MyWidget::change_left);
    connect(dht_title,&headtitle::right,this,&MyWidget::change_right);

    //该功能暂时被取消，没有在改变日期的时候，实现查询功能---目前只是显示最新的数据库数据
    connect(chart4,&TableWidget::changeShowDate,uart485_module,&MyThread::resolveDateChange);


    //此时的myT还可能是一个空指针，并不会绑定信号和槽--需要在确保myT实例化以后在绑定--对于温湿度，是在detectSerial里面绑定，其他的直接绑定
    //connect(myT,&MyThread::DynamicShowWenDu,chart4,&TableWidget::updateMVC_PH,Qt::QueuedConnection);

    //该功能暂时被取消了，没有用到自动更新combox的日期
    connect(uart485_module,&MyThread::updateComboxEC,chart3,&TableWidget::updateCombox,Qt::QueuedConnection);

    main_layout->addWidget(dht_title);
    main_layout->addWidget(chart4);
    main_layout->setMargin(0);
    main_layout->setSpacing(0);
    ui->Qchart4->setLayout(main_layout);

}

void MyWidget::set_chart5_layout()
{
    chart5 = new TableWidget ();
    chart5->axisY->setRange(0,100);//默认是0---1000
    chart5->axisY->setLabelFormat("%d");
    chart5->setTitle(SHIDU_TABLE);
    QVBoxLayout * main_layout = new QVBoxLayout();
    headtitle *dht_title = new headtitle(QString("://src_img/r_left.png"),QString("湿度"),QString("://src_img/p_right.png"),0);
    connect(dht_title,&headtitle::left,this,&MyWidget::change_left);
    connect(dht_title,&headtitle::right,this,&MyWidget::change_right);

    //该功能暂时被取消，没有在改变日期的时候，实现查询功能---目前只是显示最新的数据库数据
    connect(chart4,&TableWidget::changeShowDate,uart485_module,&MyThread::resolveDateChange);


    //此时的myT还可能是一个空指针，并不会绑定信号和槽--需要在确保myT实例化以后在绑定--对于温湿度，是在detectSerial里面绑定，其他的直接绑定
    //connect(myT,&MyThread::DynamicShowWenDu,chart4,&TableWidget::updateMVC_PH,Qt::QueuedConnection);

    //该功能暂时被取消了，没有用到自动更新combox的日期
    connect(uart485_module,&MyThread::updateComboxEC,chart3,&TableWidget::updateCombox,Qt::QueuedConnection);

    main_layout->addWidget(dht_title);
    main_layout->addWidget(chart5);
    main_layout->setMargin(0);
    main_layout->setSpacing(0);
    ui->Qchart5->setLayout(main_layout);

}



void MyWidget::dealClose()
{
    if(thread != NULL && myT != NULL)
    {
        if(thread->isRunning() == false)
         {
                return;
         }

        //2.如果调用的是子线程的函数（对象已被放入子线程，其成员函数均在子线程）
        //需要在子线程退出的之前调用

        myT->setFlag(true);//更新子线程的isStop标志--结束子线程的处理函数
        //3.退出子线程要显示的调用这两个函数，否则主线程退出但子线程还在运行
        thread->quit();
        //回收资源
        thread->wait();

        //4. 将要被放入子线程的对象在主线程初始化（构造）的时候不能指定父对象，且需要在子线程结束以后显示delete
        delete myT;
    }


    if(uart485_thread != NULL && uart485_module != NULL)
    {

        /*回收485子线程*/
        if(uart485_thread->isRunning() == false)
         {
                return;
         }

        uart485_module->setFlag(true);
        uart485_thread->quit();
        uart485_thread->wait();
        delete uart485_module;

    }

    /*处理温湿度节点对象*/
    delete[] dht_items;

    if(mytcp_thread != NULL && mytcp_obj != NULL)
    {

        /*回收发送DHT等的TCP子线程*/
        if(mytcp_thread->isRunning() == false)
         {
                return;
         }

        mytcp_obj->setFlag(true);
        mytcp_thread->quit();
        mytcp_thread->wait();
        delete mytcp_obj;

    }

    if(mytcp_pic_thread != NULL && mytcp_pic_obj != NULL)
    {

        /*回收发送PIC的子线程*/
        if(mytcp_pic_thread->isRunning() == false)
         {
                return;
         }

        mytcp_pic_obj->setFlag(true);
        mytcp_pic_thread->quit();
        mytcp_pic_thread->wait();
        delete mytcp_pic_obj;

    }
}


#define QUERY_ZIGBEE  ("are u zigbee")
#define ACK_ZIGBEE  ("I am ZigBee")
void MyWidget::detectSerial()
{    
    //绑定信号和曹的时候，如果带参数，在QT5中可以直接给出信号和槽函数名即可
    //但是，如果所传递的参数类型是未注册（非本地默认识别的可传递类型）的，需要在绑定之前进行注册
    qRegisterMetaType<QSerialPortInfo>("QSerialPortInfo");
    qRegisterMetaType<QImage>("QImage'");


    //获取可用串口列表
    QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
    if(infos.isEmpty())//系统无可用串口
    {
        //ui->comboBox->addItem("无效");//在串口选择下拉框显示“无效”
        QMessageBox::information(this, "串口检测", "没有可用串口！");
        return;
    }
    //ui->comboBox->addItem("串口");//如果有可用串口则在串口选择下拉框显示“串口”

    //将每个可用串口号作为一个条目添加到串口选择下拉框
    foreach (QSerialPortInfo info, infos) {
        //ui->comboBox->addItem(info.portName());
        if(info.portName().contains("ttyUSB"))
        {
            qDebug()<<"Aloha";

            QSerialPort* serial = new QSerialPort();
            if(serial->isOpen())//先关闭
                serial->close();
            serial->setPort(info);//设置串口号--就是从下拉框选择的串口号
            serial->open(QIODevice::ReadWrite);         //读写打开
            serial->setBaudRate(QSerialPort::Baud115200);  //波特率
            serial->setDataBits(QSerialPort::Data8); //数据位
            serial->setParity(QSerialPort::NoParity);    //无奇偶校验
            serial->setStopBits(QSerialPort::OneStop);   //停止位
            serial->setFlowControl(QSerialPort::NoFlowControl);  //无控制

            QString send_array = QString(QUERY_ZIGBEE);
            QByteArray tmp ;

            tmp.clear();
            tmp.append(send_array.toUtf8());
            qint64 ret = serial->write(tmp);

            qDebug()<<"send "<<ret<<QUERY_ZIGBEE;

            int cnt_read = 0;//已经读取到的总字节数
            int cnt_need = QString(ACK_ZIGBEE).length();//还需要的字节数（只针对完整读取指定长度的数据包）
            int cnt_tmp = 0;//存放本次读取到的临时长度
            char tmp_buf[32] = {0};
            char ack_buf[256] = {0};
            while(1)
            {

                qDebug()<<"go into while";
                if(serial->bytesAvailable() >= 1 || serial->waitForReadyRead(1000))//有可读数据再去读
                {
                    qDebug()<<"youshuju";
                    cnt_need = QString(ACK_ZIGBEE).length() - cnt_read;//更新当前还需要读取的字节数
                    cnt_tmp = serial->read(tmp_buf,cnt_need);
                    if(cnt_tmp > 0)//读取是否成功
                    {

                        memcpy(ack_buf+cnt_read,tmp_buf,cnt_tmp);//每次都把数据累加到cmd_buf
                        cnt_read +=  cnt_tmp;
                        if(cnt_read == QString(ACK_ZIGBEE).length())//是否读取了指定长度
                        {
                            //if(strcmp(ack_buf,ACK_ZIGBEE) == 0)
                            if(1)
                            {
                                if(serial->isOpen())//先关闭
                                    serial->close();
                                qDebug()<<"find zigbee";
                                zigbee_exist = 1;
                                //对于子线程的东西（将被移入子线程的自定义对象以及线程对象），最好定义为指针
                                myT = new MyThread;//将被子线程处理的自定义对象不能在主线程初始化的时候指定父对象
                                thread = new QThread(this);//初始化子线程线程
                                myT->moveToThread(thread);//将自定义对象移交给子线程，从此子线程控制他的成员函数

                                //启动子线程，但是没有启动真正的子线程处理函数，
                                //只是让子线程对象开始监控移交给他的相关对象
                                thread->start();

                                //连接子线程的isDone信号到主线程的serialDisplay槽函数，显示串口接收到的数据

                                connect(myT,&MyThread::ImageOK,this,&MyWidget::show_image);
                                connect(myT,&MyThread::ImageOK,this,&MyWidget::my_Init);
                                connect(myT,&MyThread::updateDHTSignal,this,&MyWidget::updateDHTSlot);
                                connect(myT,&MyThread::dht_lose,this,&MyWidget::dht_lose_slot);

                                connect(myT,&MyThread::DynamicShowWenDu,chart4,&TableWidget::updateMVC_PH,Qt::QueuedConnection);
                                connect(myT,&MyThread::DynamicShowShiDu,chart5,&TableWidget::updateMVC_PH,Qt::QueuedConnection);

                                //连接主线程的initUart信号到子线程的initSerial槽函数，开始串口初始化
                                connect(this,&MyWidget::initUart,myT,&MyThread::initSerial);


                                emit initUart(info);//发送串口初始化信号
                                break;
                            }
                        }else{
                            continue;
                        }
                    }
                    else if(cnt_tmp == -1)//读串口失败
                    {
                        qDebug()<<"read err";
                    }
                    //continue;
                    break;
                }


                qDebug()<<"not zigbee";
                if(serial->isOpen())//先关闭
                    serial->close();
                uart485_exist = 1;



                uart485_thread = new QThread(this);//初始化子线程线程
                uart485_module->moveToThread(uart485_thread);//将自定义对象移交给子线程，从此子线程控制他的成员函数


                //启动子线程，但是没有启动真正的子线程处理函数，
                //只是让子线程对象开始监控移交给他的相关对象
                uart485_thread->start();

                connect(this,&MyWidget::initUart485,uart485_module,&MyThread::initUart485);
                emit initUart485(info);//发送串口初始化信号
                break;
            }

        }

    }


    connect(uart485_module,&MyThread::tcp,mytcp_obj,&MyTCP::tcp_send);
    connect(myT,&MyThread::tcp,mytcp_obj,&MyTCP::tcp_send);
    connect(myT,&MyThread::send_pic,mytcp_pic_obj,&MyTcp_Pic::startTransfer);

}

MyWidget::~MyWidget()
{
    delete ui;
}


void MyWidget::change_left()
{
    if(0 == --page_index)
        page_index = ui->stackedWidget->count()-1;
    if(page_index == 3 && dht_no_page2 == true)
        page_index = 2;

    ui->stackedWidget->setCurrentIndex(page_index);

    if(SearchFilter::show_flag == 1)
    {
        m_widget_search_filter->key_board->close();
        SearchFilter::show_flag = 0;
    }

}

void MyWidget::change_right()
{
    if(ui->stackedWidget->count() == ++page_index)
        page_index = 1;

    if(page_index == 3 && dht_no_page2 == true)
        page_index = 4;
    ui->stackedWidget->setCurrentIndex(page_index);

    if(SearchFilter::show_flag == 1)
    {
        m_widget_search_filter->key_board->close();
        SearchFilter::show_flag = 0;
    }
}

void MyWidget::my_Init()
{
    m_widget_search_filter->Init(dir_str, filters);
}



void MyWidget::closeAPP()
{
    if(SearchFilter::show_flag == 1)
    {
        m_widget_search_filter->key_board->close();
        SearchFilter::show_flag = 0;
    }
    ui->stackedWidget->setCurrentWidget(ui->LoginPage);
    page_index = 0;
}

//数字键处理
void MyWidget::dealNum()
{
    //获取信号发送者
    QObject * mySender = sender();
    //转换为按钮类型
    QPushButton *p = (QPushButton *)mySender;
    if(NULL != p)
    {
        //获取按钮的内容
        QString numStr = p->text();
        resultStr += numStr;
        ui->textEdit->setText( resultStr );
        //初始化字符串结果，清空
        if(resultStr.length() == 20)
            resultStr.clear();
    }

}



void MyWidget::on_pushButtonDel_clicked()
{
    if(resultStr.size() == 1)
    {
        resultStr.clear();
        ui->textEdit->clear();
    }
    else
    {
        resultStr.chop(1); //截断最后一位字符
        ui->textEdit->setText(resultStr);
    }
}

void MyWidget::on_pushButtonConnect_clicked()
{
    //通过当前编辑框的IP地址更新配置文件
    QFile data("zigbee_config.txt");
    if (data.open(QFile::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&data);
        out << ui->textEdit->toPlainText();
        data.close();
    }
    qDebug()<<ui->textEdit->toPlainText();
    qDebug()<<resultStr;


    //QMessageBox::information(this, "被骗了吧", "Mason Is HandSome！");
    emit loginSys();
}


void MyWidget::gotoSystem(int flag)
{
    static int cnt = 0;
    if(1 == flag){
        QMessageBox::information(this, "Connect Fail", "连接失败，请重试！");
        return ;
    }

    cnt++;

    if(cnt < 2)
        return ;

    cnt = 0;
    ui->stackedWidget->setCurrentIndex(++page_index);
    emit load_image(m_widget_search_filter->init_file_name);
}



void MyWidget::show_next()
{
    if(SearchFilter::show_flag == 1)
    {
        m_widget_search_filter->key_board->close();
        SearchFilter::show_flag = 0;
    }
    m_widget_search_filter->SetCurrentSelectFile_Next();
}

void MyWidget::show_prev()
{
    if(SearchFilter::show_flag == 1)
    {
        m_widget_search_filter->key_board->close();
        SearchFilter::show_flag = 0;
    }
    m_widget_search_filter->SetCurrentSelectFile_Prev();
}

void MyWidget::show_image(const QString &picture_path)
{
    qDebug()<<picture_path;
    m_widget_image_viewr->loadFile(picture_path);
}



void MyWidget::on_pushButtonExit_clicked()
{
    dealClose();
    this->close();
}


