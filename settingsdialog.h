#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "customedit.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    virtual ~SettingsDialog();
    void initialize();

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
