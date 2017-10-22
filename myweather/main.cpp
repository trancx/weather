#include "weather.h"
#include <QApplication>
#include <QListWidget>
#include "resultitem.h"
#include "centralwidget.h"

const QEvent::Type WeatherEventType
    = (QEvent::Type)QEvent::registerEventType();
/* it requires a forced conversion */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Weather w(NULL, 770, 548);
    w.show();


    return a.exec();
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
