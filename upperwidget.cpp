#include "upperwidget.h"
#include "customedit.h"
#include <QHBoxLayout>
#include <QFrame>
#include <QFileInfo>

UpperWidget::UpperWidget(QWidget *parent) : QFrame(parent)
{
    createLayout();
}

QComboBox* UpperWidget::comboBox()
{
    return combobox;
}

QPushButton* UpperWidget::buttonClose()
{
    return button_close;
}

void UpperWidget::resetBackground()
{
    auto cvt = [](QColor _rgb)->QString {
        QString redStr = QString("%1").arg(_rgb.red(),2,16,QChar('0'));
        QString greenStr = QString("%1").arg(_rgb.green(),2,16,QChar('0'));
        QString blueStr = QString("%1").arg(_rgb.blue(),2,16,QChar('0'));
        QString hexStr = "#" + redStr + greenStr+ blueStr;
        return hexStr;
    };
    combobox->setStyleSheet("background-color: " + cvt(background_color.lighter())
                            + "; color: " + cvt(global_text_color));
    button_close->setStyleSheet("background-color: " + cvt(background_color.lighter()));
}

void UpperWidget::createLayout()
{
    setStyleSheet("border: 1px solid #909090; background-color: #ffffff");
    QHBoxLayout* layout = new QHBoxLayout(this);
    combobox = new QComboBox(this);
    button_close = new QPushButton(this);
    button_close->setFixedSize(18, 18);

    combobox->setStyleSheet("background-color: #ffffff");
    button_close->setStyleSheet("background-color: #ffffff");
    button_close->setIcon(QIcon(":/resources/icons/closebutton.png"));

    setLayout(layout);
    layout->addWidget(combobox);
    layout->addWidget(button_close);
    layout->setContentsMargins(10, 2, 10, 2);
    setFixedHeight(30);
}

void UpperWidget::appendFile(QString filepath)
{
    QString filename = QFileInfo(filepath).fileName();
    combobox->insertItem(0, filename, filepath);
    combobox->setCurrentIndex(0);
}

UpperWidget::~UpperWidget()
{
}
