#ifndef GETTOOLKITDIALOG_H
#define GETTOOLKITDIALOG_H

#include <QDialog>

namespace Ui {
class getToolkitDialog;
}

class getToolkitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit getToolkitDialog(QWidget *parent = 0);
    ~getToolkitDialog();
    QString getName();
    QString getDescription();
    void setCaption(const QString &newCaption);
    void setData(const QString &newName, const QString &newDescription);
    void clearData();

private:
    Ui::getToolkitDialog *ui;
};

#endif // GETTOOLKITDIALOG_H
