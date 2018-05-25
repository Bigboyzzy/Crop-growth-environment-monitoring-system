#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QWidget>

class QPushButton;
class QLineEdit;
class QProgressBar;
class QSlider;
class QTextEdit;


class frmMain : public QWidget
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();

private:
    //Ui::frmMain *ui;

public slots:
    void initForm();
    void setBtnQss(QPushButton *btn,
                   QString normalColor, QString normalTextColor,
                   QString hoverColor, QString hoverTextColor,
                   QString pressedColor, QString pressedTextColor);
    void setTextEdit(QTextEdit *txt, QString normalColor, QString focusColor);
    void setLineEditQss(QLineEdit *txt, QString normalColor, QString focusColor);
    void setBarQss(QProgressBar *bar, QString normalColor, QString chunkColor);
    void setSliderQss(QSlider *slider, QString normalColor, QString grooveColor, QString handleColor);
};

#endif // FRMMAIN_H
