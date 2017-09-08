#include "getkeyworddialog.h"
#include "ui_getkeyworddialog.h"

getKeywordDialog::getKeywordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::getKeywordDialog)
{
    ui->setupUi(this);
}

getKeywordDialog::~getKeywordDialog()
{
    delete ui;
}

// Called from the calling code to fetch the new
// keyword name. Used to update the database.
QString getKeywordDialog::getKeyword()
{
    return ui->keywordEdit->text();
}

// Called from the calling code to set the form caption
// to determine if this is a new keyword, or an amendment.
void getKeywordDialog::setCaption(const QString &newCaption)
{
    setWindowTitle(newCaption);
}


// Called from the calling code to set the form's data
// when amending an existing keyword.
void getKeywordDialog::setData(const QString &newKeyword)
{
    ui->keywordEdit->setText(newKeyword);
}

// Called from the calling code to set the form's data
// when amending an existing toolkit.
void getKeywordDialog::clearData()
{
    ui->keywordEdit->clear();
}
