#ifndef MYWINDOW_H
#define MYWINDOW_H
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QPalette>

class MyWindow : public QWidget
{
    Q_OBJECT
    QHBoxLayout * hlayout;
    QPushButton * close;
    QPushButton * minimize;
    QPushButton * maximize;
    QToolButton * refresh;
    QLabel * city;
    QPalette * palette;
public:
    explicit MyWindow(QWidget *parent = nullptr);
    void setTitle(QString str);
    friend class Weather;
private:
    void setup();
    void setupButtons();\
    void setupCity();
    void setupPalette();
    void setupLayout();
signals:

public slots:

};

#endif // MYWINDOW_H
