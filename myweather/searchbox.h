#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include <QFrame>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
class SearchBox : public QWidget
{
    Q_OBJECT
public:
    explicit SearchBox(QWidget *parent, int fixedwidth, int fixedheight);
private:
    QLineEdit * text;
    QPushButton * search;
    QPushButton * empty;
    QHBoxLayout * hl;
    void setup();
signals:
    void doSearch(QString);
public slots:
    void textEdit(QString);
    void searchClicked();
    void editClear();
protected:
    void mousePressEvent(QKeyEvent *);
};

#endif // SEARCHBOX_H
