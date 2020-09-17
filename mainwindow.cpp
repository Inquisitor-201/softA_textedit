﻿#include "mainwindow.h"
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

    connect(ac_newfile, &QAction::triggered, this, &MainWindow::newFile);
    connect(ac_openfile, &QAction::triggered, this, &MainWindow::openFile);
    connect(ac_savefile, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ac_save_as, &QAction::triggered, this, &MainWindow::saveAs);

    ac_newfile->setShortcut(QKeySequence::New);
    ac_openfile->setShortcut(QKeySequence::Open);
    ac_savefile->setShortcut(QKeySequence::Save);
    ac_save_as->setShortcut(QKeySequence::SaveAs);

    fileMenu->setTitle("文件");
    fileMenu->addAction(ac_newfile);
    fileMenu->addAction(ac_openfile);
    fileMenu->addAction(ac_savefile);
    fileMenu->addAction(ac_save_as);

    editMenu = new QMenu(this);
    editMenu->setTitle("编辑");             //新建编辑菜单
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);
}

bool MainWindow::newFile()
{
    current_filename = "";
    textEdit->clear();
    textEdit->document()->setModified(false);
    return true;
}

bool MainWindow::openFile()   //打开文件
{
    QString filename = QFileDialog::getOpenFileName(NULL, "打开文件", "./", "All files (*.*);; text (*.txt)");
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite)){
        qWarning("无法打开文件！");
        return false;
    }
    QTextStream in(&file);
    QString content = in.readAll();

    textEdit->setText(content);
    textEdit->document()->setModified(false);
    current_filename = filename;
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
    QFileDialog fdialog(this, "Save as..");
    fdialog.setFileMode(QFileDialog::AnyFile);
    fdialog.setNameFilter("text(*.txt)");
    fdialog.setAcceptMode(QFileDialog::AcceptSave);
    if (fdialog.exec() == QFileDialog::Reject) {
        qWarning("Failed to save the file.");
        return false;
    }
    if (fdialog.selectedFiles().empty())
        return false;
    *str = fdialog.selectedFiles()[0];
    return true;
}

bool MainWindow::saveFile()
{
    if (!textEdit->document()->isModified() || textEdit->document()->isEmpty())
        return true;
    if (current_filename.isEmpty())
    {
        if (!getSaveFileName(&current_filename))
            return false;
    }
    bool ret = saveAsFile(current_filename);
    return ret;
}

MainWindow::~MainWindow()
{
    //delete ui;
}

