#include "mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
//#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    //, ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    createUi();
}

void MainWindow::createUi()
{
    textEdit = new QTextEdit(this)
    setCentralWidget(textEdit);
    setFixedSize(400, 300);
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);                   //新建菜单栏

    fileMenu = new QMenu(this);
    ac_openfile = new QAction("打开(&N)");  //action:打开文件

    connect(ac_openfile, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->setTitle("文件");             //新建文件菜单
    fileMenu->addAction(ac_openfile);

    editMenu = new QMenu(this);
    editMenu->setTitle("编辑");             //新建编辑菜单
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);
}

void MainWindow::openFile()   //打开文件
{
    QString filename = QFileDialog::getOpenFileName(NULL, "打开文件", "./", "All files (*.*);; text (*.txt)");
    QFile file(filename);
    if (!file.open(QFile::ReadWrite)){
        qWarning("无法打开文件！");
        return;
    }
    QTextStream in(&file);
    QString content = in.readAll();
    qDebug() << content;
    textEdit->setText(content);
}

MainWindow::~MainWindow()
{
    //delete ui;
}

