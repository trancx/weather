#ifndef WEATHER_H
#define WEATHER_H
#include "mainwidget.h"
#include "mywindow.h"

class Weather : public QFrame
{
    Q_OBJECT
public:
    explicit Weather(QFrame * parent, int minw, int minh);
private:
    MainWidget * widget;
    MyWindow * window;
    QVBoxLayout * layout;
    QPoint move_point;
    bool mouse_press;
    void setupImage();
    void setupSig();
protected:
    /* key to design your own window! */
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
public slots:
    void changeTitle(ResultItem *);
};

#endif // WEATHER_H
