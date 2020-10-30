#include "snake_difficult.h"
#include "ui_snake_difficult.h"

#include <QKeyEvent>
#include <QRectF>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QMenu>
#include <QMenuBar>
#include <QWidget>
#include <QDebug>
snake_difficult::snake_difficult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::snake_difficult)
{
    ui->setupUi(this);
    setAttribute(Qt::WidgetAttribute::WA_SetStyle,true);
    setStyleSheet("normal{background:black}");//设置窗体背景色为黑色

    snake.append(QRectF(110,110,snakeNodeWidth,snakeNodeHeight));
    addTopRectF();
    addTopRectF();

    buildwall();

    rewardNode.append(QRectF(100,100,snakeNodeWidth,snakeNodeWidth));//首先生成一个奖励节点

    timer1 = new QTimer;
    connect(timer1, SIGNAL(timeout()),this,SLOT(timeOut()));
   //timer->start(time);
    rewardTimer1 = new QTimer;
    connect(rewardTimer1,SIGNAL(timeout()),this,SLOT(rewardTimeOut()));
    //rewardTimer->start(time*30);
    setFocusPolicy(Qt::StrongFocus);
    //focusOutEvent(QFocusEvent*event);
}

void snake_difficult::timeOut()
{
    int flage = 1;
    for(int i=0; i<rewardNode.length(); i++){
        if(rewardNode.at(i).contains(snake.at(0).topLeft()+QPointF(snakeNodeWidth/2,snakeNodeHeight/2))){
        //if(snake.at(0).contains(rewardNode.at(i).x()+rewardNode.at(i).width()/2,rewardNode.at(i).y()+rewardNode.at(i).height()/2)){
            if(rewardNode.at(i).width()>snakeNodeWidth){//额外奖励
                flage += 2;
            }
            flage++;//正常奖励
            rewardNode.removeAt(i);
            break;
        }
    }
    while(flage--){
        switch (moveFlage) {
        case Up:
            addTopRectF();
            break;
        case Down:
            addDownRectF();
            break;
        case Right:
            addRightRectF();
            break;
        case Left:
            addLeftRectF();
            break;
        default:
            break;
        }
    }
    deleteLastRectF();
    update();
}

//随机奖励
void snake_difficult::rewardTimeOut()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    if(rewardNode.length() > 5)
    {
        rewardNode.removeAt(qrand()%5);
    }
    rewardNode.append(QRectF(qrand()%(this->width()/20)*20,qrand()%(this->height()/20)*20,snakeNodeWidth,snakeNodeWidth));
    if(qrand()%5 == 3){
        rewardNode.append(QRectF(qrand()%(this->width()/20)*20-5,qrand()%(this->height()/20)*20-5,snakeNodeWidth*2,snakeNodeWidth*2));
    }
}

//向上移动
void snake_difficult::addTopRectF()
{
    if(snake.at(0).y()-snakeNodeHeight < 0)
    {
        snake.insert(0,QRectF(QPointF(snake.at(0).x(),this->height()-snakeNodeHeight),
                     QPointF(snake.at(0).x()+snakeNodeWidth,this->height())));
    }
    else
    {
        snake.insert(0,QRectF(snake.at(0).topLeft()+QPointF(0,-snakeNodeHeight),snake.at(0).topRight()));
    }
}

//向下移动
void snake_difficult::addDownRectF()
{
    if(snake.at(0).y()+snakeNodeHeight*2 > this->height())
    {
        snake.insert(0,QRectF(QPointF(snake.at(0).x(),snakeNodeHeight),
                     QPointF(snake.at(0).x()+snakeNodeWidth,0)));
    }
    else
    {
        snake.insert(0,QRectF(snake.at(0).bottomLeft(),snake.at(0).bottomRight()+QPointF(0,snakeNodeHeight)));
    }
}

//向左移动
void snake_difficult::addLeftRectF()
{
    if(snake.at(0).x()-snakeNodeWidth < 0)
    {
        snake.insert(0,QRectF(QPointF(this->width()-snakeNodeWidth,snake.at(0).y()),
                     QPointF(this->width(),snake.at(0).y()+snakeNodeHeight)));
    }
    else
    {
        snake.insert(0,QRectF(snake.at(0).topLeft()+QPointF(-snakeNodeWidth,0),snake.at(0).bottomLeft()));
    }
}

//向右移动
void snake_difficult::addRightRectF()
{
    if(snake.at(0).x()+snakeNodeWidth*2 > this->width()){
        snake.insert(0,QRectF(QPointF(0,snake.at(0).y()),
                     QPointF(snakeNodeWidth,snake.at(0).y()+snakeNodeHeight)));
    }else{
        snake.insert(0,QRectF(snake.at(0).topRight(),snake.at(0).bottomRight()+QPointF(snakeNodeWidth,0)));
    }
}

//删除结尾数据
void snake_difficult::deleteLastRectF()
{
    snake.removeLast();
}


//绘图
void snake_difficult::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen;
    QBrush brush;
    QFont font("方正舒体",12,QFont::ExtraLight,false);
    //反锯齿
    painter.setRenderHint(QPainter::Antialiasing);
    pen.setColor(Qt::black);
    brush.setColor(Qt::green);
    brush.setStyle(Qt::SolidPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    for(int i=0; i<snake.length(); i++)
    {
        painter.drawRect(snake.at(i));
    }
    brush.setColor(Qt::black);
    painter.setBrush(brush);
    for(int i = 0; i<4 ; i++)
    {
        for(int j=0;j<wall[i].length();j++)
        {
            painter.drawRect(wall[i].at(j));
        }
    }
    brush.setColor(Qt::red);
    painter.setBrush(brush);
    for(int i=0; i<rewardNode.length(); i++)
    {
        painter.drawEllipse(rewardNode.at(i));
    }
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(20,20,QString("当前得分：")+QString("%1").arg(snake.length()));
    if(snakeStrike())
    {
        QFont font("方正舒体",30,QFont::ExtraLight,false);
        painter.setFont(font);

        pen.setColor(Qt::black);
        painter.setPen(pen);
        painter.drawText((this->width()-300)/2,(this->height()-30)/2,QString("GAME OVER!"));
        timer1->stop();
        rewardTimer1->stop();
        gameOver = true;
    }
   QWidget::paintEvent(event);
}

