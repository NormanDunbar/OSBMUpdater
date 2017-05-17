#include "keyworddialog.h"
#include "ui_keyworddialog.h"

keywordDialog::keywordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keywordDialog)
{
    ui->setupUi(this);
}

keywordDialog::~keywordDialog()
{
    delete ui;
}
