#include "resultitem.h"
#include <QListWidgetItem>
#include <QDebug>

ResultItem::ResultItem(QWidget *parent,
        const QString &name, CentralWidget *owner): QWidget(parent), item(NULL)
{
    this->owner = owner;
    close = new QPushButton;
    label = new QLabel;
    label->setText(name);
    label->setStyleSheet("color: white");
//    item = new QListWidgetItem(this);
    l = new QHBoxLayout;
    close->setIcon(QPixmap(":/imgs/imgs/edit-delete.png"));
    close->setFlat(true);
    close->setFixedSize(32,32);

//    item->setText(name);
//    l->addWidget(item);
    l->addWidget(label);
    l->addWidget(close);
    l->setContentsMargins(5,0,5,0);
    this->setLayout(l);

    connect(close, SIGNAL(clicked(bool)),
            this, SLOT(pressClose()) );

}

void ResultItem::pressClose() {

    qDebug() << "close pressed";
    emit emitClose(this);
}

void ResultItem::sigShow() {

}

void ResultItem::mouseReleaseEvent(QMouseEvent *event) {
    unsigned long a = (unsigned long)owner;
    qDebug() << "mouse release!";
    qDebug() <<  a << "\n";

    QWidget::mouseReleaseEvent(event);
    emit emitShow(this);
}

/* 以下函数最好都是 inline， 当然可能会有其他问题， 因为我是使用者*/
QListWidgetItem * ResultItem::getItem() {
    return item;
}

void ResultItem::setItem(QListWidgetItem * item) {
    this->item = item;
}

CentralWidget * ResultItem::getOwner() {
    return owner;
}

void ResultItem::setOwner(CentralWidget * owner) {
    this->owner = owner;
}

QString ResultItem::getName() {
    return label->text();
}
