#include "secondscene.h"
#include<QPaintEvent>
#include<QMenuBar>
#include<qpainter.h>
#include<qpushbutton.h>
#include<qdebug.h>
#include<mainwindow.h>
secondscene::secondscene(QWidget *parent) : QMainWindow(parent)
{
    //配置第二场景的属性
    this->setWindowTitle("红黑夜法官");
    this->setWindowIcon(QIcon(":/res/windowicon.jpg"));
    this->setFixedSize(650,975);

    //菜单栏的创建
    QMenuBar    *bar=menuBar();
    setMenuBar(bar);
    //创建开始菜单
    QMenu   *startMenu=bar->addMenu("开始");
    QAction *quitAction=startMenu->addAction("退出");

    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });
QPushButton *jxbtn=new  QPushButton;
    jxbtn->resize(80,40);
    jxbtn->setText("竞选村长");
    jxbtn->setParent(this);
    jxbtn->move(270,400);



}



void secondscene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/scene1.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

}
