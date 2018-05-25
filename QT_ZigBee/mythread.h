#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QThread>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>



//一个包的长度信息
#define REAL_LEN    64//有效数据最大长度
#define HDR_LEN     8//包头长度（固定不变）

//处理照片数据的状态标志（目前没用到--留待以后扩展）
//照片数据分为三部分（begin + 有效数据 +end）
#define BEGIN       0x01//开始处理照片--接收到含有begin的包
#define END         0x02//照片数据接收完毕--接收到含有end的包
#define READING     0x03//正在处理照片数据--照片的实际有效数据

//读串口的状态标志--每个包分为两部分读取（包头+有效数据）
#define READ_HDR            0x01//读取包头
#define READ_DATA           0x02//读取有效数据

/*返回值第七位和第八位有效*/
/*命令是{0x08,0x04,0x00,0x00,0x00,0x05,0x30,0x90}*/
#define CONDUCT_ADDR        (0x08)
#define CONDUCT_LENGTH      (15)

/*返回值第三位和第四位有效*/
/*命令是{0x02,0x03,0,0,0,0x01,0x84,0x39}*/
#define PH_DEV_ADDR        (0x02)
#define PH_DEV_LENGTH      (7)

/*返回值第三位到第四位有效*/
/*命令是{0x01,0x03,0,0,0,0x01,0x84,0x0A}*/
#define LIGHT_ADDR        (0x01)
#define LIGHT_LENGTH      (7)

/*485串口工作状态*/
#define READ_PH         0x01
#define READ_LIGHT      0x02
#define READ_CONDUCT    0x03


#define PLOT_NUM  96

#define PH_TABLE  (QString("ph"))
#define CONDUCT_TABLE  (QString("conduct"))
#define LIGHT_TABLE  (QString("light"))


#define WENDU_TABLE  (QString("wendu"))
#define SHIDU_TABLE  (QString("shidu"))

//#define DEBUG_TIME   1
#define DHT_NUMBERS  16

#define BASE 1

#define PH_TYPE 1
#define CONDUC_TYPE 2
#define LIGHT_TYPE 3
#define DHT_TYPE 4
#define PHOTO_TYPE 5
#define QUERY_TIME 6

#define BLINK

#define WAIT_DHT_PIC_TIME  (20*60)

//QString select_max_sql = "select max(id) from PH";
//QString update_sql = "update student set name = :name where id = :id";
//QString select_sql = "select id, name from student";

//QString delete_sql = "delete from student where id = ?";
//QString clear_sql = "delete from student";

class MyThread : public QObject
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);
    void initSerial(QSerialPortInfo info);//串口初始化
    void initUart485(QSerialPortInfo info);//串口初始化
    void readSerial();//子线程真正的处理函数--在没有结束程序的时候是死循环
    void readUart485();
    void updateSensorData();
    void setFlag(bool flag);//更新isStop标志


    void updateTable(QString str);
    void updateShow(QString str);
    void test(QString str);

    //读取指定长度的数据，但是返回的是本次读取到的实际字节数
    //实际读取的字节数可能比length小
    int  readFrameData(unsigned int length);
    void handleHead(); //处理包头数据
    void handlePhoto();//处理图片的有效数据
    void handleDHT();
    void sendToUart(QByteArray tmp);
    void sleep(int msec);

    int read_needed_data(int length,char *tmp_buf,char *result_buf);

    /*************************
     * 电导率操作函数
    *************************/

    void set_conduct_addr(unsigned char dev_addr,unsigned char addr);
    void get_conduct_addr(unsigned char dev_addr);
    void get_conduct_val(unsigned char dev_addr);

    /*************************
     * PH传感器操作函数
    *************************/
    void get_PH_val(unsigned char dev_addr);


    /*************************
     * 光照传感器操作函数
    *************************/
    void get_light_val(unsigned char dev_addr);


