#include "binaryeditor.h"

#include <QPainter>
#include <QDebug>
#include <QScrollBar>
#include <QFontMetrics>
#include <QApplication>
#include "mainwindow.h"

BinaryEditor::BinaryEditor(QWidget* parent)
    : QAbstractScrollArea(parent)
{
    textString = QString();
    cursor_r = 0;
    cursor_c = 0;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=](){
        blink = !blink;
        viewport()->update();
    });
    timer->start(500);
}

void BinaryEditor::setTextString(QString s)
{
    cursor_c = cursor_r = 0;
    textString = s;
    viewport()->update();
}

void BinaryEditor::setRelatedTextEdit(customEdit* editor)
{
    textEdit = editor;
}

void BinaryEditor::paintEvent(QPaintEvent* e)
{
    //qDebug() << "paint";
    QFontMetrics ft(*globalFont);
    m_char = ft.horizontalAdvance("M");
    m_margin = m_char;
    m_height = ft.height();
    side_width = m_margin + 8 * m_char;
    tot_width = side_width + 65 * m_char;

    QPainter painter(viewport());
    painter.setFont(*globalFont);

    int viewport_height = viewport()->height();
    int viewport_width = viewport()->width();
    int text_length = textString.size();
    num_lines = (text_length + 15) / 16;            //总共多少行
    shown_lines = viewport_height / m_height;       //viewport总共要print多少行

//![setScrollBar]
    horizontalScrollBar()->setPageStep(viewport_width / 2);
    if (num_lines >= shown_lines) {
        verticalScrollBar()->setRange(0, num_lines - shown_lines + 1);
        verticalScrollBar()->show();
    } else {
        verticalScrollBar()->setValue(0);
        verticalScrollBar()->hide();
    }
    if (tot_width >= viewport_width) {
        horizontalScrollBar()->setRange(0, tot_width - viewport_width);
        horizontalScrollBar()->show();
    } else {
        horizontalScrollBar()->setValue(0);
        horizontalScrollBar()->hide();
    }
//![setScrollBar]

//![paint_header]
    int y_offset = verticalScrollBar()->value();
    int x_offset = horizontalScrollBar()->value();

    painter.drawText(QRect(- x_offset + m_margin, 0, 8 * m_char, m_height), "Address"); //绘制"Address"
    painter.drawLine(- x_offset + side_width, 0, - x_offset + side_width, m_height);
    for (int i = 0; i < 16; i++)
    {
        int x_end = - x_offset + side_width + 3 * (i + 1) * m_char;
        painter.drawLine(x_end, 0, x_end, m_height);
        QRect r(- x_offset + side_width + 3 * i * m_char + m_margin, 0, m_char, m_height);
        painter.drawText(r, toHex(i));
    }                                               //绘制Title中的0到f
    painter.drawText(QRect(- x_offset + side_width + 48 * m_char + m_margin / 2, 0, 4 * m_char, m_height), "Dump");
//![paint_header]

//![paint_hexContent]
    for (int i = 0; i < shown_lines; i++)
    {
        if ((i + y_offset) * 16 >= text_length) break;
        QRect r(- x_offset + m_margin / 2, (i + 1) * m_height, 8 * m_char, m_height);
        painter.drawText(r, getAddress(i + y_offset));         //绘制每行开头的Address
        for (int j = 0; j < 16; j++)
        {
            if ((i + y_offset) * 16 + j >= text_length) break;
            int c = textString[(i+y_offset)*16+j].toLatin1();
            QRect r1(- x_offset + side_width + 3 * j * m_char + m_margin / 2, (i + 1) * m_height, 2 * m_char, m_height);
            QRect r2(- x_offset + side_width + 48 * m_char + m_margin / 2 + j * m_char, (i + 1) * m_height, m_char, m_height);

            if (i + y_offset == cursor_r && j == cursor_c) {
                painter.setPen(QColor(255, 0, 0));
                painter.drawRect(- x_offset + side_width + 3 * j * m_char + m_margin / 2 - 1, (i + 1) * m_height - 1, 2 * m_char + 1, m_height + 1);
                painter.drawRect(- x_offset + side_width + 48 * m_char + m_margin / 2 + j * m_char - 1, (i + 1) * m_height - 1, m_char + 1, m_height + 1);
                painter.setPen(QColor(0, 0, 0));
            }  //绘制选中部分边框
            if (i + y_offset == cursor_r && j == cursor_c && blink)
            {
                painter.fillRect(r1, QBrush(QColor(136, 136, 255)));
                painter.fillRect(r2, QBrush(QColor(136, 136, 255)));
                painter.setPen(QColor(255, 255, 255));
                painter.drawText(r1, QString(toHex(c >> 4)) + toHex(c % 16));
                painter.drawText(r2, QChar(c).isPrint() ? QChar(c) : '.');
                painter.setPen(QColor(0, 0, 0));                                    //绘制选中部分
            } else {
                painter.drawText(r1, QString(toHex(c >> 4)) + toHex(c % 16));       //绘制每行的十六进制表
                if (QChar(c).isPrint())
                    painter.drawText(r2, QChar(c));
                else {
                    painter.setPen(QColor(255, 0, 0));
                    painter.drawText(r2, QChar('.'));
                    painter.setPen(QColor(0, 0, 0));
                }
            }
        }
    }

    //qDebug() << "shown_lines = " << shown_lines;
//![paint_hexContent]

    QAbstractScrollArea::paintEvent(e);
}

