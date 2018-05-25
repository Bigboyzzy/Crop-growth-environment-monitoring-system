#ifndef MYDHT_H
#define MYDHT_H

#include <QWidget>
#include "qcwmeter.h"
#include "qcwthermometer.h"
#include <QLabel>
#include <QMouseEvent>
#include <QTimer>
#include <QMutex>

class MyDHT: public QWidget
{
public:
    explicit MyDHT(QWidget *parent = 0);
    //MyDHT(QString info="",QWidget *parent = 0);
    MyDHT(MyDHT *obj);
    void set_info(QString str);
    QString get_info();
    int dht_count();
    //重写mousePressEvent事件
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    void blinkDHT();

    void setBlink(bool cmd);


public slots:


public:
    QcwThermometer* thermometer;
    QcwMeter *meter;
    QLabel * node_info;
    static int cnt;
    MyDHT *tmp;

    bool blink_flag;
    int blinkDHT_cnt;

    QTimer * dht_timer;



};

#endif // MYDHT_H
