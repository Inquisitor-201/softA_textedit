#include "mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
//#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createUi();
}

void MainWindow::createUi()
{
    textEdit = new QTextEdit(this);
    setCentralWidget(textEdit);
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);                   //新建菜单栏

    fileMenu = new QMenu(this);            //新建文件菜单

    ac_newfile = new QAction("新建(&N)");
    ac_openfile = new QAction("打开(&O)");
    ac_savefile = new QAction("保存(&S)");
    ac_save_as = new QAction("另存为(&A)");
    ac_quit = new QAction("关闭(&Q)");

    connect(ac_newfile, &QAction::triggered, this, &MainWindow::newFile);
    connect(ac_openfile, &QAction::triggered, this, &MainWindow::openFile);
    connect(ac_savefile, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ac_save_as, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ac_quit, &QAction::triggered, this, &MainWindow::QuitProgram);

    ac_newfile->setShortcut(QKeySequence::New);
    ac_openfile->setShortcut(QKeySequence::Open);
    ac_savefile->setShortcut(QKeySequence::Save);
    ac_save_as->setShortcut(QKeySequence::SaveAs);
    ac_quit->setShortcut(QKeySequence::Quit);

    fileMenu->setTitle("文件");
    fileMenu->addAction(ac_newfile);
    fileMenu->addAction(ac_openfile);
    fileMenu->addAction(ac_savefile);
    fileMenu->addAction(ac_save_as);
    fileMenu->addAction(ac_quit);

    editMenu = new QMenu(this);
    editMenu->setTitle("编辑");             //新建编辑菜单
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);
}

bool MainWindow::newFile()
{
    if (!querySave())
        return false;
    current_filename = "";
    textEdit->clear();
    textEdit->document()->setModified(false);
    return true;
}

bool MainWindow::openFile()          //打开文件
{
    if (!querySave())
        return false;
    QString filename = QFileDialog::getOpenFileName(NULL, "打开文件", "./", "All files (*.*);; text (*.txt)");
    if (filename.isEmpty())
        return false;                            //选择要打开的文件，将其文件名存放在filename中

    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite)){
        qWarning("无法打开文件！");
        return false;
    }
    QTextStream in(&file);
    QString content = in.readAll();

    textEdit->setText(content);
    textEdit->document()->setModified(false);
    current_filename = filename;                 //将文件内容输出到文本框中
    return true;
}

bool MainWindow::saveAs()
{
    QString save_filename;
    if (!getSaveFileName(&save_filename))
        return false;
    if (!saveAsFile(save_filename))
        return false;
    current_filename = save_filename;
    return true;
}

bool MainWindow::saveAsFile(QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning("Failed to open the device.");
        return false;
    }
    QTextStream out(&file);
    out << textEdit->document()->toPlainText();
    textEdit->document()->setModified(false);
    return true;
}

bool MainWindow::getSaveFileName(QString* str)
{
    QFileDialog fdialog(this, "另存为..");
    fdialog.setFileMode(QFileDialog::AnyFile);
    fdialog.setNameFilter("text(*.txt)");
    fdialog.setAcceptMode(QFileDialog::AcceptSave);
    if (fdialog.exec() == QFileDialog::Reject) {
        qWarning("保存失败.");
        return false;
    }
    if (fdialog.selectedFiles().empty())
        return false;
    *str = fdialog.selectedFiles()[0];
    return true;
}

bool MainWindow::saveFile()
{
    if (!textEdit->document()->isModified())
        return true;
    if (current_filename.isEmpty())
    {
        if (!getSaveFileName(&current_filename))
            return false;
    }
    bool ret = saveAsFile(current_filename);
    return ret;
}

bool MainWindow::QuitProgram()
{
    if (!querySave())
        return false;
    return close();
}

bool MainWindow::querySave()
{
    if (!textEdit->document()->isModified())
        return true;

    QMessageBox msgbox(this);
    msgbox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgbox.setText("文件 " + (current_filename.isEmpty() ? "(Untitled)" : current_filename) + " 已修改。");
    msgbox.setInformativeText("是否保存？");
    msgbox.setDefaultButton(QMessageBox::Save);
    int ret = msgbox.exec();

    switch (ret)
    {
        case QMessageBox::Save:
            saveFile();
            break;
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            return false;
    }
    return true;
}

MainWindow::~MainWindow()
{
    //delete ui;
}

