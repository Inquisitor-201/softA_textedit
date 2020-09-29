#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QTextEdit>
#include "highlighter.h"
#include "customedit.h"
#include "replacedialog.h"

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
    void setupTextEdit();
    void setupDialogs();

    bool newFile();
    bool openFile();
    bool saveFile();                   //保存文件（若已存在文件名，则直接保存，否则调用getSaveFileName)
    bool saveAs();                     //另存为
    bool QuitProgram();                //关闭程序

    void Replace();

    bool saveAsFile(QString&);         //给定一个QString filename，将文本框的内容保存到filename的文件中
    bool getSaveFileName(QString*);    //展示一个QFileDialog, 将需要保存的文件名存放到给定地址中
    bool querySave();                  //询问是否要保存，返回为1为已决定“是否要保存”，返回0为选择了“cancel”

//    Ui::MainWindow *ui;
    customEdit* textEdit;

//! [file]
    QAction* ac_newfile;
    QAction* ac_openfile;
    QAction* ac_savefile;
    QAction* ac_save_as;
    QAction* ac_quit;
//! [file]

//! [edit]
    QAction* ac_replace;
//! [edit]

    QMenuBar* menuBar;
    QMenu* fileMenu;
    QMenu* editMenu;

    replaceDialog* replace_dialog;
};
#endif // MAINWINDOW_H
