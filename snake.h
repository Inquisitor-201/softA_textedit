#ifndef SNAKE_H
#define SNAKE_H

#include <QDialog>
#include <QStackedWidget>
namespace Ui {
class snake;
}

class snake : public QDialog
{
    Q_OBJECT
public:
    explicit snake(QWidget *parent = 0);
    ~snake();
private:
    Ui::snake *ui;
    void switchPage();
    QStackedWidget  m_pStackedWidget ;
    int firstIndex;
};
#endif // SNAKE_H
