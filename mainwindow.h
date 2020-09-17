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
    QString current_filename;

    void createUi();
    bool newFile();
    bool openFile();
    bool saveFile();
    bool saveAs();
    bool saveAsFile(QString&);
    bool getSaveFileName(QString*);

//    Ui::MainWindow *ui;
    QTextEdit* textEdit;

    QAction* ac_newfile;
    QAction* ac_openfile;
    QAction* ac_savefile;
    QAction* ac_save_as;

    QMenuBar* menuBar;
    QMenu* fileMenu;
    QMenu* editMenu;
};
#endif // MAINWINDOW_H
