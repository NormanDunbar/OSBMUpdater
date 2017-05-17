#include "toolkitdialog.h"
#include "ui_toolkitdialog.h"
#include "gettoolkitdialog.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QSqlError>


toolkitDialog::toolkitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::toolkitDialog)
{
    ui->setupUi(this);
    tk = NULL;

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}

toolkitDialog::~toolkitDialog()
{
    if (tk) {
        delete tk;
    }

    delete ui;
}


QDialog::DialogCode toolkitDialog::execute(const QSqlDatabase &db) {
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
    QSqlTableModel toolkitModel(this, db);
    toolkitModel.setTable("toolkit");

    toolkitModel.setHeaderData(0, Qt::Horizontal, qApp->tr("TK_ID"));
    toolkitModel.setHeaderData(1, Qt::Horizontal, qApp->tr("TK_Code"));
    toolkitModel.setHeaderData(2, Qt::Horizontal, qApp->tr("TK_Name"));

    // Changes applied immediately.
    toolkitModel.setEditStrategy(QSqlTableModel::OnManualSubmit);
    toolkitModel.select();

    ui->tableView->setModel(&toolkitModel);

    // Make the columns fill the grid space.
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Hide the ID column as we never use it visually.
    ui->tableView->hideColumn(0);

    // Widget mapper to link database columns to form controls.
    mapper->setModel(&toolkitModel);
    //mapper->setItemDelegate(new QSqlRelatonDeleget);
    mapper->addMapping(ui->nameEdit, toolkitModel.fieldIndex("TK_CODE"));
    mapper->addMapping(ui->descriptionEdit, toolkitModel.fieldIndex("TK_NAME"));

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
void toolkitDialog::on_firstButton_clicked()
{
    mapper->toFirst();
}

// Go to the previous record.
void toolkitDialog::on_prevButton_clicked()
{
    mapper->toPrevious();
}

// Go to the next record.
void toolkitDialog::on_nextButton_clicked()
{
    mapper->toNext();
}

// Go to the last record.
void toolkitDialog::on_lastButton_clicked()
{
    mapper->toLast();
}

// Called when the mapper's index changes.
void toolkitDialog::setNavButtons(int index)
{
    // Get the model.
    QSqlTableModel *model = qobject_cast<QSqlTableModel *>(mapper->model());

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
void toolkitDialog::on_addButton_clicked()
{
    // Add a new toolkit.
    if (!tk) {
        tk = new getToolkitDialog(0);
    }

    if (tk) {
        tk->setCaption(qApp->tr("Add New Toolkit"));
        tk->clearData();

        if (tk->exec() == QDialog::Accepted) {
            // Get the model.
            QSqlTableModel *model = qobject_cast<QSqlTableModel *>(mapper->model());

            // Get the final row.
            int rowCount = model->rowCount();

            // Insert after the final row.
            if (model->insertRow(rowCount)) {
                mapper->setCurrentIndex(rowCount);
                ui->nameEdit->setText(tk->getName());
                ui->descriptionEdit->setText(tk->getDescription());
                mapper->submit();
            }
        }
    } else {
        QMessageBox::critical(this, qApp->tr("Error"),
                                    qApp->tr("Cannot create a getToolkitDialog.\n"
                                             "Possibly out of memory/resources?"));
        return;
    }

}

// Delete the current row from the model.
void toolkitDialog::on_deleteButton_clicked()
{
    // Get the model.
    QSqlTableModel *model = qobject_cast<QSqlTableModel *>(mapper->model());
    int currentRow = mapper->currentIndex();

    // Confirm the user's wishes.
    if (QMessageBox::question(this, qApp->tr("Are you sure?"),
                              qApp->tr("Do you really want to delete this toolkit\n"
                                       "plus ALL it's commands, if any?"),
                              QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {

        // Remove the current row from the model.
        if (!model->removeRow(currentRow)) {
            QMessageBox::information(this, qApp->tr("Toolkit NOT Deleted"),
                                     model->database().lastError().text());
        }
    }
}

// Before we exit, we need to commit or rollback all changes.
void toolkitDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    // Get the model.
    QSqlTableModel *model = qobject_cast<QSqlTableModel *>(mapper->model());

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
void toolkitDialog::on_editButton_clicked()
{
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


}

// Edit the highlighted Tollkit's name and/or description
// when user double clicks in the grid.
void toolkitDialog::on_tableView_doubleClicked(const QModelIndex &index)
{
    on_editButton_clicked();
}
