#include "headtitle.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QDebug>

headtitle::headtitle(QWidget *parent) : QWidget(parent)
{

}

headtitle::headtitle(QString left_btn,QString  str,QString right_btn,QWidget *parent) : QWidget(parent)
{
    LeftBtn = new QPushButton();
    RightBtn = new QPushButton();
    title = new QLabel();

    LeftBtn->setMinimumWidth(80);
    LeftBtn->setMaximumWidth(80);
    LeftBtn->setMinimumHeight(60);
    LeftBtn->setMinimumHeight(60);
    //diy_control->setBtnQss(PhotoLeftBtn, "#3498DB", "#FFFFFF", "#5DACE4", "#E5FEFF", "#2483C7", "#A0DAFB");
    LeftBtn->setFlat(true);
    //PhotoLeftBtn->setStyleSheet("background: transparent;outline: none");

    //QIcon photo_left_ico(":/src_img/p_left.png");
    //PhotoLeftBtn->setIcon(photo_left_ico);
    //PhotoLeftBtn->setIconSize(QSize(60,28));
    LeftBtn->setObjectName("p_left_btn");

    LeftBtn->setStyleSheet(QString("QPushButton#p_left_btn{border-image:url(%1);background: transparent;outline: none}").arg(left_btn) );

    RightBtn->setMinimumWidth(80);
    RightBtn->setMaximumWidth(80);
    RightBtn->setMinimumHeight(60);
    RightBtn->setMinimumHeight(60);
    //diy_control->setBtnQss(PhotoRightBtn, "#1ABC9C", "#E6F8F5", "#2EE1C1", "#FFFFFF", "#16A086", "#A7EEE6");
    RightBtn->setFlat(true);
    //RightBtn->setStyleSheet("background: transparent;outline: none");

    //QIcon photo_right_ico(":/src_img/p_right.png");
    //PhotoRightBtn->setIcon(photo_right_ico);
    //PhotoRightBtn->setIconSize(QSize(60,28));

    RightBtn->setObjectName("p_right_btn");

    RightBtn->setStyleSheet(QString("QPushButton#p_right_btn{border-image:url(%1);background: transparent;outline: none}").arg(right_btn));

    title->setText(str);
    title->adjustSize();

    QSpacerItem* left = new QSpacerItem(75, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem* right = new QSpacerItem(75, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QHBoxLayout *tool_layout = new QHBoxLayout();

    tool_layout->addWidget(LeftBtn);
    tool_layout->addSpacerItem(right);
    tool_layout->addWidget(title);
    tool_layout->addSpacerItem(left);
    tool_layout->addWidget(RightBtn);

    this->setLayout(tool_layout);

    connect(LeftBtn,&QPushButton::clicked,this,&headtitle::resolv_left);
    connect(RightBtn,&QPushButton::clicked,this,&headtitle::resolv_right);
}

void headtitle::resolv_left()
{
    emit left();

}

void headtitle::resolv_right()
{
    emit right();
}

