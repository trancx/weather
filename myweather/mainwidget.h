#ifndef MAINWIDGET_H
#define MAINWIDGET_H
#include "centralwidget.h"
#include "sideframe.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QLinkedList>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    MainWidget(QWidget *parent, int minimumWidth, int minimumHeight);
    ~MainWidget();
private:
    void instantiate();
    QFrame * tmp;
    CentralWidget * now; /* the central widget we show now! */
    QLinkedList<CentralWidget> list; /* re-write to file */
    SideFrame * side;
    QHBoxLayout * layout;
public slots:
    void newCentralWidget(ResultItem *);
    void delCentralWidget(ResultItem *);
    void showCentralWidget(ResultItem *);
    void refreshCentralWidget();
signals:
    void emitChanged(ResultItem * );
};

#endif // MAINWIDGET_H
