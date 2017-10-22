#ifndef QWEATHEREVENT_H
#define QWEATHEREVENT_H

#include <QEvent>
extern "C" {
    #include "result.h"
    #include "query.h"
}

class QWeatherEvent : public QEvent
{
public:
    QWeatherEvent(Type type, struct query *);
    QString getTemperature();
    QString getWeather();
    QString getCode();
private:
    struct query * q;
};

#endif // QWEATHEREVENT_H
