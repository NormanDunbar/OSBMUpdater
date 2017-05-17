#ifndef TOOLKITDIALOG_H
#define TOOLKITDIALOG_H

#include <QDialog>
#include "gettoolkitdialog.h"

class QSqlDatabase;
class QDataWidgetMapper;
class QAbstractButton;
class QModelIndex;

namespace Ui {
class toolkitDialog;
}

class toolkitDialog  : public QDialog
{
    Q_OBJECT

public:
    explicit toolkitDialog(QWidget *parent = 0);
    ~toolkitDialog();
    DialogCode execute(const QSqlDatabase &db);

private slots:
    void on_firstButton_clicked();
    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_lastButton_clicked();
    void setNavButtons(int index);
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_editButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::toolkitDialog *ui;
    QDataWidgetMapper *mapper;
    getToolkitDialog *tk;
};


#endif // TOOLKITDIALOG_H
