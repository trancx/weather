#include "sideframe.h"
#include "resultitem.h"
#include <QListWidgetItem>
#include <QDebug>

SideFrame::SideFrame(QWidget *parent,
        int fixedwidth, int minimaleheight) : QWidget(parent), row(1)
{
    this->setFixedWidth(fixedwidth);
    this->setMinimumHeight(minimaleheight);

    s = new SearchBox(this, fixedwidth-4, 36);
    // 36 is perfect
    vl = new QVBoxLayout();
    list = new QListWidget(this);
    list->setFixedWidth(fixedwidth-4);
    list->setMinimumHeight(460);
    list->setStyleSheet("background-color: transparent");
    setup();
    setupBg();

    connect(s, SIGNAL(doSearch(QString)), this, SLOT(newWidget(QString)) );
}

void SideFrame::setup() {
    vl->setContentsMargins(2,20,2,2);
    vl->addWidget(s);
    vl->addWidget(list);
    setLayout(vl);
}


void SideFrame::setupBg() {
    QPalette plaette;
    plaette.setBrush(this->backgroundRole(), QBrush(QColor(0,0,0,99)));
    this->setPalette(plaette);
    this->setAutoFillBackground(true);
}

void SideFrame::newWidget(QString s) {
    ResultItem * result = new ResultItem(NULL, s, NULL);
    /* if NULL, means it counts on me!(mem leak) */
    QListWidgetItem * item = new QListWidgetItem;
    result->setItem(item);
    item->setBackground(QBrush(QColor(Qt::transparent)));
    item->setSizeHint(QSize(0,50));
    list->insertItem(0,item);
    list->setItemWidget(item, result);
    connect(result, SIGNAL(emitClose(ResultItem*)),this, SLOT(delWidget(ResultItem*))) ;
    qDebug() << list->count();
    emit emitNewWidget(result);
}

void SideFrame::delWidget(ResultItem * addr) {
    list->removeItemWidget(addr->getItem());
    qDebug() << "remove item widget";
    delete list->takeItem(list->row(addr->getItem()));
    qDebug() << "delete item";
    row--;
    qDebug() << list->count();
    emit emitDelWidget(addr);
}
