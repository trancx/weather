#include "mainwidget.h"
#include "sideframe.h"
#include <QFrame>
#include <QDebug>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
}

MainWidget::~MainWidget()
{

}

MainWidget::MainWidget(QWidget *parent,
        int minimumWidth, int minimumHeight)
    : QWidget(parent), now(NULL)
{
    this->setMinimumSize(minimumWidth, minimumHeight);
    instantiate();

    connect(side, SIGNAL(emitNewWidget(ResultItem*)),
            this, SLOT(newCentralWidget(ResultItem*)) );
    connect(side, SIGNAL(emitDelWidget(ResultItem*)),
            this, SLOT(delCentralWidget(ResultItem*)) );
}

void MainWidget::instantiate() {
    side = new SideFrame(this, 250, 500 );
//    center = new CentralWidget(this, 512, 512);
    tmp = new QFrame(this);
    tmp->setMinimumSize(514, 514);
    QHBoxLayout * tl = new QHBoxLayout();
    tl->setContentsMargins(2,2,2,2);
    tmp->setLayout(tl);
    layout = new QHBoxLayout();
    layout->addWidget(tmp);
    layout->addWidget(side);
    layout->setContentsMargins(0,0,0,0);
    layout->setAlignment(side, Qt::AlignRight);
    setLayout(layout);
}

void MainWidget::newCentralWidget(ResultItem * addr) {
    qDebug() << "MainWiget::newCentalWidget";
    if( now ) now->setHidden(true);
    CentralWidget * cw = new CentralWidget(NULL, 512, 512);
    addr->setOwner(cw);
    now = cw;
    QLayout * l = tmp->layout();
    l->addWidget(cw);
    l->setAlignment(cw, Qt::AlignCenter);
    cw->setUrl(addr->getName());
    cw->setWeather("....");
    cw->setTemperature("__°");
    cw->getWeather();
    connect(addr, SIGNAL(emitShow(ResultItem*)),
            this, SLOT(showCentralWidget(ResultItem*)) );
    emit emitChanged(addr);
}

/**
 *              __SEE_FIXME__
 *
 * it's a problem, what to do if the '@@now' is
 * exactly the widget we delete? how to switch
 * to the alter?   or what if the '@@list' is an
 * empty list?
 */
void MainWidget::delCentralWidget(ResultItem * addr) {
    CentralWidget * tmp = addr->getOwner();
    if( now == tmp ) {
        now->setHidden(true);
        now = NULL; // FIXME
        qDebug() << "Problem Occurs! ";
    }
    qDebug() << "delete widget";
    delete tmp;
    delete addr;
}

void MainWidget::showCentralWidget(ResultItem * addr) {
    CentralWidget * tmp = addr->getOwner();
    if( now == tmp ) {
        //do nothing
        return;
    }
    /* @@now maybe NULL */
    if( now )
        now->setHidden(true);
    now = tmp;
    now->setHidden(false);
    emit emitChanged(addr);
}


void MainWidget::refreshCentralWidget() {
    now->refresh();
}
