#include "weather.h"
#include <QApplication>
#include <QListWidget>
#include "resultitem.h"
#include "centralwidget.h"
#include <QDebug>

extern "C" {
    #include <init.h>
}

const QEvent::Type WeatherEventType
    = (QEvent::Type)QEvent::registerEventType();
/* it requires a forced conversion */

extern "C" {
int mainWidget(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Weather w(NULL, 770, 548);
    w.show();
    init();

    return a.exec();
    }
}
extern "C" {
    void notifyWeather(struct query * q) {
        QCoreApplication::postEvent((CentralWidget *)q->arg,
                    new QWeatherEvent(WeatherEventType, q));

    }
    /**
     *  it would be better not using q->arg,
     * even it acts more efficient, direct.
     * but's maybe it's hard to maintain...
     *
     * OMG totally coupling.....but i truly
     * learn a lot.
     */
}
