#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<secondscene.h>

extern int n1;//总游戏人数
extern int rednum;//剩余的红方人数
extern int blacknum;//剩余的黑方人数

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //重写paintevent事件
   void  paintEvent(QPaintEvent *);
   void getNum();//获得游戏游玩总人数
   void start();//游戏开始
   void end();//判断游戏结束
   void setplay();//初始化游戏进程
   void jxbtn();//竞选按钮
   void game();//游戏进程函数
   void playerinform();//设置角色信息的显示
   void votecunzhang();//村长竞选
   void speakorder();//决定发言顺序
   void getjxnum();//获得竞选人数
   void  nignt();//夜晚情况
   void voteoutfunc();//公投环节的投人出局函数







private:
    Ui::MainWindow *ui;
    int n;//总游戏人数
    int rednum;//剩余的红方人数
    int blacknum;//剩余的黑方人数
    int jxsum=0;
    int czid;
    bool nightstate;//0表示黑夜，1表示红夜
    int player[11];

};


#endif // MAINWINDOW_H
