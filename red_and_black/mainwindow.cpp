#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<qpainter.h>
#include<gameplayer.h>
#include<qdebug.h>
#include<QString>
#include<qpushbutton.h>
#include<qtimer.h>
#include<QMessageBox>
#include<qmediaplayer.h>
#include<qsound.h>
static int afcount;
static int livesum;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
     ui(new Ui::MainWindow)
{  //背景音乐设置
   QSound *music=new QSound(":/res/bgm.wav",this);
   music->play();



    ui->setupUi(this);
    //设置窗口的基本信息
    this->setWindowTitle("红黑夜法官");
    this->setWindowIcon(QIcon(":/res/windowicon.jpg"));
    this->setFixedSize(650,975);
    //退出按钮的实现
    connect(ui->actionclose,&QAction::triggered,[=](){

        this->close();

    });
    ui->yiyanbtn->hide();//发表遗言按钮
    ui->afspeak->hide();//竞选结束后的发言按钮
    ui->nextbtn->hide();//竞选结束的next按钮
    ui->nightbtn->hide();//nightbtn按钮的消失
    ui->gamewidget->setCurrentWidget(ui->page);
    //nightbtn的设置
    connect(ui->nightbtn,&QPushButton::clicked,[=](){
        if(rand()%2==0)
        {
            nightstate=0;
            ui->duihua->append("下面进入黑夜");
            QMessageBox *box1=new QMessageBox;
            box1->setText("下面进入黑夜");
            box1->exec();
        }
        else
           {
            nightstate=1;
            ui->duihua->append("下面进入红夜");
            QMessageBox *box2=new QMessageBox;
            box2->setText("下面进入红夜");
            box2->exec();
        }
        ui->gamewidget->setCurrentWidget(ui->eyewidget);

    });


    //开始游戏


    connect(ui->startbtn,&QPushButton::clicked,[=](){

            ui->gamewidget->setCurrentWidget(ui->jxpage);
            game();
            qDebug()<<"天亮了，下面进行村长的竞选";

    });

    //afspeak按钮的设置
    //static int afcount;//村长的座次序号
    static int countsum2=0;//用来记录发言的人数
    connect(ui->afspeak,&QPushButton::clicked,[=](){
        if(ui->leftradioButton->isChecked())
        {   if(afcount>1)   afcount-=1;
            else  afcount+=n-1;
            if(players[afcount].state==1)
            {countsum2++;
            players[afcount].word=ui->lineEdit->text();
            ui->duihua->append(QString::number(afcount));
            ui->duihua->insertPlainText("号玩家发言:");
            ui->duihua->append(players[afcount].word);
            ui->lineEdit->setText("");
            }
            if(countsum2==livesum)
            {ui->duihua->append("所有人讲话完毕");
                ui->nextbtn->show();
                ui->afspeak->hide();
                voteoutfunc();
                countsum2=0;
            }
        }
        if(ui->rightradioButton->isChecked())
        {
            if(afcount<n)   afcount+=1;
            else    afcount-=n-1;
            if(players[afcount].state)
            {
                countsum2++;
                players[afcount].word=ui->lineEdit->text();
                ui->duihua->append(QString::number(afcount));
                ui->duihua->insertPlainText("号玩家发言:");
                ui->duihua->append(players[afcount].word);
                ui->lineEdit->setText("");
            }
            if(countsum2==livesum)
            {
                ui->duihua->append("所有人讲话完毕");
                ui->nextbtn->show();
                voteoutfunc();
                countsum2=0;
            }
        }
    });
    //遗言按钮的设置
    connect(ui->yiyanbtn,&QPushButton::clicked,[=](){
        QString word=ui->lineEdit->text();
        ui->duihua->append(word);
        ui->yiyanbtn->hide();
        ui->afspeak->hide();

    });
    //竞选界面的back按钮
    connect(ui->backbtn,&QPushButton::clicked,[=](){

       ui->gamewidget->setCurrentWidget(ui->afterjx);
        playerinform();

    });
    //决定发言顺序的按钮
    connect(ui->orderbtn,&QPushButton::clicked,[=](){
        speakorder();
        ui->gamewidget->setCurrentWidget(ui->afterjx);
        if(ui->leftradioButton->isChecked())    ui->duihua->append("村长决定从左手边开始发言");
        if(ui->rightradioButton->isChecked())   ui->duihua->append("村长决定从右手边开始发言");
        ui->afspeak->show();
    });
    //确认发言按钮
    static int p=0;
    connect(ui->speakbtn,&QPushButton::clicked,[=](){

        p++;
        //竞选时的发言按钮设置
        if(players[p].state==1&&players[p].jxcun==1)
             {players[p].word=ui->lineEdit->text();
             ui->duihua->append("有请");
             QString s=QString::number(p);
             ui->duihua->insertPlainText(s);
             ui->duihua->insertPlainText("号玩家发表竞选演讲");
             ui->duihua->append(players[p].word);
             ui->lineEdit->setText("");
        }

    });
    connect(ui->notspeakbtn,&QPushButton::clicked,[=](){
        p++;
        if(p>=jxsum)    ui->gamewidget->setCurrentWidget(ui->jxcunz);
        ui->notspeakbtn->hide();
        ui->speakbtn->hide();
        ui->afspeak->show();
        votecunzhang();

    });
    //对话框的基本内容
    ui->duihua->append("游戏开始");
    ui->duihua->append("请玩家选择是否竞选");
    ui->duihua->append("现在有请竞选者发言");

    //nextbtn的设置
    connect(ui->nextbtn,&QPushButton::clicked,[=](){

        QMessageBox *votebox=new QMessageBox;
        votebox->setText("进入全民公投环节");
        votebox->exec();
        ui->gamewidget->setCurrentWidget(ui->voteout);
        for(int i=1;i<=n;i++)
            if(players[i].state==1) ui->voteoutbox->addItem(QString::number(i));
    });
}
void MainWindow:: paintEvent(QPaintEvent *)//设置开始背景
{

    QPainter painter(this);
    QPixmap pix;
    //加载图片
    pix.load(":/res/scene1.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::jxbtn()
{
    //是否竞选村长
    static int i=0;
    connect(ui->jxBtn,&QPushButton::clicked,[=](){


            i++;
            players[i].jxcun=1;
            if(i>=n) ui->gamewidget->setCurrentWidget(ui->afterjx);
    });

    connect(ui->bjxbtn,&QPushButton::clicked,[=](){


            i++;
            players[i].jxcun=0;
            if(i>=n) ui->gamewidget->setCurrentWidget(ui->afterjx);
    });


}
void  MainWindow::getNum()
{
    n=ui->numbox->currentText().toInt();


}
//游戏开始时的人数统计
void   MainWindow::start()
{   getNum();
    qDebug()<<"参与游戏的人数为"<<n;
    livesum=n;
}


//游戏结束
void    MainWindow::end()
{
    if(rednum==0)
    {QMessageBox *redbox=new QMessageBox;
        redbox->setText("黑方获得胜利");
        redbox->exec();
     this->close();
    }

    if(blacknum==0)
    {
     QMessageBox *blackbox=new QMessageBox;
     blackbox->setText("红方获得胜利");
     blackbox->exec();
        this->close();
    }

}
//游戏初始化
void MainWindow::setplay()
{
    if(n%2==0)
    {rednum=n/2;
        blacknum=n/2;
        }
    else
    {rednum=n/2+1;
        blacknum=n/2;
    }//记录红黑的人数
    int countnumred=0;
    int countnumblack=0;
    for(int i=1;i<=n;i++)
    {
        if(rand()%2==0)
        {   countnumred++;
            if(countnumred<=rednum)
            players[i].status=1;
            else players[i].status=0;
         }
        else
        {   countnumblack++;
            if(countnumblack<=blacknum)
            players[i].status=0;
            else players[i].status=1;
        }
        players[i].ID=i+1;
        players[i].point_have=1;
        players[i].points_voted=0;
        players[i].points_votedcz=0;
        players[i].state=true;
        player[i]=1;
    }
}
//通过表格形式显示游戏玩家的信息
void    MainWindow::playerinform()
{
    //玩家信息情况
    ui->information->setStyleSheet("background-color:transparent");//设置tablewidget背景颜色为透明
    ui->information->setRowCount(n);
    ui->information->setColumnCount(5);
    ui->information->setHorizontalHeaderLabels(QStringList()<<"玩家id"<<"存活状况"<<"玩家身份"<<"是否为村长"<<"睁眼情况");
    ui->information->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //玩家id在表格中的设置
    for(int i=1;i<=n;i++)
    {   if(i==1) ui->information->setItem(i-1,0,new QTableWidgetItem("1"));
        if(i==2) ui->information->setItem(i-1,0,new QTableWidgetItem("2"));
        if(i==3) ui->information->setItem(i-1,0,new QTableWidgetItem("3"));
        if(i==4) ui->information->setItem(i-1,0,new QTableWidgetItem("4"));
        if(i==5) ui->information->setItem(i-1,0,new QTableWidgetItem("5"));
        if(i==6) ui->information->setItem(i-1,0,new QTableWidgetItem("6"));
        if(i==7) ui->information->setItem(i-1,0,new QTableWidgetItem("7"));
        if(i==8) ui->information->setItem(i-1,0,new QTableWidgetItem("8"));
        if(i==9) ui->information->setItem(i-1,0,new QTableWidgetItem("9"));
        if(i==10) ui->information->setItem(i-1,0,new QTableWidgetItem("10"));
    }
    //设置玩家存活状况的表示
    for(int i=1;i<=n;i++)
    {
        if(players[i].state)    ui->information->setItem(i-1,1,new QTableWidgetItem("存活"));
        else    ui->information->setItem(i-1,1,new QTableWidgetItem("死亡"));
    }
    //设置玩家身份的表格显示
    for(int i=1;i<=n;i++)
    {
        if(players[i].status)   ui->information->setItem(i-1,2,new QTableWidgetItem("红色身份"));
                else    ui->information->setItem(i-1,2,new QTableWidgetItem("黑色身份"));
    }
    //设置玩家是否为村长
    for(int i=1;i<=n;i++)
    {
        if(players[i].cunzhang) ui->information->setItem(i-1,3,new QTableWidgetItem("村长"));
        else ui->information->setItem(i-1,3,new QTableWidgetItem("村民"));

    }
    //睁眼情况的设置
    for(int i=1;i<=n;i++)
    {
     if(players[i].eye) ui->information->setItem(i-1,4,new QTableWidgetItem("睁着浓眉大眼"));
        else    ui->information->setItem(i-1,4,new QTableWidgetItem("闭得死死的"));
    }

}
//竞选环节的游戏设置
void MainWindow::votecunzhang()
{   //把所有竞选人名字加入
    getjxnum();
    for(int k=1;k<=n;k++)
       {  if(players[k].jxcun==1)
          {    ui->jxplayer->addItem(QString::number(k));
               ui->jxplayer->setCurrentIndex(0);
          }
      }

        static int j=0;
        connect(ui->votebtn,&QPushButton::clicked,[=](){
           int p=ui->jxplayer->currentText().toInt();
           players[p].points_votedcz+=1;
           j++;
           if(j>=n-jxsum)
           {   //平票处理
               if(jxsum>1&&jxsum<n) {
                   int cunzsum=0;
                   int max=players[1].points_votedcz;
                   for(int i=1;i<=n;i++)
                       if(max<players[i].points_votedcz)
                       {
                           max=players[i].points_votedcz;
                       }//记录下最高票数
                   for(int i=1;i<=n;i++)
                   {if(max==players[i].points_votedcz)
                           cunzsum+=1;
                   }//记录下最高票票人数
                   if(cunzsum==1)//若只有一人获得最高票则进入常规判定
                   {
                       for(int i=1;i<=n;i++)
                           if(players[i].points_votedcz==max)
                           {
                               players[i].cunzhang=1;
                               if(n%2==0)  players[i].point_have=1.5;
                               if(n%2!=0)  players[i].point_have=0.5;
                               czid=i;
                               break;
                           }
                   }
                       if(cunzsum>1)//若超过一人，则采取随机出局方式
                       {   int cunzplayer[10]={0};
                          int j=0;
                        for(int i=1;i<=n;i++)
                              { if(players[i].points_votedcz==max)
                                  { cunzplayer[j]=i;
                                  j+=1;
                                 }
                               }
                         players[cunzplayer[rand()%cunzsum]].cunzhang=1;
                         for(int czi2=1;czi2<=n;czi2++)
                           {  if(players[czi2].cunzhang==1)
                             {
                                  czid=czi2;
                                  qDebug()<<czid;
                                 if(n%2==0)  players[czi2].point_have=1.5;
                                if(n%2!=0)  players[czi2].point_have=0.5;
                                   break;
                              }
                           }
                       }
                   }
               ui->gamewidget->setCurrentWidget(ui->orderwidget);
           }
        });
        connect(ui->cancelvote,&QPushButton::clicked,[=](){
            j++;
            if(j>=n-jxsum)
            {   //平票处理
                if(jxsum>1&&jxsum<n) {
                    int cunzsum=0;
                    int max=players[1].points_votedcz;
                    for(int i=1;i<=n;i++)
                        if(max<players[i].points_votedcz)
                        {
                            max=players[i].points_votedcz;
                        }
                    for(int i=1;i<=n;i++)
                    {if(max==players[i].points_votedcz)
                            cunzsum+=1;
                    }
                    if(cunzsum==1)
                    {
                        for(int i=1;i<=n;i++)
                            if(players[i].points_votedcz==max)
                            {
                                players[i].cunzhang=1;
                                if(n%2==0)  players[i].point_have=1.5;
                                if(n%2!=0)  players[i].point_have=0.5;
                                czid=i;
                                break;
                            }
                    }
                        if(cunzsum>1)
                        {   int cunzplayer[10]={0};
                           int j=0;
                         for(int i=1;i<=n;i++)
                               { if(players[i].points_votedcz==max)
                                   { cunzplayer[j]=i;
                                   j+=1;
                                  }
                                }
                          players[cunzplayer[rand()%cunzsum]].cunzhang=1;
                          for(int czi2=1;czi2<=n;czi2++)
                            {  if(players[czi2].cunzhang==1)
                              {
                                   czid=czi2;
                                   qDebug()<<czid;
                                  if(n%2==0)  players[czi2].point_have=1.5;
                                 if(n%2!=0)  players[czi2].point_have=0.5;
                                    break;
                               }
                            }
                        }
                    }
                ui->gamewidget->setCurrentWidget(ui->orderwidget);
            }
        });



    if(jxsum==n||jxsum==0)
    {
        players[rand()%n+1].cunzhang=1;
        for(int czi=1;czi<=n;czi++)
            if(players[czi].cunzhang==1)
            {czid=czi;
                if(n%2==0)  players[czi].point_have=1.5;
                if(n%2!=0)  players[czi].point_have=0.5;
                break;
            }
        ui->gamewidget->setCurrentWidget(ui->orderwidget);
    }
    if(jxsum==1)
    {
        for(int i=1;i<=n;i++)
        {if(players[i].jxcun==1) players[i].cunzhang=1;
            czid=i;
            break;
        }
        ui->gamewidget->setCurrentWidget(ui->orderwidget);
        playerinform();

    }
    afcount = czid ;
    livesum=rednum+blacknum;
}

void    MainWindow::getjxnum(){

    for(int i=0;i<=n;i++)
        if(players[i].jxcun==1) jxsum++;

}

void MainWindow::speakorder()
{    ui->leftradioButton->setChecked(false);
     ui->rightradioButton->setChecked(false);
    for(int i=0;i<=n;i++)
        players[i].jxcun=0;
    playerinform();
    ui->nextbtn->show();


}

void MainWindow::nignt()
{   static int redeye=0;
    static int blackeye=0;
    static int eyenum=1;//记录序号
    static  int eyesum=0;//记录人数
    connect(ui->openeye,&QPushButton::clicked,[=](){
        while(players[eyenum].state==0){
            eyenum++;
        }
        players[eyenum].eye=1;
        if(players[eyenum].status==1)   redeye++;
        if(players[eyenum].status==0)   blackeye++;
        eyesum++;
        eyenum++;
        if(eyesum==livesum)
            {ui->gamewidget->setCurrentWidget(ui->orderwidget);
            speakorder();
            eyenum=1;
            eyesum=0;
            }
        ui->nightbtn->hide();
        playerinform();
    });

    connect(ui->eyeclose,&QPushButton::clicked,[=](){
        while(players[eyenum].state==0){
            eyenum++;
        }
        players[eyenum].eye=0;
        eyesum++;
        eyenum++;
        if(eyesum==livesum) {ui->gamewidget->setCurrentWidget(ui->orderwidget);
            speakorder();
            eyesum=0;
            eyenum=1;
        }
        playerinform();
        ui->nightbtn->hide();

    });

    if(nightstate==1&&redeye%2==0){ players[czid].point_have=1;

    }
    if(nightstate==0&&blackeye%2==0)    players[czid].point_have=1;
}

void MainWindow::voteoutfunc()
{   static int countsum=0;//记录序号
    static int sum=0;//记录投票的人数
    connect(ui->voteoutbtn,&QPushButton::clicked,[=](){
           while(players[countsum].state==1)
           {
               countsum++;
           }
           int q=ui->voteoutbox->currentText().toInt();
           players[q].points_voted+=players[countsum].point_have;
           countsum++;
           sum++;
        if(sum==livesum)
        {   int voutsum=0;//记录最大票人数
            int voutplayer[10]={0};//记录平票人的序号
            double maxsum=0;
            for(int i=1;i<=n;i++)
            {
                if(players[i].state==1)
                {
                    if(maxsum<players[i].points_voted) maxsum=players[i].points_voted;
                }
            }//记录最大票数
            //记录最大票人数
            for(int i=1;i<=n;i++)
            {
                if(players[i].points_voted==maxsum) voutsum++;
            }

            if(voutsum==1)
            {
            for(int j=1;j<=n;j++)
            {
                if(players[j].points_voted==maxsum)
                {players[j].state=0;
                    player[j+1]=0;
                    ui->duihua->append("本次公投淘汰的玩家是");
                    ui->duihua->append(QString::number(j));
                    if(players[j].status==1)
                    {   rednum-=1;
                        livesum-=1;
                        ui->duihua->append("他的身份是红色");
                        QMessageBox *box3=new QMessageBox;
                        box3->setText("有一名玩家被淘汰，身份为红色");
                        box3->exec();
                        ui->duihua->append("请淘汰玩家发表遗言");
                        ui->yiyanbtn->show();
                   }
                    else
                    {   blacknum-=1;
                        livesum-=1;
                        ui->duihua->append("他的身份是黑色");
                        QMessageBox *box4=new QMessageBox;
                        box4->setText("有一名玩家被淘汰，身份为黑色");
                        box4->exec();
                        ui->duihua->append("请淘汰玩家发表遗言");
                        ui->yiyanbtn->show();
                        end();
                    }
                    ui->voteoutbox->clear();
                    for(int i=1;i<=n;i++)
                        players[i].points_voted=0;
                    break;

                }
            }
        }
            if(voutsum>1)
            {  int j=0;
                for(int i=1;i<=n;i++)
                  {  if(players[i].points_voted==maxsum)
                        {voutplayer[j]=i;
                        j+=1;
                        }
                  }
                players[voutplayer[rand()%voutsum]].state=0;
                for(int i=0;i<=voutsum;i++)
                {   int tempt=voutplayer[i];
                    if(players[tempt].state==0)
                    {
                        player[tempt+1]=0;
                        ui->duihua->append("本次公投淘汰的玩家是");
                        ui->duihua->append(QString::number(tempt));
                        if(players[tempt].status==1)
                        {
                            rednum-=1;
                            livesum-=1;
                            ui->duihua->append("他的身份是红色");
                            QMessageBox *box5=new QMessageBox;
                            box5->setText("有一位玩家被淘汰，身份为红色");
                            box5->exec();
                            ui->duihua->append("请淘汰玩家发表遗言");
                            ui->yiyanbtn->show();
                            end();
                        }
                        else
                        {
                            blacknum-=1;
                            livesum-=1;
                            ui->duihua->append("他的身份是黑色");
                            QMessageBox *box6=new QMessageBox;
                            box6->setText("有一位玩家被淘汰，身份为黑色");
                            box6->exec();
                            break;
                            ui->duihua->append("请淘汰玩家发表遗言");
                            ui->yiyanbtn->show();
                            end();
                        }
                        for(int i=1;i<=n;i++)
                            players[i].points_voted=0;
                        break;
                    }

                }

            }
            sum=0;
            countsum=0;//静态变量归零
            ui->voteoutbox->clear();//重置投票下拉框
            for(int i=0;i<10;i++)
            voutplayer[i]=0;
            playerinform();//更新玩家信息
            ui->gamewidget->setCurrentWidget(ui->afterjx);
            ui->nightbtn->show();
            ui->nextbtn->hide();
        }
        end();

    });
    connect(ui->novoteoutbtn,&QPushButton::clicked,[=](){

        countsum++;
        sum++;
        if(sum==livesum)
        {   int voutsum=0;//记录最大票人数
            int voutplayer[10]={0};//记录平票人的序号
            double maxsum=0;
            for(int i=1;i<=n;i++)
            {
                if(players[i].state==1)
                {
                    if(maxsum<players[i].points_voted) maxsum=players[i].points_voted;
                }
            }//记录最大票数
            //记录最大票人数
            for(int i=1;i<=n;i++)
            {
                if(players[i].points_voted==maxsum) voutsum++;
            }

            if(voutsum==1)
            {
            for(int j=1;j<=n;j++)
            {
                if(players[j].points_voted==maxsum)
                {players[j].state=0;
                    player[j+1]=0;
                    ui->duihua->append("本次公投淘汰的玩家是");
                    ui->duihua->append(QString::number(j));
                    if(players[j].status==1)
                    {   rednum-=1;
                        livesum-=1;
                        ui->duihua->append("他的身份是红色");
                        QMessageBox *box3=new QMessageBox;
                        box3->setText("有一名玩家被淘汰，身份为红色");
                        box3->exec();
                        ui->duihua->append("请淘汰玩家发表遗言");
                        ui->yiyanbtn->show();
                        end();
                   }
                    else
                    {   blacknum-=1;
                        livesum-=1;
                        ui->duihua->append("他的身份是黑色");
                        QMessageBox *box4=new QMessageBox;
                        box4->setText("有一名玩家被淘汰，身份为黑色");
                        box4->exec();
                        ui->duihua->append("请淘汰玩家发表遗言");
                        ui->yiyanbtn->show();
                        end();
                    }
                    ui->voteoutbox->clear();
                    for(int i=1;i<=n;i++)
                        players[i].points_voted=0;
                    break;
                }
            }
        }
            if(voutsum>1)
            {  int j=0;
                for(int i=1;i<=n;i++)
                  {  if(players[i].points_voted==maxsum)
                        {voutplayer[j]=i;
                        j+=1;
                        }
                  }
                players[voutplayer[rand()%voutsum]].state=0;
                for(int i=0;i<=voutsum;i++)
                {
                    if(players[voutplayer[i]].state==0)
                    {
                        player[voutplayer[i]+1]=0;
                        ui->duihua->append("本次公投淘汰的玩家是");
                        ui->duihua->append(QString::number(voutplayer[i]));
                        if(players[voutplayer[i]].status==1)
                        {
                            rednum-=1;
                            livesum-=1;
                            ui->duihua->append("她的身份是红色");
                            QMessageBox *box5=new QMessageBox;
                            box5->setText("有一位玩家被淘汰，身份为红色");
                            box5->exec();
                            end();
                        }
                        else
                        {
                            blacknum-=1;
                            livesum-=1;
                            ui->duihua->append("他的身份是黑色");
                            QMessageBox *box6=new QMessageBox;
                            box6->setText("有一位玩家被淘汰，身份为黑色");
                            box6->exec();
                            end();
                        }
                        for(int i=1;i<=n;i++)
                            players[i].points_voted=0;
                        break;
                    }
                }

            }
            sum=0;
            countsum=0;
            ui->voteoutbox->clear();
            playerinform();
            ui->gamewidget->setCurrentWidget(ui->afterjx);
            ui->voteoutbox->clear();
            ui->nightbtn->show();
            ui->nextbtn->hide();
        }
        end();
    });
}
//游戏函数包括别的函数
void MainWindow::game(){
  getNum();
  start();
  setplay();
  jxbtn();
  playerinform();
  MainWindow::nignt();
  voteoutfunc();
}


