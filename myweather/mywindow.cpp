#include "mywindow.h"
#include <QToolButton>
#include <QBitmap>
#include <QStyle>

// highcontrast/actions/16*16
MyWindow::MyWindow(QWidget *parent) : QWidget(parent)
{
    city = new QLabel(this);
    hlayout = new QHBoxLayout();
    refresh = new QToolButton(this);
    close = new QPushButton(this);
    minimize = new QPushButton(this);
    maximize = new QPushButton(this);
//    palette = new QPalette;
    setup();
    setupButtons();
    setupCity();
    setupLayout();
    setupPalette();
}

void MyWindow::setup() {
    this->setFixedHeight(36);
    this->setMinimumWidth(750);
    this->setWindowTitle("weather");
}

void MyWindow::setupButtons() {
//    QString qss = "border: none";
    close->setIcon(this->style()->standardPixmap(QStyle::SP_TitleBarCloseButton));
    minimize->setIcon(this->style()->standardPixmap(QStyle::SP_TitleBarMinButton));
    maximize->setIcon(this->style()->standardPixmap(QStyle::SP_TitleBarMaxButton));
    refresh->setIcon(QPixmap(":/imgs/imgs/view-refresh.png"));
//    close->setStyleSheet(qss);
//    minimize->setStyleSheet(qss);
//    maximize->setStyleSheet(qss);
    close->setFixedSize(32,32);
    close->setFlat(true);
    maximize->setFlat(true);
    minimize->setFlat(true);
//    close->setMask(QPixmap(":/imgs/imgs/close-32.png").mask());
    minimize->setFixedSize(32,32);
    maximize->setFixedSize(32,32);
    refresh->setFixedSize(32,32);
}


void MyWindow::setupCity() {
//    city->setFont(QFont("Timers", 20));
    city->setText("City");
    city->setFixedHeight(32);
    city->setMinimumWidth(600);
    city->setAlignment(Qt::AlignCenter);
}

void MyWindow::setupLayout() {
    hlayout->addWidget(city);
    hlayout->addWidget(refresh);
    hlayout->addWidget(minimize);
    hlayout->addWidget(maximize);
    hlayout->addWidget(close);
    hlayout->setContentsMargins(0,4,0,0); // key!!
    setLayout(hlayout);
}

void MyWindow::setupPalette() {
//    palette->setBrush(this->backgroundRole(), QBrush(Qt::transparent));
//    this->setPalette(*palette);
//    this->setAutoFillBackground(true);
}


void MyWindow::setTitle(QString str) {
    city->setText(str);
}
