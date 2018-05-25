#include "frmmain.h"
#include <QPushButton>
#include <QSlider>
#include <QLineEdit>
#include <QProgressBar>
#include <QTextEdit>
frmMain::frmMain(QWidget *parent) :
    QWidget(parent)
{

}

frmMain::~frmMain()
{

}

void frmMain::initForm()
{
#if 0
    ui->bar1->setRange(0, 100);
    ui->bar2->setRange(0, 100);
    ui->slider1->setRange(0, 100);
    ui->slider2->setRange(0, 100);

    connect(ui->slider1, SIGNAL(valueChanged(int)), ui->bar1, SLOT(setValue(int)));
    connect(ui->slider2, SIGNAL(valueChanged(int)), ui->bar2, SLOT(setValue(int)));
    ui->slider1->setValue(30);
    ui->slider2->setValue(30);

    this->setStyleSheet("*{outline:0px;}QWidget#frmMain{background:#FFFFFF;}");

    setBtnQss(ui->btn1, "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");
    setBtnQss(ui->btn2, "#1ABC9C", "#E6F8F5", "#2EE1C1", "#FFFFFF", "#16A086", "#A7EEE6");
    setBtnQss(ui->btn3, "#3498DB", "#FFFFFF", "#5DACE4", "#E5FEFF", "#2483C7", "#A0DAFB");
    setBtnQss(ui->btn4, "#E74C3C", "#FFFFFF", "#EC7064", "#FFF5E7", "#DC2D1A", "#F5A996");

    setTxtQss(ui->txt1, "#DCE4EC", "#34495E");
    setTxtQss(ui->txt2, "#DCE4EC", "#1ABC9C");
    setTxtQss(ui->txt3, "#DCE4EC", "#3498DB");
    setTxtQss(ui->txt4, "#DCE4EC", "#E74C3C");

    setBarQss(ui->bar1, "#E8EDF2", "#E74C3C");
    setBarQss(ui->bar2, "#E8EDF2", "#1ABC9C");

    setSliderQss(ui->slider1, "#E8EDF2", "#1ABC9C", "#1ABC9C");
    setSliderQss(ui->slider2, "#E8EDF2", "#E74C3C", "#E74C3C");
    //setSliderQss(ui->slider1, "#E8EDF2", "#34495E", "#1ABC9C");
    //setSliderQss(ui->slider2, "#E8EDF2", "#3498DB", "#E74C3C");
#endif
}

void frmMain::setBtnQss(QPushButton *btn,
                        QString normalColor, QString normalTextColor,
                        QString hoverColor, QString hoverTextColor,
                        QString pressedColor, QString pressedTextColor)
{
    QStringList qss;
    qss.append(QString("QPushButton{border-style:none;padding:10px;border-radius:5px;color:%1;background:%2}").arg(normalTextColor).arg(normalColor));
    qss.append(QString("QPushButton:hover{color:%1;background:%2;}").arg(hoverTextColor).arg(hoverColor));
    qss.append(QString("QPushButton:pressed{color:%1;background:%2;}").arg(pressedTextColor).arg(pressedColor));
    qss.append((QString("QPushButton{outline: none}")));
    btn->setStyleSheet(qss.join(""));
    //btn->setStyleSheet("outline: none");
}

void frmMain::setLineEditQss(QLineEdit *txt, QString normalColor, QString focusColor)
{
    QStringList qss;
    qss.append(QString("QLineEdit{border-style:none;padding:6px;border-radius:5px;border:2px solid %1;}").arg(normalColor));
    qss.append(QString("QLineEdit:focus{border:2px solid %1;}").arg(focusColor));
    txt->setStyleSheet(qss.join(""));
}

void frmMain::setTextEdit(QTextEdit *txt, QString normalColor, QString focusColor)
{
    QStringList qss;
    qss.append(QString("QTextEdit{border-style:none;padding:6px;border-radius:5px;border:2px solid %1;}").arg(normalColor));
    qss.append(QString("QTextEdit:focus{border:2px solid %1;}").arg(focusColor));
    txt->setStyleSheet(qss.join(""));
}

void frmMain::setBarQss(QProgressBar *bar, QString normalColor, QString chunkColor)
{
    int barHeight = 8;
    int barRadius = 8;

    QStringList qss;
    qss.append(QString("QProgressBar{font:9pt;height:%2px;background:%1;border-radius:%3px;text-align:center;border:1px solid %1;}").arg(normalColor).arg(barHeight).arg(barRadius));
    qss.append(QString("QProgressBar:chunk{border-radius:%2px;background-color:%1;}").arg(chunkColor).arg(barRadius));
    bar->setStyleSheet(qss.join(""));
}

void frmMain::setSliderQss(QSlider *slider, QString normalColor, QString grooveColor, QString handleColor)
{
    int sliderHeight = 8;
    int sliderRadius = 4;
    int handleWidth = 13;
    int handleRadius = 6;
    int handleOffset = 3;

    QStringList qss;
    qss.append(QString("QSlider::groove:horizontal,QSlider::add-page:horizontal{height:%2px;border-radius:%3px;background:%1;}").arg(normalColor).arg(sliderHeight).arg(sliderRadius));
    qss.append(QString("QSlider::sub-page:horizontal{height:%2px;border-radius:%3px;background:%1;}").arg(grooveColor).arg(sliderHeight).arg(sliderRadius));
    qss.append(QString("QSlider::handle:horizontal{width:%2px;margin-top:-%3px;margin-bottom:-%3px;border-radius:%4px;"
                       "background:qradialgradient(spread:pad,cx:0.5,cy:0.5,radius:0.5,fx:0.5,fy:0.5,stop:0.6 #FFFFFF,stop:0.8 %1);}")
               .arg(handleColor).arg(handleWidth).arg(handleOffset).arg(handleRadius));
    slider->setStyleSheet(qss.join(""));
}
