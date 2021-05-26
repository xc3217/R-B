#ifndef SECONDSCENE_H
#define SECONDSCENE_H

#include <QMainWindow>
#include <QWidget>
#include<qpushbutton.h>

class secondscene : public QMainWindow
{
    Q_OBJECT
public:
    explicit secondscene(QWidget *parent = nullptr);

    //重写paintevent
    void  paintEvent(QPaintEvent *);

signals:

};

#endif // SECONDSCENE_H
