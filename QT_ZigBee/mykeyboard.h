#ifndef MYKEYBOARD_H
#define MYKEYBOARD_H

#include <QWidget>
#include "frmmain.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>


#define KEY_NUMBER 15
#define LINE_NUM 5


class MyKeyboard : public QWidget
{
    Q_OBJECT
public:
    explicit MyKeyboard(QWidget *parent = 0);

signals:

public slots:
private:

    QHBoxLayout *h_layout;
    QVBoxLayout *main_layout;
    frmMain * diy_control;

public:
    QPushButton * key_board;
};

#endif // MYKEYBOARD_H
