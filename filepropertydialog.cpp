#include "filepropertydialog.h"
#include "ui_filepropertydialog.h"
#include <QMimeDatabase>
#include <QDateTime>

FilePropertyDialog::FilePropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilePropertyDialog)
{
    ui->setupUi(this);
    connect(ui->b_cancel, &QPushButton::released, [=](){close();});
}

void FilePropertyDialog::showProperty(QFileInfo f)
{
    ui->l_name->setText(f.fileName().isEmpty() ? f.filePath() : f.fileName());
    ui->l_path->setText(f.absolutePath());
    ui->l_type->setText(QMimeDatabase().mimeTypeForFile(f.filePath()).name());
    ui->l_owner->setText(f.owner().isEmpty() ? "Administrator" : f.owner());
    ui->l_lastread->setText(f.lastRead().toString("yyyy/MM/dd hh:mm:ss"));
    ui->l_lastmodified->setText(f.lastModified().toString("yyyy/MM/dd hh:mm:ss"));

    qint64 size = f.size();

    if (f.isDir())
        ui->l_size->setText("---");
    else if (size < 1024)
        ui->l_size->setText(QString::number(size) + " Bytes");
    else if (size < 1024 * 1024)
        ui->l_size->setText(QString::number(size / 1024.0, 'f', 2) + " KB");
    else if (size < 1024 * 1024 * 1024)
        ui->l_size->setText(QString::number(size / 1024.0 / 1024.0, 'f', 2) + " MB");
    else
        ui->l_size->setText(QString::number(size / 1024.0 / 1024.0 / 1024.0, 'f', 2) + " GB");
    exec();
}

FilePropertyDialog::~FilePropertyDialog()
{
    delete ui;
}
