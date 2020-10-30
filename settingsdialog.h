#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "customedit.h"

namespace Ui {
class SettingsDialog;
}

class MainWindow;
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    virtual ~SettingsDialog();
    void initialize();
    void showCurrentFormat();
    void showCurrentBackground();
    void setEditor(customEdit*);
    void setMainWindow(MainWindow*);

private:
    enum {
        Keyword,
        Number,
        Operator,
        Quotation,
        PrecomplingInstructions,
        SingleLineComment,
        MultiLineComment
    }patterns;

    Ui::SettingsDialog *ui;
    QTextCharFormat s_keywordFormat;
    QTextCharFormat s_numberFormat;
    QTextCharFormat s_operatorFormat;
    QTextCharFormat s_quotationFormat;
    QTextCharFormat s_precomplingInstructionFormat;
    QTextCharFormat s_singleLineCommentFormat;
    QTextCharFormat s_multiLineCommentFormat;
    QColor s_background_color;
    QColor s_current_line_color;
    QColor s_selected_bg_color;
    QColor s_global_text_color;

    QTextCharFormat* p_currentFormat;
    QFont s_font;
    QString s_example;

    customEdit* editor;
    MainWindow* main_window;
};

#endif // SETTINGSDIALOG_H
