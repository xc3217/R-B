#ifndef GAMEPLAYER_H
#define GAMEPLAYER_H

#include <QWidget>
#include<QString>
struct Gameplayer{
    bool    cunzhang;//1表示是村长，0表示不是村长
    double  points_voted;//记录公投环节所获得的的票数
    double  point_have;//记录该玩家可以使用的票数
    double  points_votedcz;//记录投村长环节获得的票数
    bool    jxcun;//1表示是否要竞选担任村长
    bool    eye;//1表示睁眼
    int ID;//记录玩家序号
    bool state;//记录玩家的存活情况
    QString word;
    bool status;//1表示身份为红方，0表示身份为黑方
};


extern struct Gameplayer players[11];

class gameplayer : public QWidget
{
    Q_OBJECT
public:
    explicit gameplayer(QWidget *parent = nullptr);



signals:

};

#endif // GAMEPLAYER_H
