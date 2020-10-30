#ifndef SNAKE_NORMAL_H
#define SNAKE_NORMAL_H

#include <QWidget>

namespace Ui {
class snake_normal;
}

class snake_normal : public QWidget
{
    Q_OBJECT

public:
    explicit snake_normal(QWidget *parent = nullptr);
    ~snake_normal();
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
    enum Move{Left,Right,Up,Down};
protected slots:
    void timeOut();
    void rewardTimeOut();
private:
    Ui::snake_normal *ui;
    QList<QRectF> snake;//贪吃蛇本体
    int snakeNodeWidth = 10;
    int snakeNodeHeight = 10;
    QTimer *timer2;
    QTimer *rewardTimer2;
    int time = 100;
    int moveFlage = Up;
    bool gameOver = false;
    bool gameStart = false;
    QList<QRectF> rewardNode;//奖励节点

};

#endif // SNAKE_NORMAL_H
