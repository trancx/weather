#include "weather.h"
#include <QMouseEvent>
#include <QPixmap>
#include <QBrush>

Weather::Weather(QFrame *parent, int minw, int minh) : QFrame(parent)
{

    setWindowIcon(QIcon(QPixmap(":/imgs/pix/im-gadugadu.png")));
    setWindowFlags(Qt::FramelessWindowHint);
    setMouseTracking(true);
    this->setFixedSize(minw+50, minh+50);
    widget = new MainWidget(this, minw-4, minh-36);
    window = new MyWindow(this);
    layout = new QVBoxLayout();
    layout->addWidget(window);
    layout->addWidget(widget);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    setupImage();
    setupSig();
}

void Weather::setupSig() {
        /* interact with min/max/close button  */
    connect(window->close, SIGNAL(clicked()),
            this, SLOT(close()) );
//    connect(window->maximize, SIGNAL(clicked()),
//    this, SLOT(showMaximized()));
    connect(window->minimize, SIGNAL(clicked()),
            this, SLOT(showMinimized()) );

        /* change title */
    connect(widget, SIGNAL(emitChanged(ResultItem*)),
            this, SLOT(changeTitle(ResultItem*)) );

        /* refresh */
    connect(window->refresh, SIGNAL(clicked()),
            widget, SLOT(refreshCentralWidget()) );
}

void Weather::setupImage() {
//    background-color: rgb(52, 101, 164);
//    QPixmap p (":/imgs/background/base.png");
    QPalette plaette;
//    this->setWindowFlags(Qt::FramelessWindowHint);
    plaette.setBrush(widget->backgroundRole(),
        QBrush(QPixmap(":/imgs/pix/foggy.jpg").scaled(this->size())));
    widget->setPalette(plaette);
    widget->setAutoFillBackground(true);
}

void Weather::mouseMoveEvent(QMouseEvent * event) {
    if( mouse_press ) {
        QPoint mos = event->globalPos();
        this->move(mos-move_point);
    }
}

void Weather::mousePressEvent(QMouseEvent * event) {
    if( event->button() == Qt::LeftButton ) {
        mouse_press = true;
        move_point = event->pos();
    }
}

void Weather::mouseReleaseEvent(QMouseEvent * event) {
    if( mouse_press )
        mouse_press = false;
    QFrame::mouseReleaseEvent(event);
}


void Weather::changeTitle(ResultItem * addr ) {
    setWindowTitle(addr->getName());
    window->setTitle(addr->getName());
}
