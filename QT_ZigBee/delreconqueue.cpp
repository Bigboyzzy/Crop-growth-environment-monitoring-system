#include "delreconqueue.h"
#include <QApplication>
#include <QPainter>
#include <QStyleOption>
#include <QDesktopWidget>
#include <QDateTime>
#include <QDebug>
#include "mythread.h"

DelReconQueue::DelReconQueue(QWidget *parent) :
    QStyledItemDelegate(parent)
{

}


void DelReconQueue::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{

    if (index.column() == 0)
    {
        //获得当前项值
        QDateTime time = index.model()->data(index, Qt::DisplayRole).toDateTime();
        //设置'Id'格式字符串: P:00000x;6位10进制数，不足补0;
        //QString text = QString("P:%1").arg(patientId, 6, 10, QChar('0'));
#ifndef DEBUG_TIME
        QString text = time.toString("hh:mm");
#else
        QString text = time.toString("mm:ss");
#endif

        //获取项风格设置
        QStyleOptionViewItem myOption = option;
        myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
        //绘制文本
        QApplication::style()->drawItemText ( painter, myOption.rect , myOption.displayAlignment, QApplication::palette(), true,text );


    }
  else
    {
        //否则调用默认委托
        QStyledItemDelegate::paint(painter, option, index);
    }

}



QWidget * DelReconQueue::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);
    QDateTime today = QDateTime ::currentDateTime();
    for (int i = -30;i <= 0; i++)
        editor->addItem(today.addDays(i).toString("dd"));

    connect(editor, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DelReconQueue::commitAndCloseEditor);
    return editor;
}

void DelReconQueue::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    int tindex = comboBox->findText(text);
    comboBox->setCurrentIndex(tindex);

}

void DelReconQueue::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString text = comboBox->currentText();

    model->setData(index, text, Qt::EditRole);


}
void DelReconQueue::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

void DelReconQueue::commitAndCloseEditor(int )
{
        QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
        //emit commitData(comboBox);
        //emit closeEditor(comboBox);
}
