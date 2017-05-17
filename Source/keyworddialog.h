#ifndef KEYWORDDIALOG_H
#define KEYWORDDIALOG_H

#include <QDialog>

class QSqlDatabase;
class QDataWidgetMapper;
class QAbstractButton;
class QModelIndex;

namespace Ui {
class keywordDialog;
}

class keywordDialog  : public QDialog
{
    Q_OBJECT

public:
    explicit keywordDialog(QWidget *parent = 0);
    ~keywordDialog();
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
    Ui::keywordDialog *ui;
    QDataWidgetMapper *mapper;
};


#endif // KEYWORDDIALOG_H
