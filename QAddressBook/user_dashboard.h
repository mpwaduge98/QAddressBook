#ifndef USER_DASHBOARD_H
#define USER_DASHBOARD_H


#include <QDialog>
#include <QtSql>
#include <QDebug>


namespace Ui {
class User_Dashboard;
}

class User_Dashboard : public QDialog
{
    Q_OBJECT

public:
    QSqlDatabase mydb;

    void connClose()
    {
        mydb.close(); //closing my database
        mydb.removeDatabase(QSqlDatabase::defaultConnection); //closing my database and removing any connections there
    }

    bool connOpen()
    {
        mydb = QSqlDatabase::addDatabase("QSQLITE");

        //copy and paste the filepath of the mydb.db file
        mydb.setDatabaseName("D:/Qt Projects/QAddressBook/database/mydb.db");

        if(!mydb.open())
        {
            qDebug() << "Failed to connect";
            return false;
        }

        else{
                qDebug() << "Database Connected...";
                return true;
            }

    }

public:
    explicit User_Dashboard(QWidget *parent = nullptr);
    ~User_Dashboard();

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_load_clicked();

    void on_tableView_activated(const QModelIndex &index);

    void on_pushButton_search_clicked();

    void on_pushButton_export_clicked();

    void on_pushButton_count_clicked();



private:
    Ui::User_Dashboard *ui;

    QWidget * QWidget;
};

#endif // USER_DASHBOARD_H
