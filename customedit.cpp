#include "customedit.h"
#include "replacedialog.h"
#include "mainwindow.h"
#include <QDebug>
#include <QListWidget>
#include <QStringListModel>
#include <QHBoxLayout>
#include <QApplication>

QFont* globalFont = new QFont();
QColor background_color;
QColor selected_bg_color;
QColor current_line_color;
QColor global_text_color;

customEdit::customEdit(QWidget* parent): QPlainTextEdit(parent)
{
    globalFont->setFamily("Consolas");
    globalFont->setPixelSize(18);
    global_text_color = QColor(0, 0, 0);
    current_line_color = QColor(204, 255, 255);
    background_color = QColor(255, 255, 255);
    selected_bg_color = QColor(0, 120, 215);

    resetAppearance();

    setLineWrapMode(QPlainTextEdit::NoWrap);
    setTabStopDistance(40);

    customHighlighter = new Highlighter(document());
    connect(this, &QPlainTextEdit::cursorPositionChanged,
            this, &customEdit::highlightCurrentLine);
    connect(this, &QPlainTextEdit::cursorPositionChanged, [=](){
                setFont(*globalFont);
                QTextCharFormat fmt;
                fmt.setForeground(global_text_color);
                setCurrentCharFormat(fmt);
    });
    customCompleter = new Completer(this);

    wordList = new QListWidget(this);
    wordList->setVisible(false);        //代码自动补全的单词表
}

void customEdit::resetAppearance()
{
    setFont(*globalFont);
    QTextCharFormat fmt;
    fmt.setForeground(global_text_color);

    QTextCursor cs = textCursor();
    cs.select(QTextCursor::Document);
    cs.setCharFormat(fmt);
    setCurrentCharFormat(fmt);

    //设置背景颜色和selected颜色
    auto invert_color = [](QColor c) -> QColor {
        int ir = 255 - c.red();
        int ig = 255 - c.green();
        int ib = 255 - c.blue();
        return QColor(ir, ig, ib);
    };
    QPalette p = palette();
    p.setColor(QPalette::Active, QPalette::Base, background_color);
    p.setColor(QPalette::Inactive, QPalette::Base, background_color);
    p.setColor(QPalette::Inactive, QPalette::Highlight, selected_bg_color);
    p.setColor(QPalette::Inactive, QPalette::HighlightedText, invert_color(global_text_color));
    p.setColor(QPalette::Inactive, QPalette::Highlight, selected_bg_color);
    p.setColor(QPalette::Inactive, QPalette::HighlightedText, invert_color(global_text_color));
    setPalette(p);
}

void customEdit::setMainWindow(MainWindow* mw)
{
    main_window = mw;
}

Highlighter* customEdit::highlighter()
{
    return customHighlighter;
}

MainWindow* customEdit::getMainWindow()
{
    return main_window;
}

void customEdit::keyPressEvent(QKeyEvent *e) // 代码补全
{
    QPlainTextEdit::keyPressEvent(e);
    QTextCursor tc = textCursor();

    switch (e->key()) {
        case Qt::Key_BraceLeft:
            tc.insertText("}");
            tc.setPosition(tc.position() - 1);
            setTextCursor(tc);
            break;
        case Qt::Key_BracketLeft:
            tc.insertText("]");
            tc.setPosition(tc.position() - 1);
            setTextCursor(tc);
            break;
        case Qt::Key_ParenLeft:
            tc.insertText(")");
            tc.setPosition(tc.position() - 1);
            setTextCursor(tc);
            break;
    }
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    QString prefix = cursor.selectedText();
    QStringList stringList = customCompleter->conformingWords(prefix);

    QRect rect = cursorRect();
    int X = rect.x(), Y = rect.y();

    wordList->clear();
    wordList->setVisible(!stringList.isEmpty());
    wordList->setGeometry(X, Y + globalFont->pixelSize() + 4, 200, std::min(20 * stringList.length(), 160) + 2);
    wordList->addItems(stringList);
}

void customEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        selection.format.setBackground(current_line_color);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void customEdit::wheelEvent(QWheelEvent *e)    // 滚轮事件
{
    //qDebug() << "wheel";
    if (QApplication::keyboardModifiers () == Qt::ControlModifier)
    {
        if (e->delta() > 0){
            main_window->zoomin();
        } else {
            main_window->zoomout();
        }
    } else {
        QPlainTextEdit::wheelEvent(e);
    }
}

customEdit::~customEdit()
{
}

