#ifndef CUSTOMEDIT_H
#define CUSTOMEDIT_H

#include <QPlainTextEdit>
#include <QListWidget>
#include "highlighter.h"
#include "completer.h"

class replaceDialog;

class customEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    customEdit(QWidget* parent = nullptr);
    ~customEdit();

protected:
    void keyPressEvent(QKeyEvent *e) override;

private slots:
    void highlightCurrentLine();

private:
    Highlighter* customHighlighter;
    QListWidget* wordList;
    Completer* customCompleter;
    replaceDialog* replace_dialog;
    QFont globalFont;
};

#endif // CUSTOMEDIT_H
