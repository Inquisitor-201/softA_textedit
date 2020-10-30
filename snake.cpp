
#include "snake.h"
#include "ui_snake.h"
#include "snake_normal.h"
#include "snake_difficult.h"

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
int firstIndex=0;
snake::snake(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::snake)
{
    ui->setupUi(this);
    setWindowTitle(QString("贪吃蛇"));
    resize(620,550);
    setStyleSheet("QWidget{background:white}");//设置窗体背景色
    setWindowOpacity(0.9);//设置窗口的透明度

    snake_difficult *snake_difficultWidget = new snake_difficult(this);
    snake_normal *snake_normalWidget = new snake_normal(this);


    ui->stackedWidget->addWidget(snake_normalWidget);
    ui->stackedWidget->addWidget(snake_difficultWidget);
    ui->stackedWidget->setCurrentWidget(snake_normalWidget);
    firstIndex = ui->stackedWidget->currentIndex();

    qDebug()<<"firstIndex:"<<firstIndex;
    ui->pushButton->setStyleSheet("color:black");
    connect(ui->pushButton, &QPushButton::clicked, [=]{
        switchPage();
    });

}
void snake::switchPage()
{
    int nCount =  ui->stackedWidget->count();
    int nIndex = ui->stackedWidget->currentIndex();
    ++nIndex;
    if(nIndex>=nCount)
        nIndex = firstIndex;
    qDebug() << "switch to " << nIndex;
    ui->stackedWidget->setCurrentIndex(nIndex);

}
 snake::~snake()
{
    delete ui;
}

