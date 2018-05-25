/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "customtablemodel.h"
#include <QtCore/QVector>
#include <QtCore/QTime>
#include <QtCore/QRect>
#include <QtGui/QColor>
#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include "mythread.h"

CustomTableModel::CustomTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_columnCount = 2;
    m_rowCount = 96;
    m_data.clear();
    title = QString("");
    InitData();
}

void CustomTableModel::InitData()
{
    //system("hwclock -w");
    system("hwclock -s");
    //system("hwclock -w");

    QDateTime  tmpDate = QDateTime::currentDateTime();
    QString year = tmpDate.toString("yy");
    QString month = tmpDate.toString("M");
    QString day = tmpDate.toString("d");
    QString str = QString("M%1d%2y%3").arg(month).arg(day).arg(year);
    str.append(QString("00:00:00"));
    qDebug()<<str;
    QDateTime dateTime2 = QDateTime::fromString(str,
                                                  "'M'M'd'd'y'yyhh:mm:ss");
    qDebug()<<dateTime2.toString();
    for (int i = 0; i <m_rowCount; i++) {
            //m_data.insert(tmpDate.addSecs((30)*i),10);
#ifndef DEBUG_TIME

            m_data.insert(dateTime2.addSecs((60*15)*i),0);
#else
            m_data.insert(tmpDate.addSecs(i*7),0);
#endif
            QModelIndex index = this->index(i, 0, QModelIndex());
            emit dataChanged(index, index);
            QModelIndex index2 = this->index(i, 1, QModelIndex());
            emit dataChanged(index2, index2);

    }
}
#if 0
void CustomTableModel::UpdateShow(QMap<QDateTime,qreal> tmp_map)
{
    QDateTime  tmpDate = QDateTime::currentDateTime();
    QString year = tmpDate.toString("yy");
    QString month = tmpDate.toString("M");
    QString day = tmpDate.toString("d");
    static int k = 0;
    QString str = QString("M%1d%2y%3").arg(month).arg(day).arg(year);
    str.append(QString("00:00:00"));
    qDebug()<<str;
    QDateTime dateTime2 = QDateTime::fromString(str,
                                                  "'M'M'd'd'y'yyhh:mm:ss");
    qDebug()<<dateTime2.toString();
    qDebug()<<"begin update..............................................................";
    if(m_data.count() > 0)
        this->m_data.clear();
    for (int i = 0; i <m_rowCount; i++,k++) {
            m_data.insert(dateTime2.addSecs((60*15)*i),10+k*10);
            QModelIndex index = this->index(i, 0, QModelIndex());
            emit dataChanged(index, index);
            QModelIndex index2 = this->index(i, 1, QModelIndex());
            emit dataChanged(index2, index2);

    }
}

#else
void CustomTableModel::UpdateShow(QMap<QDateTime,qreal> tmp_map)
{

    if(m_data.count() > 0)
        this->m_data.clear();
    //this->m_data.clear();
    this->m_data = tmp_map;
    //qDebug()<<"into CustomTableModel::UpdateShowPH ";
    for (int i = 0; i <m_rowCount; i++) {
            QModelIndex index = this->index(i, 0, QModelIndex());
            emit dataChanged(index, index);
            QModelIndex index2 = this->index(i, 1, QModelIndex());
            emit dataChanged(index2, index2);
    }
}
#endif

void CustomTableModel::UpdateData()
{
    static float inc = 0.1;
    static int flag = 0;
    m_data.clear();
    QDateTime  tmpDate = QDateTime::currentDateTime();
    for (int i = 0; i <m_rowCount; i++) {
            m_data.insert(tmpDate.addSecs((60*15)*i),6.5+inc);

            QModelIndex index = this->index(i, 0, QModelIndex());
            emit dataChanged(index, index);
            QModelIndex index2 = this->index(i, 1, QModelIndex());
            emit dataChanged(index2, index2);

            if(inc >= 2.5)
                flag = 1;
            else if(inc <= -2.5)
                flag = 0;

            if(flag == 0)
                inc = inc +0.1;
            else if(flag == 1)
                inc  = inc -0.1;
    }
}

int CustomTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.count();
}

int CustomTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columnCount;
}


//设置左侧表格第二列标题
QVariant CustomTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //qDebug()<<"set header";
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section % 2 == 0)
            return "时间";
        else
        {
            if(title.contains(PH_TABLE))
                 return "PH值";
            else if(title.contains(CONDUCT_TABLE))
                 return "电导率";
            else if(title.contains(LIGHT_TABLE))
                 return "光照强度";
            else if(title.contains(WENDU_TABLE))
                 return "温度";
            else if(title.contains(SHIDU_TABLE))
                 return "湿度";
        }
    } else {
        return QString("%1").arg(section + 1);
    }

}

QDateTime CustomTableModel::currencyAt(int offset) const
{
    return (m_data.begin() + offset).key();
}

QVariant CustomTableModel::data(const QModelIndex &index, int role) const
{
    QDateTime rowDate;
    if (role == Qt::DisplayRole) {
        switch(index.column())
        {
        case 0:
            rowDate = currencyAt(index.row());
            return rowDate;
            break;
        case 1:
            rowDate = currencyAt(index.row());
            return m_data.value(rowDate);
            break;

        }
    } else if (role == Qt::EditRole) {
        switch(index.column())
        {
        case 0:
            rowDate = currencyAt(index.row());
            return rowDate;
            break;
        case 1:
            rowDate = currencyAt(index.row());
            return m_data.value(rowDate);
            break;

        }
    } else if (role == Qt::BackgroundRole) {
        foreach (QRect rect, m_mapping) {
            if (rect.contains(index.column(), index.row()))
                return QColor(m_mapping.key(rect));
        }
        return QColor(Qt::white);
    }
    return QVariant();
}

bool CustomTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        QDateTime rowDate;
        rowDate = currencyAt(index.row());
        m_data[rowDate] = value.toDouble();
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags CustomTableModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void CustomTableModel::addMapping(QString color, QRect area)
{
    m_mapping.insertMulti(color, area);
}
