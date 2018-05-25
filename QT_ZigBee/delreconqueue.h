#ifndef DELRECONQUEUE_H
#define DELRECONQUEUE_H
#include <QtWidgets/QTableView>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QStyledItemDelegate>
#include <QItemDelegate>
#include <QComboBox>




class DelReconQueue : public QStyledItemDelegate
{

signals:

public:
     explicit DelReconQueue(QWidget *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)const ;
    void updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &) const;
public:
    //const QComboBox *comboBox ;


private slots:
    void commitAndCloseEditor(int );
};


#endif // DELRECONQUEUE_H