void snake_difficult::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        if(moveFlage != Down)
        {
            moveFlage = Up;
        }
        break;
    case Qt::Key_Down:
        if(moveFlage != Up)
        {
            moveFlage = Down;
        }
        break;
    case Qt::Key_Right:
        if(moveFlage != Left)
        {
            moveFlage = Right;
        }
        break;
    case Qt::Key_Left:
        if(moveFlage != Right)
        {
            moveFlage = Left;
        }
        break;
    case Qt::Key_1:
    case Qt::Key_2:
        if(gameOver)
        {
            snake.clear();
            rewardNode.clear();
            moveFlage = Up;
            snake.append(QRectF(110,110,snakeNodeWidth,snakeNodeHeight));
            addTopRectF();
            addTopRectF();
            //首先生成一个奖励节点
            rewardNode.append(QRectF(100,100,snakeNodeWidth,snakeNodeWidth));
            timer1->start(time);
            rewardTimer1->start(time*30);
            gameOver = false;
        }
        break;
    case Qt::Key_0:
        if(gameStart && !gameOver)
        {
            timer1->start(time);
            rewardTimer1->start(time*30);
            gameStart = false;
        }
        else if(!gameStart && !gameOver)
        {
            timer1->stop();
            rewardTimer1->stop();
            gameStart = true;
        }
        break;
    default:
        break;
    }
   QWidget::keyPressEvent(event);
}

//判断蛇头是否和蛇身相撞
bool snake_difficult::snakeStrike()
{
     for(int i=0; i<snake.length();i++)
     {
          for(int j=0; j<wall[0].length(); j++)
          {
                qDebug() << "dsaa" << endl ;

                if(snake.at(i) == wall[0].at(j))
                {
                   // qDebug() << "dsaa" << endl ;
                    return true;
                }
           }
      }
     for(int i=0; i<snake.length();i++)
     {
          for(int j=0; j<wall[1].length(); j++)
          {
                qDebug() << "dsaa" << endl ;

                if(snake.at(i) == wall[1].at(j))
                {
                   // qDebug() << "dsaa" << endl ;
                    return true;
                }
           }
      }
     for(int i=0; i<snake.length();i++)
     {
          for(int j=0; j<wall[2].length(); j++)
          {
                qDebug() << "dsaa" << endl ;

                if(snake.at(i) == wall[2].at(j))
                {
                   // qDebug() << "dsaa" << endl ;
                    return true;
                }
           }
      }
     for(int i=0; i<snake.length();i++)
     {
          for(int j=0; j<wall[3].length(); j++)
          {
                qDebug() << "dsaa" << endl ;

                if(snake.at(i) == wall[3].at(j))
                {
                   // qDebug() << "dsaa" << endl ;
                    return true;
                }
           }
      }
    for(int i=0; i<snake.length(); i++)
    {
        for(int j=i+1; j<snake.length(); j++)
        {
            if(snake.at(i) == snake.at(j))
            {
                return true;
            }
        }
    }
    return false;
}

void snake_difficult::buildwall()
{
    for(int i = 0; i<5; i++)
    {
        wall[0].append(QRectF(130,130,snakeNodeWidth,snakeNodeWidth));
        addTopRectFwall(0);
    }
    for(int i = 0; i<5; i++)
    {
        wall[1].append(QRectF(200,200,snakeNodeWidth,snakeNodeWidth));
        addLeftRectFwall(1);
    }
    for(int i = 0; i<5; i++)
    {
        wall[2].append(QRectF(350,350,snakeNodeWidth,snakeNodeWidth));
        addLeftRectFwall(2);
        addLeftRectFwall(2);
        addTopRectFwall(2);
        addTopRectFwall(2);
    }
    for(int i = 0; i<5; i++)
    {
        wall[3].append(QRectF(250,110,snakeNodeWidth,snakeNodeWidth));
         addTopRectFwall(3);
    }
    for(int i = 0; i<5; i++)
    {
       addLeftRectFwall(3);
    }

}
void snake_difficult::addTopRectFwall(int i)
{
    if(wall[i].at(0).y()-snakeNodeHeight < 0)
    {
        wall[i].insert(0,QRectF(QPointF(wall[i].at(0).x(),this->height()-snakeNodeHeight),
                     QPointF(wall[i].at(0).x()+snakeNodeWidth,this->height())));
    }
    else
    {
        wall[i].insert(0,QRectF(wall[i].at(0).topLeft()+QPointF(0,-snakeNodeHeight),wall[i].at(0).topRight()));
    }
}
void snake_difficult::addLeftRectFwall(int i)
{
    if(wall[i].at(0).x()-snakeNodeWidth < 0)
    {
        wall[i].insert(0,QRectF(QPointF(this->width()-snakeNodeWidth,snake.at(0).y()),
                     QPointF(this->width(),wall[i].at(0).y()+snakeNodeHeight)));
    }
    else
    {
        wall[i].insert(0,QRectF(wall[i].at(0).topLeft()+QPointF(-snakeNodeWidth,0),wall[i].at(0).bottomLeft()));
    }
}
snake_difficult::~snake_difficult()
{
    delete ui;
}
