#include "gettoolkitdialog.h"
#include "ui_gettoolkitdialog.h"

getToolkitDialog::getToolkitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::getToolkitDialog)
{
    ui->setupUi(this);
}

getToolkitDialog::~getToolkitDialog()
{
    delete ui;
}

// Called from the calling code to fetch the new
// toolkit name. Used to update the database.
QString getToolkitDialog::getName()
{
    return ui->nameEdit->text();
}

// Called from the calling code to fetch the new
// toolkit description. Used to update the database.
QString getToolkitDialog::getDescription()
{
    return ui->descriptionEdit->text();
}

// Called from the calling code to set the form caption
// to determine if this is a new toolkit, or an amendment.
void getToolkitDialog::setCaption(const QString &newCaption)
{
    setWindowTitle(newCaption);
}


// Called from the calling code to set the form's data
// when amending an existing toolkit.
void getToolkitDialog::setData(const QString &newName, const QString &newDescription)
{
    ui->nameEdit->setText(newName);
    ui->descriptionEdit->setText(newDescription);
}
