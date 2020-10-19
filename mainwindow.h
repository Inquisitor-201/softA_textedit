﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QTextEdit>
#include <QTreeWidget>
#include <QSplitter>
#include <QHash>
#include <QList>
#include <QStackedWidget>
#include "highlighter.h"
#include "customedit.h"
#include "filepropertydialog.h"
#include "binaryeditor.h"

class replaceDialog;
//QT_BEGIN_NAMESPACE
//namespace Ui { class MainWindow; }
//QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void findNext();
    void replaceAll();
    void replace();
    void highlightFindContent();
    void clearHighlightContent();      //清除当前由“查找”产生的高亮

private slots:
    void appendFileToEditor(QTreeWidgetItem*, int);
    void popupContextMenu(const QPoint &pos);

private:
    void setCurrentFileName(QString);
    void createToolBar();
    void createMenu();
    void setupTextEdit();
    void setupDialogs();
    void createUI();
    void createFileTreeWidget();

    bool newFile();
    bool openFile();
    void openProject();
    bool saveFile();                   //保存文件（若已存在文件名，则直接保存，否则调用getSaveFileName)
    bool saveAs();                     //另存为
    bool QuitProgram();                //关闭程序

    bool saveAsFile(QString);          //给定一个QString filename，将文本框的内容保存到filename的文件中
    bool getSaveFileName(QString*);    //展示一个QFileDialog, 将需要保存的文件名存放到给定地址中
    bool querySave();                  //询问是否要保存，返回为1为已决定“是否要保存”，返回0为选择了“cancel”

    void trace(QString, QTreeWidgetItem*);
    void deleteChildren(QTreeWidgetItem*);

    void openInExplorer();             //给定filename, 在文件管理器中打开该文件
    void createContextMenu();          //创建filetree的右键目录

    QString current_filename;
    QString selected_filetree_filename;
    QTreeWidgetItem* selected_filetree_widget;

    customEdit* textEdit;

//! [file]
    QAction* ac_newfile;
    QAction* ac_openfile;
    QAction* ac_open_project;
    QAction* ac_savefile;
    QAction* ac_save_as;
    QAction* ac_quit;
//! [file]

//! [edit]
    QAction* ac_undo;
    QAction* ac_redo;
    QAction* ac_find;
    QAction* ac_replace;
    QAction* ac_cut;
    QAction* ac_copy;
    QAction* ac_paste;
    QAction* ac_select_all;
//! [edit]

    QMenuBar* menuBar;
    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* FileTreeContextMenu;

    QStatusBar* stbar;
    BinaryEditor* binEdit;
    QStackedWidget* editArea;

    QToolBar* toolBar;
    QListWidget* openedFileList;
    QTreeWidget* fileTree;
    replaceDialog* replace_dialog;
    FilePropertyDialog* fileproperty_dialog;
    QHash<QString, QString> textMap;
};
#endif // MAINWINDOW_H
