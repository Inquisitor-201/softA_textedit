#include "game2048.h"
#include <QTime>
#include <QMessageBox>
#include <QPainter>
#include <QDebug>
#include <QKeyEvent>

GAME2048::GAME2048(QWidget *parent) :
      QDialog(parent),score(0),state(false)
{
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            s[i][j]=0;
    button = new QPushButton("begin",this);
    button->setGeometry(60,400,200,50);
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));//随机函数
    connect(button,SIGNAL(clicked()),this,SLOT(slotStart()));

    //connect(timer,SIGNAL(timeout()),this,SLOT(update()));
}

void GAME2048::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setBrush(Qt::blue);
    p.setFont(QFont("微软雅黑",20,700,false));

    QString strscore;
    p.drawText(QPoint(20,60),"score:  "+QString::number(score));

    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++){
            p.setPen(Qt::transparent);
            if(s[i][j] == 0){
                p.setBrush(Qt::gray);
                p.drawRect(i*60+40,j*60+120,55,55);
            }
            else if(s[i][j] == 2){
                p.setBrush(Qt::red);
                p.drawRect(i*60+40,j*60+120,55,55);
                p.setPen(Qt::black);
                p.setFont(QFont("微软雅黑",10,700,false));
               p.drawText(QRectF(i*60+40,j*60+120,55,55),QString::number(2),QTextOption(Qt::AlignCenter));
            }
            else if(s[i][j] == 4){
                p.setBrush(Qt::darkRed);
                p.drawRect(i*60+40,j*60+120,55,55);
                p.setPen(Qt::black);
                p.setFont(QFont("微软雅黑",10,700,false));
               p.drawText(QRectF(i*60+40,j*60+120,55,55),QString::number(4),QTextOption(Qt::AlignCenter));            }
            else if(s[i][j] == 8){
                p.setBrush(Qt::green);
                p.drawRect(i*60+40,j*60+120,55,55);
                p.setPen(Qt::black);
                p.setFont(QFont("微软雅黑",10,700,false));
               p.drawText(QRectF(i*60+40,j*60+120,55,55),QString::number(8),QTextOption(Qt::AlignCenter));            }
            else if(s[i][j] == 16){
                p.setBrush(Qt::darkGreen);
                p.drawRect(i*60+40,j*60+120,55,55);
                p.setPen(Qt::black);
                p.setFont(QFont("微软雅黑",10,700,false));
               p.drawText(QRectF(i*60+40,j*60+120,55,55),QString::number(16),QTextOption(Qt::AlignCenter));            }
            else if(s[i][j] == 32){
                p.setBrush(Qt::yellow);
                p.drawRect(i*60+40,j*60+120,55,55);
                p.setPen(Qt::black);
                p.setFont(QFont("微软雅黑",10,700,false));
               p.drawText(QRectF(i*60+40,j*60+120,55,55),QString::number(32),QTextOption(Qt::AlignCenter));            }
            else if(s[i][j] == 64){
                p.setBrush(Qt::darkYellow);
                p.drawRect(i*60+40,j*60+120,55,55);
                p.setPen(Qt::black);
                p.setFont(QFont("微软雅黑",10,700,false));
               p.drawText(QRectF(i*60+40,j*60+120,55,55),QString::number(64),QTextOption(Qt::AlignCenter));            }
            else if(s[i][j] == 128){
                p.setBrush(Qt::cyan);
                p.drawRect(i*60+40,j*60+120,55,55);
                p.setPen(Qt::black);
                p.setFont(QFont("微软雅黑",10,700,false));
               p.drawText(QRectF(i*60+40,j*60+120,55,55),QString::number(128),QTextOption(Qt::AlignCenter));            }
            else if(s[i][j] == 256){
                p.setBrush(Qt::darkCyan);
                p.drawRect(i*60+40,j*60+120,55,55);
                p.setPen(Qt::black);
                p.setFont(QFont("微软雅黑",10,700,false));
               p.drawText(QRectF(i*60+40,j*60+120,55,55),QString::number(256),QTextOption(Qt::AlignCenter));            }
            else if(s[i][j] == 512){
                p.setBrush(Qt::magenta);
                p.drawRect(i*60+40,j*60+120,55,55);
                p.setPen(Qt::black);
                p.setFont(QFont("微软雅黑",10,700,false));
               p.drawText(QRectF(i*60+40,j*60+120,55,55),QString::number(512),QTextOption(Qt::AlignCenter));            }
            else if(s[i][j] == 1024){
                p.setBrush(Qt::darkMagenta);
                p.drawRect(i*60+40,j*60+120,55,55);
                p.setPen(Qt::black);
                p.setFont(QFont("微软雅黑",10,700,false));
               p.drawText(QRectF(i*60+40,j*60+120,55,55),QString::number(1024),QTextOption(Qt::AlignCenter));            }
            else if(s[i][j] == 2048){
                p.setBrush(Qt::blue);
                p.drawRect(i*60+40,j*60+120,55,55);
                p.setPen(Qt::black);
                p.setFont(QFont("微软雅黑",10,700,false));
               p.drawText(QRectF(i*60+40,j*60+120,55,55),QString::number(2048),QTextOption(Qt::AlignCenter));            }
            else{
                p.setBrush(Qt::darkBlue);
                p.drawRect(i*60+40,j*60+120,55,55);
                p.setPen(Qt::black);
                p.setFont(QFont("微软雅黑",10,700,false));
               p.drawText(QRectF(i*60+40,j*60+120,55,55),QString::number(s[i][j]),QTextOption(Qt::AlignCenter));
            }

        }
}

