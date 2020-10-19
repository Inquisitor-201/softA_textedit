#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>

namespace Ui {
class replaceDialog;
}

class MainWindow;

class replaceDialog : public QDialog
{
    Q_OBJECT

public:
    replaceDialog(MainWindow *parent = nullptr);
    ~replaceDialog();
    QString findContent();
    QString targetText();
    bool checkedCapital();
    bool checkedRepeat();
    void changeIndex(int);

private:
    void setMainWindow(MainWindow* mw);
    Ui::replaceDialog *ui;
    MainWindow* main_window;

private slots:
    void closeEvent(QCloseEvent *) override;
};

#endif // REPLACEDIALOG_H
