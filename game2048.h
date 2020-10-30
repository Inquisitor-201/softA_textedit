#ifndef GAME2048_H
#define GAME2048_H

#include <QDialog>
#include <QPushButton>

class GAME2048 : public QDialog
{
    Q_OBJECT
public:
    explicit GAME2048(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);

    bool PressUp();//向上更新
    bool PressDown();//向下更新
    bool PressLeft();//向左更新
    bool PressRight();//向右更新
    void myRand();//随机数决定2，4出现的位置。
    QPushButton *button;
    int s[4][4];//二位数组存储每个格子数据
    int score;
    bool state;

    struct Ns{  //保存空格
        int i;
        int j;
    };


signals:

public slots:
    void slotStart();//槽函数：游戏开始
};

#endif // GAME2048_H
