﻿#ifndef MAINWINDOW_H
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
    bool saveFile();                   //保存文件（若已存在文件名，则直接保存，否则调用getSaveFileName)
    bool saveAs();                     //另存为

    bool saveAsFile(QString&);         //给定一个QString filename，将文本框的内容保存到filename的文件中
    bool getSaveFileName(QString*);    //展示一个QFileDialog, 将需要保存的文件名存放到给定地址中
    bool querySave();                  //询问是否要保存，返回为1为已决定“是否要保存”，返回0为选择了“cancel”

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
