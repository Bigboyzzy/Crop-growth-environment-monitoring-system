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

#include "tablewidget.h"
#include "customtablemodel.h"
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTableView>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QVXYModelMapper>
#include <QtWidgets/QHeaderView>
#include <QDateTimeAxis>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QSplineSeries>
#include <QDebug>
#include <QTableWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include "mythread.h"

QT_CHARTS_USE_NAMESPACE

TableWidget::TableWidget(QWidget *parent)
    : QWidget(parent)
{
    // create simple model for storing data
    // user's table data model
    //! [1]
    model = new CustomTableModel;
    //! [1]

    //! [2]
    // create table view and add model to it
    itemDelegate = new DelReconQueue();
    connect(itemDelegate,&DelReconQueue::commitData,this,&TableWidget::resolveCombox);

    QTableView *tableView = new QTableView;
    tableView->setModel(model);
    tableView->setItemDelegateForColumn(0, itemDelegate);//设置第二列
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->resizeColumnsToContents();
    tableView->setStyleSheet(QString::fromUtf8("font: 14pt \"Sans Serif\";"));
    tableView->verticalHeader()->hide();
    tableView->horizontalHeader()->setStyleSheet("font: 12pt \"Sans Serif\";");

    dateCombox = new QComboBox;
    QDateTime today = QDateTime ::currentDateTime();
    for (int i = 0;i >= -30; i--)
        dateCombox->addItem(today.addDays(i).toString("yyyy-MM-dd"));

    //connect(dateCombox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &TableWidget::comBoxUpdate);
    dateCombox->setStyleSheet(QString::fromUtf8("font: 14pt \"Sans Serif\";"));

    QVBoxLayout * table_layout = new QVBoxLayout;
    table_layout->addWidget(dateCombox);
    table_layout->addWidget(tableView);

    QWidget * myTable = new QWidget;
    myTable->setLayout(table_layout);


    //connect(model, &CustomTableModel::updateCount, this, &QTableView::updateCount);
    //! [2]

    //! [3]
    chart = new QChart;
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->hide();
    //! [3]

    // series 1
    //! [4]
    series = new QLineSeries();
    //splineSeries->setName("spline");

    mapper = new QVXYModelMapper(this);
    mapper->setXColumn(0);
    mapper->setYColumn(1);
    mapper->setFirstRow(0);
    mapper->setRowCount(96);
    mapper->setSeries(series);
    mapper->setModel(model);

    chart->addSeries(series);
    //! [4]

    //! [5]
    // for storing color hex from the series
    QString seriesColorHex = "#000000";

    // get the color of the series and use it for showing the mapped area
    seriesColorHex = "#" + QString::number(series->pen().color().rgb(), 16).right(6).toUpper();
    model->addMapping(seriesColorHex, QRect(0, 0, 2, model->rowCount()));
    //! [5]

    axisX = new QDateTimeAxis;

#ifndef DEBUG_TIME
    axisX->setFormat("h:m");
#else
    axisX->setFormat("m:s");
#endif
    chart->setAxisX(axisX, series);
    //chart->setTheme(QChart::ChartThemeQt);

    axisY = new QValueAxis;
    axisY->setRange(4, 9);
    //axisY->setMinorTickCount(4);

    axisY->setLabelFormat("%.2f");
    chart->setAxisY(axisY, series);


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(640, 480);

    //! [8]

    //! [9]
    // create main layout
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(myTable, 1, 0);
    mainLayout->addWidget(chartView, 1, 1);
    mainLayout->setColumnStretch(1, 1);
    mainLayout->setColumnStretch(0, 0);
    setLayout(mainLayout);
    //! [9]
}

void TableWidget::updateMVC_PH(QMap<QDateTime,qreal> tmp)
{
   this->chart->removeSeries(series);

   delete  mapper;

   delete  series;
   delete  axisX;
   delete  axisY;

   mapper = new QVXYModelMapper(this);
   model->UpdateShow(tmp);

   series = new QLineSeries();


   mapper = new QVXYModelMapper();
   mapper->setXColumn(0);
   mapper->setYColumn(1);
   mapper->setFirstRow(0);
   mapper->setRowCount(96);
   mapper->setSeries(series);
   mapper->setModel(model);

   chart->addSeries(series);
   //! [4]

   //! [5]
   // for storing color hex from the series
   QString seriesColorHex = "#000000";

   // get the color of the series and use it for showing the mapped area
   seriesColorHex = "#" + QString::number(series->pen().color().rgb(), 16).right(6).toUpper();
   model->addMapping(seriesColorHex, QRect(0, 0, 2, model->rowCount()));
   //! [5]

   axisX = new QDateTimeAxis;

#ifndef DEBUG_TIME
   axisX->setFormat("h:m");
#else
   axisX->setFormat("m:s");
#endif
   chart->setAxisX(axisX, series);


   axisY = new QValueAxis;
   if((model->title).contains(PH_TABLE))
   {
        axisY->setRange(4, 9);
        axisY->setLabelFormat("%.2f");
   }
   else if((model->title).contains(CONDUCT_TABLE))
   {
        axisY->setRange(0,100);//默认是0---1000
        //axisY->setMin(0);
        axisY->setLabelFormat("%d");
   }
   else if((model->title).contains(LIGHT_TABLE))
   {
        //axisY->setRange(0,100000);//默认是0---200000
        axisY->setMin(0);

        axisY->setLabelFormat("%d");
   }
   else if((model->title).contains(WENDU_TABLE))
   {
        //axisY->setRange(0,100000);//默认是0---200000
        axisY->setRange(0, 40);

        axisY->setLabelFormat("%d");
        qDebug()<<"wendu";
   }
   else if((model->title).contains(SHIDU_TABLE))
   {
        //axisY->setRange(0,100000);//默认是0---200000
        axisY->setRange(0, 100);

        axisY->setLabelFormat("%d");
        qDebug()<<"shidu";
   }

   chart->setAxisY(axisY, series);

}


void TableWidget::resolveCombox(QWidget *editor)
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);

    int tmp_date = comboBox->currentIndex();

    if(tmp_date != -1)
    {
        //emit changeShowDate(tmp_date, model->title);
    }
}

void TableWidget::comBoxUpdate(int date_index)
{
    //emit changeShowDate(date_index, model->title);
    qDebug()<<"---------------------------------------------------touch"<<model->title;
}

void TableWidget::setTitle(QString  str)
{
   model->title = QString(str);

}

void TableWidget::updateCombox(QDateTime first_day)
{
    //QDateTime current_day = first_day.addDays(30);
#if 0
    for (int i = 0;i <= 30; i++)
        dateCombox->removeItem(i);

    QDateTime today = QDateTime ::currentDateTime();
    for (int i = 0;i >= -30; i--)
        dateCombox->addItem(today.addDays(i).toString("yyyy-MM-dd"));
#endif
}
void TableWidget::ShowTime(QString arg)
{
    this->dateCombox->setItemText(0,arg);
}

