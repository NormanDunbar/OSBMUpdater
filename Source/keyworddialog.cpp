#include "keyworddialog.h"
#include "ui_keyworddialog.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QDataWidgetMapper>
#include <QSqlError>


keywordDialog::keywordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keywordDialog)
{
    ui->setupUi(this);

    kw = NULL;
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

keywordDialog::~keywordDialog()
{
    if (kw) {
        delete kw;
    }

    delete ui;
}


QDialog::DialogCode keywordDialog::execute(const QSqlDatabase &db) {
    // Called instead of exec() to pass over the database
    // connection for use here.
    if (!db.isOpen()) {
        QMessageBox::critical(this, qApp->tr("Database Not Open"),
            qApp->tr("Unable to access a closed database.\n"
                     "Please report this as a bug because this\n"
                     "really should never happen!"), QMessageBox::Cancel);
        return QDialog::Rejected;
    }

    // Create the model and tie it to the dialog controls.
    QSqlRelationalTableModel keywordModel(this, db);
    keywordModel.setTable("keyword");

    keywordModel.setHeaderData(0, Qt::Horizontal, qApp->tr("KW_ID"));
    keywordModel.setHeaderData(1, Qt::Horizontal, qApp->tr("KW_Toolkit"));
    keywordModel.setHeaderData(2, Qt::Horizontal, qApp->tr("KW_Name"));
    keywordModel.setHeaderData(3, Qt::Horizontal, qApp->tr("KW_Description"));

    // Changes applied immediately.
    keywordModel.setEditStrategy(QSqlTableModel::OnManualSubmit);

    // Map the FK column to the Toolkit Name.
    keywordModel.setRelation(1, QSqlRelation("TOOLKIT","TK_ID","TK_NAME"));
    keywordModel.select();

    ui->tableView->setModel(&keywordModel);

    // Make the columns fill the grid space.
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    // Only allow one row to be selected, and select the entire row.
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Hide the ID and DESCRIPTION columns as we never use them visually.
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(3);

    // Widget mapper to link database columns to form controls.
    mapper->setModel(&keywordModel);

    // Extract the related table for column 1 which is an FK on toolkit.tk_code.
    mapper->addMapping(ui->toolkitEdit, keywordModel.relationModel(1)->fieldIndex("TK_CODE"));
    mapper->addMapping(ui->keywordEdit, keywordModel.fieldIndex("KW_NAME"));

    // Make sure we have the correct buttons enabled.
    connect(mapper, SIGNAL(currentIndexChanged(int)), this, SLOT(setNavButtons(int)));

    // And make sure we change the current row in the grid.
    connect(mapper, SIGNAL(currentIndexChanged(int)), ui->tableView, SLOT(selectRow(int)));

    // Make sure that clicking on the grid row changes the mapper location too.
    connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            mapper, SLOT(setCurrentModelIndex(QModelIndex)));

    // Start at the first row.
    mapper->toFirst();

    // Call our own exec() to do the dialog stuff.
    return QDialog::DialogCode(exec());
}


// Go to the first record.
void keywordDialog::on_firstButton_clicked()
{
    mapper->toFirst();
}

// Go to the previous record.
void keywordDialog::on_prevButton_clicked()
{
    mapper->toPrevious();
}

// Go to the next record.
void keywordDialog::on_nextButton_clicked()
{
    mapper->toNext();
}

// Go to the last record.
void keywordDialog::on_lastButton_clicked()
{
    mapper->toLast();
}

