#ifndef UPPERWIDGET_H
#define UPPERWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>

class UpperWidget : public QFrame
{
    Q_OBJECT

public:
    explicit UpperWidget(QWidget *parent = nullptr);
    virtual ~UpperWidget();
    void appendFile(QString filepath);
    void resetBackground();
    QComboBox* comboBox();
    QPushButton* buttonClose();

private:
    void createLayout();
    QComboBox* combobox;
    QPushButton* button_close;
};

#endif // UPPERWIDGET_H
