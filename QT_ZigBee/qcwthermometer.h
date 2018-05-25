#ifndef QCWTHERMOMETER_H
#define QCWTHERMOMETER_H

#include <QWidget>
#include <QPainter>
#include <QObject>
#include <QRectF>
#include <QMutex>
class Q_DECL_EXPORT QcwThermometer : public QWidget
{
Q_OBJECT
    Q_ENUMS(errorCode)
    Q_PROPERTY(double value READ value WRITE setValue)
    Q_PROPERTY(int minValue READ minValue WRITE setMinValue)
    Q_PROPERTY(int maxValue READ maxValue WRITE setMaxValue)
    Q_PROPERTY(double thresholdUp READ thresholdUp WRITE setThresholdUp)
    Q_PROPERTY(double thresholdDown READ thresholdDown WRITE setThresholdDown)
    Q_PROPERTY(QString units READ units WRITE setUnits)
    Q_PROPERTY(int scaleMajor READ scaleMajor WRITE setScaleMajor)
    Q_PROPERTY(int scaleMinor READ scaleMinor WRITE setScaleMinor)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
    Q_PROPERTY(QColor foreground READ foreground WRITE setForeground)
    Q_PROPERTY(QColor background READ background WRITE setBackground)
    Q_PROPERTY(QColor liquidColor READ liquidColor WRITE setLiquidColor)

public:
    enum AlarmCode {AlarmUp = 1, AlarmDown, AlarmNo};

    explicit QcwThermometer(QWidget *parent = 0);
    double value() const {return m_value;}
    int minValue() const {return m_minValue;}
    int maxValue() const {return m_maxValue;}
    double thresholdUp() const {return m_thresholdUp;}
    double thresholdDown() const {return m_thresholdDown;}
    QString units()const {return m_units;}
    int scaleMajor() const {return m_scaleMajor;}
    int scaleMinor() const {return m_scaleMinor;}
    QColor borderColor() const {return m_borderColor;}
    QColor foreground() const {return m_foreground;}
    QColor background() const {return m_background;}
    QColor liquidColor() const {return m_liquidColor;}

Q_SIGNALS:
    void thresholdAlarm(enum AlarmCode);

public Q_SLOTS:
    void setValue(double);
    void setMinValue(int);
    void setMaxValue(int);
    void setThresholdUp(double);
    void setThresholdDown(double);
    void setUnits(QString);
    void setScaleMajor(int);
    void setScaleMinor(int);
    void setBorderColor(QColor);
    void setForeground(QColor);
    void setBackground(QColor);
    void setLiquidColor(QColor);

protected:
    void paintEvent(QPaintEvent *event);
    void thresholdManager();

public:
    double m_value;
    int m_minValue;
    int m_maxValue;
    double m_thresholdUp;
    double m_thresholdDown;
    bool m_thresholdFlag;
    QString m_units;
    int m_scaleMajor;
    int m_scaleMinor;
    QColor m_borderColor;
    QColor m_foreground;
    QColor m_background;
    QColor m_liquidColor;
};

#endif
