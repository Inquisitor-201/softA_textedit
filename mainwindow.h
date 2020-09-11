#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QTextEdit>

//QT_BEGIN_NAMESPACE
//namespace Ui { class MainWindow; }
//QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createUi();
    void openFile();
//    Ui::MainWindow *ui;
    QTextEdit* textEdit;
    QAction* ac_openfile;
    QMenuBar* menuBar;
    QMenu* fileMenu;
    QMenu* editMenu;
};
#endif // MAINWINDOW_H