QChar BinaryEditor::toHex(int x)
{
    return x < 10 ? QChar(x + 48) : QChar(x - 10 + 'a');
}

QString BinaryEditor::getAddress(int x)
{
	QString str("0");
    for (int i = 1; i <= 7; i++)
        str = toHex(x % 16) + str,
        x >>= 4;
    return str;
}

void BinaryEditor::keyPressEvent(QKeyEvent *e) // 代码补全
{
    int x_offset = horizontalScrollBar()->value();
    int y_offset = verticalScrollBar()->value();
    switch (e->key()) {
        case Qt::Key_Up:
            if (cursor_r) {
                cursor_r--;
                if (y_offset > cursor_r)
                    verticalScrollBar()->setValue(cursor_r);
                else if (y_offset + shown_lines < cursor_r + 2)
                    verticalScrollBar()->setValue(cursor_r - shown_lines + 2);
                viewport()->update();
            }
            break;
        case Qt::Key_Down:
            if ((cursor_r + 1) * 16 + cursor_c < textString.length()) {
                cursor_r++;
                if (y_offset > cursor_r)
                    verticalScrollBar()->setValue(cursor_r);
                else if (y_offset + shown_lines < cursor_r + 2)
                    verticalScrollBar()->setValue(cursor_r - shown_lines + 2);
                viewport()->update();
            }
            break;
        case Qt::Key_Left:
            if (cursor_c) {
                cursor_c--;
                if (x_offset + viewport()->width() < (cursor_c + 1) * 3 * m_char + side_width)
                    horizontalScrollBar()->setValue((cursor_c + 1) * 3 * m_char + side_width - viewport()->width());
                else if (x_offset > cursor_c * 3 * m_char + side_width)
                    horizontalScrollBar()->setValue(cursor_c * 3 * m_char + side_width);
                viewport()->update();
            }
            break;
        case Qt::Key_Right:
            if (cursor_c < 15) {
                cursor_c++;
                if (x_offset + viewport()->width() < (cursor_c + 1) * 3 * m_char + side_width)
                    horizontalScrollBar()->setValue((cursor_c + 1) * 3 * m_char + side_width - viewport()->width());
                else if (x_offset > cursor_c * 3 * m_char + side_width)
                    horizontalScrollBar()->setValue(cursor_c * 3 * m_char + side_width);
                viewport()->update();
            }
            break;
        default:
            QAbstractScrollArea::keyPressEvent(e);
    }
}

void BinaryEditor::wheelEvent(QWheelEvent *e)
{
    //qDebug() << "wheel2";
    if (QApplication::keyboardModifiers () == Qt::ControlModifier)
    {
        MainWindow* main_window = textEdit->getMainWindow();
        if (e->delta() > 0){
            main_window->zoomin();
        } else {
            main_window->zoomout();
        }
    } else {
        QAbstractScrollArea::wheelEvent(e);
    }
}


BinaryEditor::~BinaryEditor()
{
}
