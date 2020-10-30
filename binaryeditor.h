#ifndef BINARYEDITOR_H
#define BINARYEDITOR_H

#include <QAbstractScrollArea>
#include <QTimer>
#include "customedit.h"

class BinaryEditor : public QAbstractScrollArea
{
    Q_OBJECT
public:
    explicit BinaryEditor(QWidget* parent);
    void setRelatedTextEdit(customEdit* editor);
    virtual ~BinaryEditor();
    void setTextString(QString s);
    //void resetFont();

protected:
    void paintEvent(QPaintEvent* e) override;
    void wheelEvent(QWheelEvent *e) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
private:
    customEdit* textEdit;

    QTimer* timer;
    QString textString;
    int m_margin;
    int m_char;
    int m_height;
    int side_width;
    int num_lines;
    int shown_lines;
    int tot_width;
    int cursor_r;
    int cursor_c;
    bool blink;                     //选中区域闪烁
    static QChar toHex(int x);
    static QString getAddress(int x);
};

#endif // BINARYEDITOR_H
