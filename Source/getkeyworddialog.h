#ifndef GETKEYWORDDIALOG_H
#define GETKEYWORDDIALOG_H

#include <QDialog>

namespace Ui {
class getKeywordDialog;
}

class getKeywordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit getKeywordDialog(QWidget *parent = 0);
    ~getKeywordDialog();
    QString getKeyword();
    void setCaption(const QString &newCaption);
    void setData(const QString &newKeyword);
    void clearData();

private:
    Ui::getKeywordDialog *ui;
};

#endif // GETKEYWORDDIALOG_H