// Called when the mapper's index changes.
void keywordDialog::setNavButtons(int index)
{
    // Get the model.
    QSqlRelationalTableModel *model = qobject_cast<QSqlRelationalTableModel *>(mapper->model());

    // First and previous can't work if on the first row.
    bool firstRecord = (index == 0);
    ui->firstButton->setEnabled(!firstRecord);
    ui->prevButton->setEnabled(!firstRecord);

    // Delete button can't work if empty.
    // Amend button can't work if empty.
    // Add Button always enabled.
    int rowCount = model->rowCount();
    bool rowsPresent = (rowCount > 0);
    ui->deleteButton->setEnabled(rowsPresent);
    ui->editButton->setEnabled(rowsPresent);

    // Next and last can't work if on the last row.
    bool lastRecord = (index == rowCount - 1);
    ui->lastButton->setEnabled(!lastRecord);
    ui->nextButton->setEnabled(!lastRecord);
}

// Add a new (blank) row to the model, then allow the user to edit the details.
void keywordDialog::on_addButton_clicked()
{
    // Add a new keyword.
    if (!kw) {
        kw = new getKeywordDialog(0);
    }

    if (kw) {
        kw->setCaption(qApp->tr("Add New Keyword"));
        kw->clearData();

        if (kw->exec() == QDialog::Accepted) {
            // Get the model.
            QSqlTableModel *model = qobject_cast<QSqlTableModel *>(mapper->model());

            // Get the final row.
            int rowCount = model->rowCount();

            // Insert after the final row.
            if (model->insertRow(rowCount)) {
                mapper->setCurrentIndex(rowCount);
                ui->keywordEdit->setText(kw->getKeyword());
                mapper->submit();
            }
        }
    } else {
        QMessageBox::critical(this, qApp->tr("Error"),
                                    qApp->tr("Cannot create a getKeywordDialog.\n"
                                             "Possibly out of memory/resources?"));
        return;
    }
}

// Delete the current row from the model.
void keywordDialog::on_deleteButton_clicked()
{
    // Get the model.
    QSqlRelationalTableModel *model = qobject_cast<QSqlRelationalTableModel *>(mapper->model());
    int currentRow = mapper->currentIndex();

    // Confirm the user's wishes.
    if (QMessageBox::question(this, qApp->tr("Are you sure?"),
                              qApp->tr("Do you really want to delete this keyword\n"
                                       "plus ALL its syntax, examples, notes etc, if any?"),
                              QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {

        // Remove the current row from the model.
        if (!model->removeRow(currentRow)) {
            QMessageBox::information(this, qApp->tr("Keyword NOT Deleted"),
                                     model->database().lastError().text());
        }
    }
}

// Before we exit, we need to commit or rollback all changes.
void keywordDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    // Get the model.
    QSqlRelationalTableModel *model = qobject_cast<QSqlRelationalTableModel *>(mapper->model());

    if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole) {
        // Commit.
        if (!model->submitAll()) {
            QMessageBox::critical(this, qApp->tr("Commit Error"),
                                        qApp->tr("Updates have been rolled back.\n\nDatabase Error:\n") +
                                        model->lastError().text());
            model->revertAll();
        }
    } else {
        // Rollback.
        model->revertAll();
    }

}

// Edit the highlighted Tollkit's name and/or description.
void keywordDialog::on_editButton_clicked()
{
    /*
    // Update a toolkit.
    if (!tk) {
        tk = new getToolkitDialog(0);
    }

    if (tk) {
        tk->setCaption(qApp->tr("Amend Current Toolkit"));
        tk->setData(ui->nameEdit->text(), ui->descriptionEdit->text());

        if (tk->exec() == QDialog::Accepted) {
            // Update current row.
            ui->nameEdit->setText(tk->getName());
            ui->descriptionEdit->setText(tk->getDescription());
            mapper->submit();
        }
    } else {
        QMessageBox::critical(this, qApp->tr("Error"),
                                    qApp->tr("Cannot create a getToolkitDialog.\n"
                                             "Possibly out of memory/resources?"));
        return;
    }
*/
}

// Edit the highlighted Tollkit's name and/or description
// when user double clicks in the grid.
void keywordDialog::on_tableView_doubleClicked(const QModelIndex &index)
{
    on_editButton_clicked();
}

