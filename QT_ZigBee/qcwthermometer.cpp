#include "qcwthermometer.h"
#include <QPainter>


QcwThermometer::QcwThermometer(QWidget *parent): QWidget(parent)
{
    m_value = 0;
    m_minValue = -30;
    m_maxValue = 70;
    m_thresholdUp = -20;
    m_thresholdDown = 50;
    m_thresholdFlag = false;
    m_units = QString::fromUtf8("°C", -1);
    m_scaleMajor = 10;
    m_scaleMinor = 5;
    m_borderColor = Qt::blue;
    m_foreground = Qt::white;
    m_background = QColor(41, 137, 254);
    m_liquidColor = Qt::red;

    resize(100, 100);
}

void QcwThermometer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); 	/* 使用反锯齿(如果可用) */
    painter.translate(width() / 2, height() / 2);	/* 坐标变换为窗体中心 */
    int side = qMin(width(), height());
    painter.scale(side / 200.0, side / 200.0);		/* 比例缩放 */
    painter.setPen(Qt::NoPen);
    int nSteps = m_scaleMajor * m_scaleMinor;
    double step = 400.0 / nSteps;
    QFont font = this->font();

    /* 绘制边框 */
    painter.save();
    painter.setPen(QPen(m_borderColor, 4));
    painter.drawRect(-95, -245, 190, 490);
    painter.restore();

    /* 绘制背景 */
    painter.save();
    painter.setBrush(m_background);
    painter.drawRect(-90, -240, 180, 480);
    painter.restore();

    /* 绘制液体容器圆 */
    painter.save();
    painter.setBrush(m_foreground);
    painter.drawEllipse(-50, 230, 40, -40);
    painter.restore();

    /* 绘制液体容器柱 */
    painter.save();
    painter.setBrush(m_foreground);
    painter.drawRect(-40, 210, 20, -440);
    painter.restore();

    /* 绘制单位 */
    painter.save();
    painter.setPen(m_foreground);
    font.setPointSize(18);
    painter.setFont(font);
    painter.drawText(15, 220, m_units);
    painter.restore();

    /* 绘制刻度线 */
    painter.save();
    QPen pen(m_foreground);
    for (int i = 0, y = 180.0; i <= nSteps; i++, y -= step)
    {
        if (i % m_scaleMinor == 0)
        {
            pen.setWidth(2);
            painter.setPen(pen);
            painter.drawLine(-20, y, 0, y);
        }
        else
        {
            pen.setWidth(1);
            painter.setPen(pen);
            painter.drawLine(-20, y, -10, y);
        }
    }
    painter.restore();

    /* 绘制刻度值 */
    painter.save();
    painter.setPen(m_foreground);
    double tmpVal;
    step = 400.0 / m_scaleMajor;
    for (int i = 0, y = 185.0; i <= m_scaleMajor; i++, y -= step)
    {
        tmpVal = 1.0 * i *((m_maxValue - m_minValue) / m_scaleMajor) + m_minValue;
        painter.drawText(5, y, QString( "%1" ).arg(tmpVal));
    }
    painter.restore();

    /* 绘制液体圆 */
    painter.save();
    painter.setBrush(m_liquidColor);
    painter.drawEllipse(-45, 225, 30, -30);
    painter.restore();

    /* 绘制液体柱(指示) */
    painter.save();
    painter.setBrush(m_liquidColor);
    double val =  400.0 /(m_maxValue - m_minValue)*(m_value - m_minValue);
    painter.drawRect(-35, 205, 10, -val - 25);
    painter.restore();
}

void QcwThermometer::setValue(double val)
{
    if (val > m_maxValue)
        m_value = m_maxValue;
    else if (val < m_minValue)
        m_value = m_minValue;
    else
        m_value = val;

    thresholdManager();
    update();
}

void QcwThermometer::setMinValue(int val)
{
    m_minValue = val;
    update();
}

void QcwThermometer::setMaxValue(int val)
{
    m_maxValue = val;
    update();
}

void QcwThermometer::setThresholdUp(double val)
{
    m_thresholdUp = val;
    thresholdManager();
    update();
}

void QcwThermometer::setThresholdDown(double val)
{
    m_thresholdDown = val;
    thresholdManager();
    update();
}

void QcwThermometer::setUnits(QString str)
{
    m_units = str;
    update();
}

void QcwThermometer::setScaleMajor(int scale)
{
    m_scaleMajor = scale;
    update();
}

void QcwThermometer::setScaleMinor(int scale)
{
    m_scaleMinor = scale;
    update();
}

void QcwThermometer::setBorderColor(QColor color)
{
    m_borderColor = color;
    update();
}

void QcwThermometer::setForeground(QColor color)
{
    m_foreground = color;
    update();
}
//extern  QMutex mutex;
void QcwThermometer::setBackground(QColor color)
{
    //mutex.lock();
    m_background = color;
    update();
    //mutex.unlock();
}

void QcwThermometer::setLiquidColor(QColor color)
{
    m_liquidColor = color;
    update();
}

void QcwThermometer::thresholdManager()
{
    if (!m_thresholdFlag)
    {
        if(m_value > m_thresholdUp)
        {
            m_thresholdFlag = true;
            Q_EMIT thresholdAlarm(AlarmUp);
        }
        else if(m_value < m_thresholdDown)
        {
            m_thresholdFlag = true;
            Q_EMIT thresholdAlarm(AlarmDown);
        }

    }
    else if (m_value <= m_thresholdUp && m_value >= m_thresholdDown)
    {
        m_thresholdFlag = false;
        Q_EMIT thresholdAlarm(AlarmNo);
    }
}
