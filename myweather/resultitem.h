#ifndef RESULTITEM_H
#define RESULTITEM_H
#include "centralwidget.h"
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidgetItem>

class ResultItem : public QWidget
{
    Q_OBJECT
private:
    CentralWidget * owner;
    QPushButton * close;
    QListWidgetItem * item;
    QHBoxLayout * l;
    QLabel * label;
public:
    /**
     *
     * 不是不想使用Const 而是 baseclass 并不是我设计
     * 加上const 可能导致很多函数无法使用
     */
    ResultItem(QWidget * parent,
               const QString& name, CentralWidget * owner);
    QListWidgetItem * getItem();
    void setItem(QListWidgetItem *);
    CentralWidget * getOwner();
    void setOwner(CentralWidget * owner);
    QString getName();
protected:
    void mouseReleaseEvent(QMouseEvent *event);
signals:
    void emitClose(ResultItem *);
    void emitShow(ResultItem *);
public slots:
    void pressClose(); /* when ckick the close button */
    void sigShow(); /* when click this item */
};

#endif // RESULTITEM_H
