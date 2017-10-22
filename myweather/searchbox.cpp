#include "searchbox.h"
#include <QKeyEvent>
#include <QDebug>
#include <QBitmap>


SearchBox::SearchBox(QWidget *parent,
        int fixedwidth, int fixedheight) : QWidget(parent) {
    this->setFixedSize(fixedwidth, fixedheight);
//    this->setFrameShape(QFrame::Panel);
    text = new QLineEdit;
    text->setObjectName("text");
    text->setFixedHeight(fixedheight-2);
    search = new QPushButton;
    search->setObjectName("search");
    search->setFixedSize(fixedheight-2, fixedheight-2);
    empty = new QPushButton;
    empty->setObjectName("empty");
    empty->setFixedSize(fixedheight-2, fixedheight-2);
    hl = new QHBoxLayout;
    hl->setObjectName("searchbox");
    setup();
    connect(text, SIGNAL(textEdited(QString)), this, SLOT(textEdit(QString)));
    connect(search, SIGNAL(clicked()), this, SLOT(searchClicked()));
    connect(empty, SIGNAL(clicked()), this, SLOT(editClear()));
}

void SearchBox::setup() {
//    QString qss = "background-color: transparent";
    search->setIcon(QPixmap(":/imgs/imgs/system-search.png"));
//    search->setStyleSheet(qss);
    search->setFlat(true);
    empty->setIcon(QPixmap(":/imgs/imgs/edit-clear"));
//    empty->setStyleSheet(qss);
    empty->setHidden(true);
    empty->setFlat(true);
    text->setStyleSheet("background-color: transparent; color: white");

    hl->addWidget(search);
    hl->addWidget(text);
    hl->addWidget(empty);
    hl->setContentsMargins(0,0,0,0);
    setLayout(hl);
}

void SearchBox::textEdit(QString str) {
    if( str.length() ) {
        empty->setHidden(false);
    } else {
        empty->setHidden(true);
    }
}

void SearchBox::searchClicked() {
    QString str = text->text();
    text->clear();
    empty->setHidden(true);
    qDebug() << str;
    emit doSearch(str);
}

void SearchBox::editClear() {
    text->clear();
    empty->setHidden(true);
}

