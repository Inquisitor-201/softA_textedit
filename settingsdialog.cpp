#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    setFixedSize(405, 465);
    ui->setupUi(this);
}

void SettingsDialog::initialize()
{
    ui->fontComboBox->setCurrentFont(*globalFont);
    ui->spinbox_fontsize->setValue(globalFont->pixelSize());
    exec();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}


