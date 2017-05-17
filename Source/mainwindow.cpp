#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "toolkitdialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QSqlQuery>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Locate the "databases" folder beneath the executable.
    databaseFolder = qApp->applicationDirPath() + "/databases";
    databaseName = "default.osbm";

    // Make sure we can use SQLLite3 databases.
    db = QSqlDatabase::addDatabase("QSQLITE");
}

MainWindow::~MainWindow()
{
    // Close the database, if open.
    closeDatabase();
    delete ui;
}


bool MainWindow::createConnection(const QString &databasePath)
{
    // Make sure we shut down any open databases.
    closeDatabase();

    db.setDatabaseName(databasePath);
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "This example needs SQLite support. Please read "
                     "the Qt SQL driver documentation for information how "
                     "to build it.\n\n"), QMessageBox::Cancel);
        return false;
    }

    // Database is open.
    QMessageBox::information(this, qApp->tr("Database Open"),
                                   qApp->tr("Database: ") + db.databaseName() + qApp->tr(" opened."),
                                   QMessageBox::Ok);

    // Enable Foreign keys.
    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON;");

    return true;
}

// Close the current database, if one is open. Called
// from lots of places. Usually by an action though.
void MainWindow::closeDatabase() {
    if (db.isOpen()) {
        db.close();
        updateButtons(false);
        QMessageBox::information(this, qApp->tr("Database Closed"),
                                       qApp->tr("Database: ") + db.databaseName() + qApp->tr(" closed."),
                                       QMessageBox::Ok);
    }
}

// Update the buttons. Only the Database button stays enabled.
// The rest depend on there being an open database.
// If enable is true, there's an open database.
void MainWindow::updateButtons(const bool &enable) {
    // First do the buttons.
    ui->buttonToolkit->setEnabled(enable);
    ui->buttonCommand->setEnabled(enable);
    ui->buttonExport->setEnabled(enable);
}

// Open a new database after prompting for a name.
void MainWindow::openDatabase() {
    QString newDatabase = QFileDialog::getOpenFileName(this, qApp->tr("Open Database"),
                                                       databaseFolder,
                                                       qApp->tr("OLSBM Databases (*.osbm);;All Files (*.*)"));


    if (!newDatabase.isEmpty()) {
        updateButtons(createConnection(newDatabase));
    }
}

void MainWindow::on_buttonDatabase_clicked()
{
    // Open the database, or close it.
    if (!db.isOpen()) {
        openDatabase();
        if (db.isOpen()) {
            ui->buttonDatabase->setIcon(QIcon("://images/close-database.png"));
            ui->buttonDatabase->setToolTip(qApp->tr("Close database"));
        }
    } else {
        closeDatabase();
        if (!db.isOpen()) {
            ui->buttonDatabase->setIcon(QIcon("://images/open-database.png"));
            ui->buttonDatabase->setToolTip(qApp->tr("Open database"));
        }
    }
}

void MainWindow::on_buttonToolkit_clicked()
{
    // Someone clicked the toolkits button!
    toolkitDialog *tk = new toolkitDialog;
    tk->execute(db);
    delete tk;
}


void MainWindow::on_buttonCommand_clicked()
{
    // Someone clicked the keywords button!
    //keywordDialog *kw = new keywordDialog;
    //kw->execute(db);
    //delete kw;
}
