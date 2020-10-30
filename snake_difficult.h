#ifndef SNAKE_DIFFICULT_H
#define SNAKE_DIFFICULT_H

#include <QWidget>
namespace Ui {
class snake_difficult;
}
class snake_difficult : public QWidget
{
    Q_OBJECT

public:
    explicit snake_difficult(QWidget *parent = nullptr);
    ~snake_difficult();
protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
private:
    void addTopRectF();
    void addDownRectF();
    void addLeftRectF();
    void addRightRectF();
    void deleteLastRectF();
    bool snakeStrike();
    void buildwall();
    void addTopRectFwall(int i);
    void addLeftRectFwall(int i);
    enum Move{Left,Right,Up,Down};
protected slots:
    void timeOut();
    void rewardTimeOut();
private:
    Ui::snake_difficult *ui;
    QList<QRectF> snake;//贪吃蛇本体
    int snakeNodeWidth = 10;
    int snakeNodeHeight = 10;
    QTimer *timer1;
    QTimer *rewardTimer1;
    int time = 100;
    int moveFlage = Up;
    bool gameOver = false;
    bool gameStart = false;
    QList<QRectF> rewardNode;//奖励节点
    QList<QRectF> wall[4];
};
namespace Ui {
class snake_difficult;
}
#endif // SNAKE_DIFFICULT_H
