#ifndef CUSTOMEDIT_H
#define CUSTOMEDIT_H

#include <QPlainTextEdit>
#include <QListWidget>
#include "highlighter.h"
#include "completer.h"

class replaceDialog;
class MainWindow;

class customEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    customEdit(QWidget* parent = nullptr);
    ~customEdit();
    void setMainWindow(MainWindow* mw);
    MainWindow* getMainWindow();

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;

private slots:
    void highlightCurrentLine();

private:
    Highlighter* customHighlighter;
    QListWidget* wordList;
    Completer* customCompleter;
    replaceDialog* replace_dialog;
    MainWindow* main_window;
};

extern QFont* globalFont;

#endif // CUSTOMEDIT_H
