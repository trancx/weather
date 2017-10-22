#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "qweatherevent.h"
/**
 * @brief The CentralWidget class
 *
 * it violates OOP principle, but we
 * just use it once.....
 *                          --Trance
 */

extern const QEvent::Type WeatherEventType;

class CentralWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *parent = nullptr);
    explicit CentralWidget(QWidget *, int fixedwidth, int fixedheight );
    ~CentralWidget();
    void setWeatherIcon(QString);
    void setTemperature(QString);
    void setWeather(QString);
    void updateWeather();
    void setUrl(QString);
    void getWeather();
    void refresh();
private:
    void instantiate();
    void setupLayout();
    void setupStyle();
    void weatherEvent(QEvent *);
    QWidget * w; // contains icon tem, setlayout(local)
    QHBoxLayout * local;
    QVBoxLayout * global;
    QLabel * icon;
    QLabel * temperature;
    QLabel * weather;
    struct query * query;
protected:
    bool event(QEvent *);
signals:

public slots:
};

#endif // CENTRALWIDGET_H