void GAME2048::keyPressEvent(QKeyEvent *event)
{
    if(!state)
        return;

    int ret;
   switch(event->key())
   {
    case Qt::Key_W:
         ret = PressUp();
         break;
    case Qt::Key_S:
        ret = PressDown();
         break;
    case Qt::Key_A:
       ret = PressLeft();
         break;
    case Qt::Key_D:
       ret = PressRight();
         break;
    default:return;//忽略其他按键
   }
    if (ret) {
       myRand();
       int lose = 1;
       for(int i=0;i<4;i++)
           for(int j=0;j<3;j++)
               if(s[i][j] == s[i][j+1] || !s[i][j] || !s[i][j+1])
                  lose = 0;
       for(int j=0;j<4;j++)
            for(int i=0;i<3;i++)
                if(s[i][j] == s[i+1][j] || !s[i][j] || !s[i+1][j])
                   lose = 0;
      if (lose) {
        QMessageBox::about(this,"GG","your score:"+QString::number(score)+"       ");
        return;
      }
       update();
    }
}

void GAME2048::slotStart()
{
   QMessageBox::about(this,"游戏规则","亲！~用W,S,A,D分别控制方块上下左右移动，知道了吗？");
    score = 0;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            s[i][j]=0;
    button->setText("重新游戏");

    int randi = qrand() % 4;
    int randj = qrand() % 4;
    s[randi][randj] = 2;

    state = true;
    update();

}

bool GAME2048::PressLeft()
{
    bool ret = 0;
    for (int rep = 0; rep < 3; rep++)
        for (int c = 0; c < 4; c++)
            for (int r = 1; r < 4; r++)
                if (s[r][c] > 0 && s[r-1][c] == 0) {
                    s[r-1][c] = s[r][c];
                    s[r][c] = 0;
                    ret = 1;
                }

    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 4; c++)
            if (s[r+1][c] == s[r][c] && s[r][c]) {
                s[r][c] *= 2;
                score += s[r][c];
                s[r+1][c] = 0;
                ret = 1;
            }

    for (int rep = 0; rep < 3; rep++)
        for (int c = 0; c < 4; c++)
            for (int r = 1; r < 4; r++)
                if (s[r][c] > 0 && s[r-1][c] == 0) {
                    s[r-1][c] = s[r][c];
                    s[r][c] = 0;
                    ret = 1;
                }
    return ret;
}