public:
    /******电导率测量值--目前所用的命令返回值是15个字节*****/
    //float temprature;//摄氏度
    quint16 ec;//电导 us/cm
    QMap<QDateTime,qreal> ec_map;
    //quint16 salinity;//盐度 mg/L
    //quint16 tds;//总溶解固体 mg/L

    /*********光照传感器--目前所用命令，返回值是7个字节***********/
    quint32 light;
    QMap<QDateTime,qreal> light_map;

    /*********PH传感器--目前所用命令，返回值是7个字节***********/
    float ph_val;
    QSqlDatabase database;
    QMap<QDateTime,qreal> ph_map;

    /********wendu***********/
    quint32 wendu;
    QMap<QDateTime,qreal> wendu_map;

    /********shidu***********/
    quint32 shidu;
    QMap<QDateTime,qreal> shidu_map;

    QDateTime current_date;
    QDateTime today;



    /*date timestamp not null default (datetime('now','localtime'))*/
signals:
    void isDone(QByteArray tmp);//接收到完整的包头以后给主线程发送的信号--携带包头数据
    void end();
    void ImageOK(const QString& picture_path);
    void DynamicShowPH(QMap<QDateTime,qreal> map);
    void DynamicShowEC(QMap<QDateTime,qreal> map);
    void DynamicShowLight(QMap<QDateTime,qreal> map);
    void DynamicShowWenDu(QMap<QDateTime,qreal> map);
    void DynamicShowShiDu(QMap<QDateTime,qreal> map);


    void updateComboxPH(QDateTime first_day);
    void updateComboxEC(QDateTime first_day);
    void updateComboxLight(QDateTime first_day);
    void updateComboxWenDu(QDateTime first_day);
    void updateComboxShiDu(QDateTime first_day);


    void updateDHTSignal(int node, int humiture,int temprature);
    void dht_lose(quint16 index);
    void tcp(char * str,int len);
    void send_pic(QString pic_path,qint64 len);

    void UpdateTime(QString);

public slots:
    void resolveDateChange(int date_index,QString str);
    void detect_dht();


private:

    bool isStop;//控制子线程的处理函数是否结束
    //8. IODevice及其子类对象都不应该跨线程调用
    //所以子线程声明/初始化该类对象的时候需要显示指定其父对象
    //通常其父对象就是将要被放入子线程的对象，所以只需要在构造函数中将其父对象指定为this指针即可
    QSerialPort *serial;//串口对象指针
    QFile  dst;//目标文件--图片文件

    unsigned int cnt_read;//总的已读取的字节数（只针对完整读取指定长度的数据包）
    unsigned int cnt_need;//还需要的字节数（只针对完整读取指定长度的数据包）
    unsigned int data_need;//每一个包的有效数据的总长度
    qint64 cnt;//串口接收到的所有有效数据的字节数（针对读取图片的整个过程）
    //char debug ;

    QByteArray read_data;//总的已读取的数据（只针对完整读取指定长度的数据包）

    //缓冲区，当读取的有效数据累计满32*64的时候在进行写入文件操作，减少写文件的次数，以提高速度
    QByteArray my_stream;
    QByteArray tmp;
    QString photo_name;


    int photo_state;//读取图片的操作状态（暂时没用到）--对应BEGIN/READING/END
    int serial_state;//读取串口的状态（读取包头/读取有效数据--对应READ_HDR/READ_DATA）

    //为了以后扩展系统--一个协调器可能接收来自多个终端节点的数据
    int node_type;//节点类型（判断当前数据包是哪一种类型的节点发过来的）
    int node_id;//节点序号（同类节点有可能有多个）
    unsigned int seqnb;//当前数据包的序号
    unsigned int seq_old;
    unsigned int pkt_cnt;//读取到图片有效数据的包的个数--满32个就进行一次写入操作

    quint8 dht_received[DHT_NUMBERS];

    bool FAIL_FLAG;//读取的数据包是否发生丢失或者重复
    QTimer *timer;

    QTimer *dht_timer;

    char tcp_data[18];

};

#endif // MYTHREAD_H
