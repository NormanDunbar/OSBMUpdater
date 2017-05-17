#ifndef KEYWORDDIALOG_H
#define KEYWORDDIALOG_H

#include <QDialog>

namespace Ui {
class keywordDialog;
}

class keywordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit keywordDialog(QWidget *parent = 0);
    ~keywordDialog();

private:
    Ui::keywordDialog *ui;
};

#endif // KEYWORDDIALOG_H