bool GAME2048::PressRight()
{
    int ret = 0;
    for (int rep = 0; rep < 3; rep++)
        for (int c = 0; c < 4; c++)
            for (int r = 2; r >= 0; r--)
                if (s[r][c] > 0 && s[r+1][c] == 0) {
                    s[r+1][c] = s[r][c];
                    s[r][c] = 0;
                    ret = 1;
                }

    for (int r = 3; r > 0; r--)
        for (int c = 0; c < 4; c++)
            if (s[r-1][c] == s[r][c] && s[r][c]) {
                s[r][c] *= 2;
                score += s[r][c];
                s[r-1][c] = 0;
                ret = 1;
            }

    for (int rep = 0; rep < 3; rep++)
        for (int c = 0; c < 4; c++)
            for (int r = 2; r >= 0; r--)
                if (s[r][c] > 0 && s[r+1][c] == 0) {
                    s[r+1][c] = s[r][c];
                    s[r][c] = 0;
                    ret = 1;
                }
    return ret;
}

bool GAME2048::PressUp()
{
    int ret = 0;
    for (int rep = 0; rep < 3; rep++)
        for (int c = 1; c < 4; c++)
            for (int r = 3; r >= 0; r--)
                if (s[r][c] > 0 && s[r][c-1] == 0) {
                    s[r][c-1] = s[r][c];
                    s[r][c] = 0;
                    ret = 1;
                }

    for (int c = 0; c < 3; c++)
        for (int r = 0; r < 4; r++)
            if (s[r][c+1] == s[r][c] && s[r][c]) {
                s[r][c] *= 2;
                score += s[r][c];
                s[r][c+1] = 0;
                ret = 1;
            }

    for (int rep = 0; rep < 3; rep++)
        for (int c = 1; c < 4; c++)
            for (int r = 3; r >= 0; r--)
                if (s[r][c] > 0 && s[r][c-1] == 0) {
                    s[r][c-1] = s[r][c];
                    s[r][c] = 0;
                    ret = 1;
                }
    return ret;
}

bool GAME2048::PressDown()
{
    int ret = 0;
    for (int rep = 0; rep < 3; rep++)
        for (int c = 2; c >= 0; c--)
            for (int r = 0; r < 4; r++)
                if (s[r][c] > 0 && s[r][c+1] == 0) {
                    s[r][c+1] = s[r][c];
                    s[r][c] = 0;
                    ret = 1;
                }

    for (int r = 0; r < 4; r++)
        for (int c = 3; c > 1; c--)
            if (s[r][c-1] == s[r][c] && s[r][c]) {
                s[r][c] *= 2;
                score += s[r][c];
                s[r][c-1] = 0;
                ret = 1;
            }

    for (int rep = 0; rep < 3; rep++)
        for (int c = 2; c >= 0; c--)
            for (int r = 0; r < 4; r++)
                if (s[r][c] > 0 && s[r][c+1] == 0) {
                    s[r][c+1] = s[r][c];
                    s[r][c] = 0;
                    ret = 1;
                }
    return ret;
}

void GAME2048::myRand()
{
    int i,j;
    //找出空的格子
   struct Ns n[15];
   int ni=0;
   for(i=0;i<4;i++)
       for(j=0;j<4;j++){
           if(s[i][j] == 0)
           {
               n[ni].i=i;
               n[ni].j=j;
               ++ni;
           }
       }

   //判断游戏是否结束
   if(ni == 0){
       for(i=0;i<4;i++)
           for(j=0;j<3;j++)
               if(s[i][j] == s[i][j+1])
                  return;
       for(j=0;j<4;j++)
            for(i=0;i<3;i++)
                if(s[i][j] == s[i+1][j])
                   return;
      QMessageBox::about(this,"GG","your score:"+QString::number(score)+"       ");
      return;
   }

   int rand = qrand() % (ni);
   s[n[rand].i][n[rand].j] = 2;

}
