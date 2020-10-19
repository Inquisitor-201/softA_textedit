#ifndef FILEPROPERTYDIALOG_H
#define FILEPROPERTYDIALOG_H

#include <QDialog>
#include <QFileInfo>

namespace Ui {
class FilePropertyDialog;
}

class FilePropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilePropertyDialog(QWidget *parent = nullptr);
    ~FilePropertyDialog();
    void showProperty(QFileInfo f);

private:
    Ui::FilePropertyDialog *ui;
};

#endif // FILEPROPERTYDIALOG_H
