#include "centralwidget.h"
#include <QBrush>
#include <QPalette>
#include <cstring>
#include <QDebug>

extern "C" {
    #include "query.h"
    #include "result.h"
}



CentralWidget::~CentralWidget() {
    delete w; // contains icon tem, setlayout(local)
    delete local;
    delete global;
    delete icon;
    delete temperature;
    delete weather;
    delete query;
}

CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
{

}

CentralWidget::CentralWidget(QWidget * parent,
        int fixedwidth, int fixedheight) : QWidget(parent)
{
    instantiate();
    this->setMaximumSize(fixedwidth, fixedheight);
//    this->setContentsMargins(0,0,0,0);
    w->setFixedSize(fixedwidth, 180);
    icon->setFixedSize(180, 180);
    temperature->setFixedSize(fixedwidth,100 );
    weather->setFixedSize(fixedwidth, 150);

    setupStyle();
    setupLayout();
}

void CentralWidget::setupStyle() {
    QString weatherqss = "font: italic 50pt \"Overpass\"; color: rgb(211, 215, 207)";
    weather->setStyleSheet(weatherqss);
    weather->setContentsMargins(0,0,0,0);
    weather->setAlignment( Qt::AlignTop | Qt::AlignHCenter);
    QString tempss = "font: italic 100pt \"AR PL UMing CN\"; color: rgb(211, 215, 207)";
    temperature->setStyleSheet(tempss);
    temperature->setAlignment( Qt::AlignCenter);

    QPalette p = icon->palette();
    p.setBrush(QPalette::Background,
               QBrush(QPixmap(":/imgs/icons/99.png").scaled(icon->size())));
    icon->setPalette(p);
    icon->setAutoFillBackground(true);
}

void CentralWidget::setupLayout() {
//    icon->setFrameShape(QFrame::Panel);
//    temperature->setFrameShape(QFrame::Panel);
//    weather->setFrameShape(QFrame::Panel);
    local->addWidget(icon);    
    local->setContentsMargins(0,0,0,0);
    local->setAlignment(icon, Qt::AlignCenter);
    w->setLayout(local);

    global->addWidget(w);
    global->addWidget(temperature);
    global->setAlignment(temperature, Qt::AlignHCenter);
    global->setAlignment(weather, Qt::AlignHCenter | Qt::AlignTop);
    global->addWidget(weather);
    global->setContentsMargins(0,0,0,0);
    setLayout(global);
}

void CentralWidget::instantiate() {
    w = new QWidget(this); // contains icon tem, setlayout(local)
    local = new QHBoxLayout();
    global = new QVBoxLayout();
    icon = new QLabel(w);
    temperature = new QLabel(this);
    weather = new QLabel(this);
    query = new struct query;
    query->flag &= 0;
    query->arg = this;
    // FIXME： 这是多余的， query本身可以用来验证了
    // 这是为了发送事件的时候确认aim而已。。。。
    query->size = 0;
    query->data = NULL;
}

bool CentralWidget::event(QEvent * event) {
    if( event->type() == WeatherEventType ) {
        qDebug() << "WeatherEvent recieved";
        weatherEvent(event);
        event->accept(); /* means we have hanled it */
        return true;
        /*
        This function returns true if the event was
        recognized, otherwise it returns false.
        If the recognized event was accepted
        (see QEvent::accepted), any further processing
        such as event propagation to the parent widget stops.
        */
    }
    return QWidget::event(event);
}

void CentralWidget::weatherEvent(QEvent * event ) {
    QWeatherEvent * wevent = (QWeatherEvent * )event;
    qDebug() << "event recieved!";
    setTemperature(wevent->getTemperature());
    setWeather(wevent->getWeather());
    setWeatherIcon(wevent->getCode());
}


void CentralWidget::setUrl(QString str) {
    qDebug() << "Url: " << str;
    std::string s = str.toStdString();
    query->url = (char *)s.c_str();
}


void CentralWidget::setTemperature(QString str ) {
    temperature->setText(str);
}

void CentralWidget::setWeather(QString str) {
    weather->setText(str);
}

void CentralWidget::setWeatherIcon(QString str) {
    QString path = ":/imgs/icons/" + str + ".png";
    qDebug() << path;
    QPalette p = icon->palette();
    p.setBrush(QPalette::Background,
               QBrush(QPixmap(path).scaled(icon->size())));
    icon->setPalette(p);
    icon->setAutoFillBackground(true);
}

void CentralWidget::refresh()
{
        query->flag &= 0;
        query->flag |= QUERY_UPDATE;
        getWeather(); // normal orther!
}

void CentralWidget::getWeather() {
    ::send_attach(query);
}
