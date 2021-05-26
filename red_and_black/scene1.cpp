#include "scene1.h"
#include "ui_scene1.h"

scene1::scene1(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::scene1)
{
    ui->setupUi(this);
}

scene1::~scene1()
{
    delete ui;
}
