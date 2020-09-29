#include "customedit.h"
#include <QDebug>
#include <QListWidget>
#include <QStringListModel>
#include <QHBoxLayout>

customEdit::customEdit(QWidget* parent): QPlainTextEdit(parent)
{
    globalFont.setFamily("Consolas");
    globalFont.setPixelSize(18);
    setFont(globalFont);

    setTabStopDistance(40);

    customHighlighter = new Highlighter(document());
    connect(this, &QPlainTextEdit::cursorPositionChanged,
            this, &customEdit::highlightCurrentLine);

    customCompleter = new Completer();

    wordList = new QListWidget(this);
    wordList->setVisible(false);
}

void customEdit::keyPressEvent(QKeyEvent *e)
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
    wordList->setGeometry(X, Y + globalFont.pixelSize() + 4, 200, std::min(20 * stringList.length(), 160) + 2);
    wordList->addItems(stringList);
}

void customEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(204, 255, 255);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

customEdit::~customEdit()
{
}

