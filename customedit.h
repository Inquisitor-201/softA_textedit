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
    MainWindow* getMainWindow();
    Highlighter* highlighter();
    void resetAppearance();
    void setMainWindow(MainWindow* mw);

public slots:
    void highlightCurrentLine();

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;

private:
    Highlighter* customHighlighter;
    QListWidget* wordList;
    Completer* customCompleter;
    replaceDialog* replace_dialog;
    MainWindow* main_window;
};

extern QFont* globalFont;
extern QColor background_color;
extern QColor selected_bg_color;
extern QColor current_line_color;
extern QColor global_text_color;
#endif // CUSTOMEDIT_H
