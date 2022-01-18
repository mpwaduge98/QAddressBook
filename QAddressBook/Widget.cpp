#include "Widget.h"
#include "ui_Widget.h"
#include <QPixmap>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QPixmap pixImage(":/images/images/Address.png");
    ui->label_image->setPixmap(pixImage.scaled(700,700, Qt::KeepAspectRatio));
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_open_clicked()
{
    hide();
    user_dashboard = new User_Dashboard();
    user_dashboard->show();
}




