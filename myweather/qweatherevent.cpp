#include "qweatherevent.h"
#include <QLatin1String>


QWeatherEvent::QWeatherEvent(Type type, struct query * result)
                    : QEvent(type), q(result) {

}

QString QWeatherEvent::getWeather() {

    struct result * res =
                (struct result *)q->data;
    char * s = res->now->text.ptr;
    return QString(QLatin1String(s));
}

QString QWeatherEvent::getCode() {
    struct result * res =
                (struct result *)q->data;
    char * s = res->now->code.ptr;

    return QString(s);
}

QString QWeatherEvent::getTemperature() {
    struct result * res =
                (struct result *)q->data;
    char* s = res->now->temperature.ptr;

    return QString(s);
}
