#include "user_dashboard.h"
#include "ui_user_dashboard.h"

#include <QMessageBox>
#include <QPixmap>
#include <QFile>
#include <QTextStream>


User_Dashboard::User_Dashboard(class QWidget *parent) :
    QDialog(parent),
    ui(new Ui::User_Dashboard)
{
    ui->setupUi(this);

    QPixmap pixImage(":/images/images/Forms.png");
    ui->label_topimg->setPixmap(pixImage.scaled(400,400, Qt::KeepAspectRatio));

    QPixmap pixImage2(":/icons/icons/users.svg");
    ui->label_userIcon->setPixmap(pixImage2.scaled(24,24,Qt::KeepAspectRatio));


    if(!connOpen())
        ui->label_status->setText("Failed to connect");

    else
        ui->label_status->setText("Database Connected...");

}

User_Dashboard::~User_Dashboard()
{
    delete ui;
}

//*********When Add Button is clicked*****************************//


void User_Dashboard::on_pushButton_add_clicked()
{
    QString userId, Firstname, Lastname, email, phone;
    userId = ui->lineEdit_id->text();
    Firstname = ui->lineEdit_fname->text();
    Lastname = ui->lineEdit_lname->text();
    email = ui->lineEdit_email->text();
    phone = ui->lineEdit_phone->text();

    if(!connOpen())
    {
        qDebug() << "Failed to open Database";
        return;
    }

    QSqlQuery query;

    //insert into TABLE_NAME(Column Names) values ('"+ QString variable names corresponding to each column name +"')

    query.prepare("insert into users(UserID, FirstName, LastName, Email, PhoneNumber) values ('"+ userId +"', '"+ Firstname +"', '"+ Lastname +"', '"+ email +"', '"+ phone +"')");

    if(query.exec())
    {
        QMessageBox::critical(this, tr("Save"), tr("Query Saved Successfully"));
        connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("error::"), query.lastError().text());
    }
}

//*********When Edit Button is clicked*****************************//


void User_Dashboard::on_pushButton_edit_clicked()
{
    QString userId, Firstname, Lastname, email, phone;
    userId = ui->lineEdit_id->text();
    Firstname = ui->lineEdit_fname->text();
    Lastname = ui->lineEdit_lname->text();
    email = ui->lineEdit_email->text();
    phone = ui->lineEdit_phone->text();

    if(!connOpen())
    {
        qDebug() << "Failed to open Database";
        return;
    }

    QSqlQuery query;

    //update TABLE_NAME set column_name = '"+ corresponding QString Vraiable name +"'

    query.prepare("update users set UserID = '"+ userId +"', FirstName = '"+ Firstname +"', LastName = '"+ Lastname +"', Email = '"+ email +"', PhoneNumber = '"+ phone +"' where UserID = '"+ userId +"' ");

    if(query.exec())
    {
        QMessageBox::critical(this, tr("Edit"), tr("Query Updated Successfully"));
        connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("error::"), query.lastError().text());
    }


}

//*********When Delete Button is clicked*****************************//


void User_Dashboard::on_pushButton_delete_clicked()
{
    QString userId, Firstname, Lastname, email, phone;
    userId = ui->lineEdit_id->text();
    Firstname = ui->lineEdit_fname->text();
    Lastname = ui->lineEdit_lname->text();
    email = ui->lineEdit_email->text();
    phone = ui->lineEdit_phone->text();

    if(!connOpen())
    {
        qDebug() << "Failed to open Database";
        return;
    }

    QSqlQuery query;

    //delete from TABLE_NAME where COLUMN_NAME = '"+ corresponding QString Vraiable name +"'

    query.prepare("Delete from users where UserID = '"+ userId +"' ");

    if(query.exec())
    {
        QMessageBox::critical(this, tr("Delete"), tr("Query Deleted Successfully"));
        connClose();
    }
    else
    {
        QMessageBox::critical(this, tr("error::"), query.lastError().text());
    }

}

//*********When Load Button is clicked*****************************//


