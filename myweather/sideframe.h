#ifndef BOTTOMWDGET_H
#define BOTTOMWDGET_H

#include <QFrame>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include "searchbox.h"
#include "resultitem.h"

class SideFrame : public QWidget
{
    Q_OBJECT
    SearchBox * s;
    QVBoxLayout * vl;
    QListWidget * list;
    int row; // for listwidget index
public:
    explicit SideFrame(QWidget *parent, int fixedwidth, int minimaleheight);
private:
    void setup();
    void setupBg();
signals:
    void emitNewWidget(ResultItem *);
    void emitDelWidget(ResultItem *);
    // to MainWidget new/del a centralwiget!!

public slots:
    void newWidget(QString);
    void delWidget(ResultItem *);
};

#endif // BOTTOMWDGET_H
