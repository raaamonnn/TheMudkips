#include "admin.h"
#include "ui_admin.h"

Admin::Admin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);

    // Create a stacked widget index for seperate QWidget pages
    ui->stackedWidget->insertWidget(1, &teamWindow);
    ui->stackedWidget->insertWidget(2, &arenaWindow);
    ui->stackedWidget->insertWidget(3, &souvenirsWindow);
}

Admin::~Admin()
{
    delete ui;
}

void Admin::on_homeButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Admin::on_page1Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Admin::on_page2Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Admin::on_page3Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void Admin::on_exitButton_clicked()
{
    this->close();
}


