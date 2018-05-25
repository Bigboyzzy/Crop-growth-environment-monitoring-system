#include "mydht.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QDebug>

int MyDHT::cnt=0;
QMutex mutex;
MyDHT::MyDHT(QWidget *parent) : QWidget(parent)
{
    thermometer=new QcwThermometer();
    thermometer->resize(56, 200);
    thermometer->setMaximumWidth(80);
    thermometer->setMinimumWidth(80);
    thermometer->setValue(10);
    thermometer->setMaxValue(50);
    thermometer->setMinValue(0);

    meter=new QcwMeter();
    meter->resize(200, 200);
    meter->setMinimumWidth(160);

    meter->setMaxValue(100);
    meter->setMinValue(0);
    meter->setValue(10);


    node_info = new QLabel();
    node_info->adjustSize();
    node_info->setMaximumHeight(20);
    node_info->setStyleSheet(QString::fromUtf8("font: 10pt \"Sans Serif\";"));

    tmp = NULL;
    blink_flag = false;
    blinkDHT_cnt = 0;
    dht_timer = NULL;

    QSpacerItem* left = new QSpacerItem(75, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem* right = new QSpacerItem(75, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QHBoxLayout * humiture_layout = new QHBoxLayout();

    humiture_layout->addSpacerItem(left);
    humiture_layout->addWidget(meter);
    humiture_layout->addWidget(thermometer);
    humiture_layout->addSpacerItem(right);
    humiture_layout->setSpacing(0);
    humiture_layout->setMargin(0);

    //printf("the value of &bird is::0x%0X\n",blink_timer);

    QSpacerItem* left1 = new QSpacerItem(75, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem* right2 = new QSpacerItem(75, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QHBoxLayout * label_layout = new QHBoxLayout();

    label_layout->addSpacerItem(left1);
    label_layout->addWidget(node_info);
    label_layout->addSpacerItem(right2);
    label_layout->setSpacing(0);
    label_layout->setMargin(0);

    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->addLayout(humiture_layout);
    main_layout->addLayout(label_layout);
    main_layout->setSpacing(0);
    main_layout->setMargin(0);

    this->setLayout(main_layout);
    cnt++;
}

MyDHT::MyDHT(MyDHT *obj)
{

    thermometer=new QcwThermometer();
    thermometer->resize(obj->thermometer->width()*2, obj->thermometer->height()*2);
    thermometer->setMaximumWidth(obj->thermometer->maximumWidth()*2);
    thermometer->setMinimumWidth(obj->thermometer->minimumWidth()*2);
    thermometer->m_value = obj->thermometer->value();
    thermometer->m_maxValue = obj->thermometer->maxValue();
    thermometer->m_minValue = obj->thermometer->minValue();

    meter=new QcwMeter();
    meter->resize(obj->meter->width()*2, obj->meter->height()*2);
    meter->setMinimumWidth(obj->meter->minimumWidth()*2);

    meter->m_value = obj->meter->value();
    meter->m_maxValue = obj->meter->maxValue();
    meter->m_minValue = obj->meter->minValue();

    node_info = new QLabel();
    node_info->adjustSize();
    node_info->setMaximumHeight(20*2);
    node_info->setStyleSheet(QString::fromUtf8("font: 20pt \"Sans Serif\";"));
    set_info(obj->get_info());

    tmp = NULL;
    blink_flag = false;
    blinkDHT_cnt = 0;
    dht_timer = NULL;



    QSpacerItem* left = new QSpacerItem(75, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem* right = new QSpacerItem(75, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QHBoxLayout * humiture_layout = new QHBoxLayout();

    humiture_layout->addSpacerItem(left);
    humiture_layout->addWidget(meter);
    humiture_layout->addWidget(thermometer);
    humiture_layout->addSpacerItem(right);
    humiture_layout->setSpacing(0);
    humiture_layout->setMargin(0);

    //printf("the value of &bird is::0x%0X\n",blink_timer);

    QSpacerItem* left1 = new QSpacerItem(75, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem* right2 = new QSpacerItem(75, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QHBoxLayout * label_layout = new QHBoxLayout();

    label_layout->addSpacerItem(left1);
    label_layout->addWidget(node_info);
    label_layout->addSpacerItem(right2);
    label_layout->setSpacing(0);
    label_layout->setMargin(0);

    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->addLayout(humiture_layout);
    main_layout->addLayout(label_layout);
    main_layout->setSpacing(0);
    main_layout->setMargin(0);

    this->setLayout(main_layout);
    //update();
}



void MyDHT::set_info(QString str)
{
    node_info->setText(str);
}

int MyDHT::dht_count()
{
    return cnt;
}

//重写mousePressEvent事件,检测事件类型是不是点击了鼠标左键
void MyDHT::mousePressEvent(QMouseEvent *event)
{
#if 1
    if(blink_flag == true)
        return;
    //如果单击了就触发clicked信号
    //dht_timer->stop();
    if (event->button() == Qt::LeftButton)
    {
        tmp = new MyDHT(this);
        qDebug()<<"prese+++++++++++++++++++++";

        tmp->setGeometry(312,100,400,400);
        tmp->show();

    }
    //将该事件传给父类处理
    QWidget::mousePressEvent(event);
#else
    qDebug()<<"prese+++++++++++++++++++++";
#endif


}

QString MyDHT::get_info()
{
    return node_info->text();
}

void MyDHT::mouseReleaseEvent(QMouseEvent *event)
{
#if 1
    if(blink_flag == true)
        return;

    if(tmp != NULL)
    {
        tmp->close();
        delete tmp;
        tmp = NULL;
    }

    //将该事件传给父类处理
    QWidget::mousePressEvent(event);
#else
    qDebug()<<"release+++++++++++++++++++++";
#endif
}

void MyDHT::blinkDHT()
{
#ifdef ZJ_DEBUG
    //qDebug()<<"blink begin";
#endif
    if(blinkDHT_cnt % 2 == 0)
    {
        meter->setBackground(Qt::white);
        thermometer->setBackground(Qt::white);
    }
    else if (blinkDHT_cnt % 2 == 1)
    {
        meter->setBackground(Qt::black);
        thermometer->setBackground(Qt::blue);
    }

    blinkDHT_cnt++;
#ifdef ZJ_DEBUG
    //qDebug()<<"blink end";
#endif
}


void MyDHT::setBlink(bool cmd)
{
#ifdef  BLINK
    if(cmd == true && blink_flag == false)
    {
#ifdef ZJ_DEBUG
        //qDebug()<<"dht_timer begin";
#endif
        blink_flag = true;
        dht_timer = new QTimer();
        connect(dht_timer,&QTimer::timeout,this,&MyDHT::blinkDHT);
        dht_timer->start(500);
    }
    else if(dht_timer != NULL && cmd == false)
    {
        dht_timer->stop();
        delete dht_timer;
        dht_timer = NULL;
        blink_flag = false;
    }
#endif

    if(cmd == true && blink_flag == false)
    {

        blink_flag = true;
        meter->setBackground(Qt::white);
        thermometer->setBackground(Qt::white);
    }
    else if(cmd == false)
    {
        blink_flag = false;
    }

}


