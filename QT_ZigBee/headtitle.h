#ifndef HEADTITLE_H
#define HEADTITLE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QString>

class headtitle : public QWidget
{
    Q_OBJECT
public:
    explicit headtitle(QWidget *parent = 0);
    headtitle(QString left_btn,QString  str,QString right_btn,QWidget *parent);

signals:
    void left();
    void right();


public slots:
    void resolv_left();
    void resolv_right();
private:
    QPushButton * LeftBtn;
    QPushButton * RightBtn;
    QLabel * title;
};

#endif // HEADTITLE_H