void User_Dashboard::on_pushButton_load_clicked()
{

    QSqlQueryModel * model = new QSqlQueryModel();
    connOpen(); //connection with database is open
    QSqlQuery * qry = new QSqlQuery(mydb);

    qry->prepare("select * from users");

    qry->exec(); //query is executed
    model-> setQuery(*qry); //transfer the data gotten from the query into the model
    ui->tableView->setModel(model);

    connClose(); //connection with database is closed

    qDebug() << (model->rowCount());
}



//**********Whenever a value on a row on the table is clicked, the data will be displayed on the line edits**********//


void User_Dashboard::on_tableView_activated(const QModelIndex &index)
{
    //To take the index selected from the table, convert the index,
    //and take the value of the index and tranfer it to the QString variable 'value'

    QString value = ui->tableView->model()->data(index).toString();

    if(!connOpen()) //check whether the Database connection is opened
    {
        qDebug() << "Failed to Open Database";
        return;
    }

     connOpen(); //Database connection open
     QSqlQuery query;
     query.prepare("select * from users where UserID = '"+ value +"' or FirstName = '"+ value +"' or LastName = '"+ value +"' or Email = '"+ value +"' or PhoneNumber = '"+ value +"' ");


     //execute the query and if it is executing the values received are being displayed on the corresponding line edits
     if(query.exec())
     {
         while(query.next())
         {
             ui->lineEdit_id->setText(query.value(0).toString());
             ui->lineEdit_fname->setText(query.value(1).toString());
             ui->lineEdit_lname->setText(query.value(2).toString());
             ui->lineEdit_email->setText(query.value(3).toString());
             ui->lineEdit_phone->setText(query.value(4).toString());
         }
         connClose(); //database connection is closed
     }

     //if the query is not executing properly, an error message will be displayed.
     else
     {
         QMessageBox::critical(this,tr("error::"),query.lastError().text());
     }
}

//***********When a text(FirstName/LastName/UserID) is entered into the searchbox the corresponding row is displayed***********//


void User_Dashboard::on_pushButton_search_clicked()
{
    QString text = ui->lineEdit_search->text();
    QSqlQueryModel * model = new QSqlQueryModel();
    connOpen();
    QSqlQuery * query = new QSqlQuery(mydb);

    query->prepare("select * from users where  UserID = '"+ text +"' or FirstName = '"+ text +"' or LastName = '"+ text +"'");

    query->exec();
    model->setQuery(*query);
    ui->tableView->setModel(model);
    connClose();

    qDebug() << (model->rowCount());
}


//*************** When Export File Button is clicked  ***************//


void User_Dashboard::on_pushButton_export_clicked()
{
    QSqlQueryModel * model = new QSqlQueryModel();
    connOpen(); //connection with database is open
    QSqlQuery * qry = new QSqlQuery(mydb);

    qry->prepare("select * from users");

    qry->exec(); //query is executed
    model-> setQuery(*qry); //transfer the data gotten from the query into the model
    ui->tableView->setModel(model);


    QString data;

    int rows = model->rowCount();
    int columns = model->columnCount();

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            data += model->data(model->index(i,j)).toString();
            data += " | ";
        }
        data += "\n\n";
    }
    //To export the database table to a txt file
    //Copy and paste the file path of MyContactList.txt
    QFile myFile("D:/Qt Projects/QAddressBook/Files/MyContactList.txt");
    if(myFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QTextStream out(&myFile);
        out << data;

        myFile.close();
    }

    connClose(); //connection with database is closed

    qDebug() << (model->rowCount());

}

//*************** When count button is clicked ***************//


void User_Dashboard::on_pushButton_count_clicked()
{
   QSqlQueryModel model;
   connOpen();
   QSqlQuery query;
   query.prepare("select count(*) from users");
   query.exec();
   int rec = 0;
   if(query.next())
   {
       rec = query.value(0).toInt();
   }

   ui->label_countRec->setNum(rec);
   connClose();

}


