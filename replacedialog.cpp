#include "replacedialog.h"
#include "ui_replacedialog.h"
#include "mainwindow.h"

replaceDialog::replaceDialog(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::replaceDialog)
{
    ui->setupUi(this);
    setFixedSize(430, 160);
    setMainWindow(parent);
    //查找 对话框
    connect(ui->button_findNext, &QPushButton::released, main_window, &MainWindow::findNext);
    connect(ui->button_cancel, &QPushButton::released, [=](){close();});
    connect(ui->findContent, &QPlainTextEdit::textChanged, main_window, &MainWindow::highlightFindContent);
    connect(ui->check_capital, &QCheckBox::stateChanged, main_window, &MainWindow::highlightFindContent);
    //替换 对话框
    connect(ui->rbutton_findNext, &QPushButton::released, main_window, &MainWindow::findNext);
    connect(ui->rbutton_replace, &QPushButton::released, main_window, &MainWindow::replace);
    connect(ui->rbutton_cancel, &QPushButton::released, [=](){close();});
    connect(ui->rbutton_replace_all, &QPushButton::released, main_window, &MainWindow::replaceAll);
    connect(ui->rFindContent, &QPlainTextEdit::textChanged, main_window, &MainWindow::highlightFindContent);
    connect(ui->rcheck_capital, &QCheckBox::stateChanged, main_window, &MainWindow::highlightFindContent);
}

void replaceDialog::setMainWindow(MainWindow* mw) {
    main_window = mw;
}

QString replaceDialog::findContent() {
    if (ui->rdlg_tab->currentIndex() == 0)       // 当前是查找对话框
        return ui->findContent->toPlainText();
    else
        return ui->rFindContent->toPlainText();
}

bool replaceDialog::checkedCapital()
{
    if (ui->rdlg_tab->currentIndex() == 0)
        return ui->check_capital->checkState() > 0;  //是否“区分大小写”
    else
        return ui->rcheck_capital->checkState() > 0;
}

bool replaceDialog::checkedRepeat()
{
    if (ui->rdlg_tab->currentIndex() == 0)
        return ui->check_repeat->checkState() > 0;  //是否“循环查找”
    else
        return ui->rcheck_repeat->checkState() > 0;
}

QString replaceDialog::targetText()
{
    return ui->rTargetContent->toPlainText();
}

void replaceDialog::changeIndex(int index){
    ui->rdlg_tab->setCurrentIndex(index);
}

void replaceDialog::closeEvent(QCloseEvent *e)
{
    main_window->clearHighlightContent();
    QDialog::closeEvent(e);
}

replaceDialog::~replaceDialog()
{
    delete ui;
}
