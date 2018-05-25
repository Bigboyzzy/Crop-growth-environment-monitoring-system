#ifndef SEARCHFILTER_H
#define SEARCHFILTER_H

#include <QWidget>
#include <QLineEdit>
#include <QListView>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QItemSelection>
#include "frmmain.h"
#include "mykeyboard.h"
#include <QEvent>

class MyLineEdit:public QLineEdit
{

    Q_OBJECT
   public:
       explicit MyLineEdit(QWidget *parent = 0);
   protected:
       //重写mousePressEvent事件
       virtual void mousePressEvent(QMouseEvent *event);

   signals:
       //自定义clicked()信号,在mousePressEvent事件发生时触发
       void clicked();

   public slots:
};

class SearchFilter : public QWidget
{
    Q_OBJECT
public:
    explicit SearchFilter(QWidget *parent = 0);
    void Init(const QString &dir_str, const QStringList &filter_list);
    void SetCurrentSelectFile_Prev();
    void SetCurrentSelectFile_Next();
    QString  init_file_name;
    MyKeyboard * key_board;
    static int show_flag;

signals:
    void signal_current_select_file(const QString& file_name);

public slots:
    void textChanged_input_edit(const QString& text);
    void onDoubleClick_listView(const QModelIndex& index);
    //void selectChanged_listView(const QItemSelection& selection);
    //bool eventFilter(QObject *,QEvent *);    //注意这里
    void dealInput();
    void dealDelete();
    void dealOK();
    void show_key_board();

private:
    QString m_dir_str;
    MyLineEdit * m_input_edit;
    QListView * m_file_list_view;
    QStringListModel *m_string_list_model;
    //QItemSelectionModel * my_model;
    QSortFilterProxyModel* m_proxy_model;
    frmMain * diy_control;



    //int key_show_flag;

    QString resultStr;


};

#endif // SEARCHFILTER_H
