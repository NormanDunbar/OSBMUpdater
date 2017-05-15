#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool createConnection(const QString &databasePath);

private slots:
    void on_buttonToolkit_clicked();
    void on_buttonDatabase_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QString databaseFolder;
    QString databaseName;
    void closeDatabase();
    void openDatabase();
    void updateButtons(const bool &enable);
};

#endif // MAINWINDOW_H
