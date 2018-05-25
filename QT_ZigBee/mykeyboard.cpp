#include "mykeyboard.h"

MyKeyboard::MyKeyboard(QWidget *parent) : QWidget(parent)
{
    key_board = new QPushButton[KEY_NUMBER];
    h_layout = new QHBoxLayout[LINE_NUM];

    main_layout = new QVBoxLayout();

    for(int i = 0;i < LINE_NUM;i++)
    {
        for(int j = 0; j < 3;j++)
        {
            (h_layout+i)->addWidget(key_board+j+3*i);
            (key_board+j+3*i)->setText(QString("%1").arg(j+3*i+1));
        }
        main_layout->addLayout(h_layout + i);
    }

    (key_board+12)->setText(QString("OK"));
    (key_board+10)->setText(QString("0"));
    (key_board+14)->setText(QString("<--"));
    (key_board+9)->setText(QString(" "));
    (key_board+13)->setText(QString("-"));
    (key_board+11)->setText(QString(" "));

    diy_control = new frmMain();
    diy_control->setBtnQss((key_board+0), "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");
    diy_control->setBtnQss(key_board+1, "#1ABC9C", "#E6F8F5", "#2EE1C1", "#FFFFFF", "#16A086", "#A7EEE6");
    diy_control->setBtnQss(key_board+2, "#3498DB", "#FFFFFF", "#5DACE4", "#E5FEFF", "#2483C7", "#A0DAFB");

    diy_control->setBtnQss(key_board+4, "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");
    diy_control->setBtnQss(key_board+5, "#1ABC9C", "#E6F8F5", "#2EE1C1", "#FFFFFF", "#16A086", "#A7EEE6");
    diy_control->setBtnQss(key_board+8, "#3498DB", "#FFFFFF", "#5DACE4", "#E5FEFF", "#2483C7", "#A0DAFB");
    diy_control->setBtnQss(key_board+6, "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");
    diy_control->setBtnQss(key_board+7, "#1ABC9C", "#E6F8F5", "#2EE1C1", "#FFFFFF", "#16A086", "#A7EEE6");
    diy_control->setBtnQss(key_board+3, "#3498DB", "#FFFFFF", "#5DACE4", "#E5FEFF", "#2483C7", "#A0DAFB");
    diy_control->setBtnQss(key_board+9, "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");
    diy_control->setBtnQss(key_board+11, "#1ABC9C", "#E6F8F5", "#2EE1C1", "#FFFFFF", "#16A086", "#A7EEE6");
    diy_control->setBtnQss(key_board+10, "#3498DB", "#FFFFFF", "#5DACE4", "#E5FEFF", "#2483C7", "#A0DAFB");

    diy_control->setBtnQss((key_board+14), "#34495E", "#FFFFFF", "#4E6D8C", "#F0F0F0", "#2D3E50", "#B8C6D1");
    diy_control->setBtnQss(key_board+13, "#1ABC9C", "#E6F8F5", "#2EE1C1", "#FFFFFF", "#16A086", "#A7EEE6");
    diy_control->setBtnQss(key_board+12, "#3498DB", "#FFFFFF", "#5DACE4", "#E5FEFF", "#2483C7", "#A0DAFB");



    this->setLayout(main_layout);
}

//MyKeyboard::~MyKeyboard()
//{
//    delete []key_board;
//    delete []h_layout;
//}


