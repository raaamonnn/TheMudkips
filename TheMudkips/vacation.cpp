#include "vacation.h"
#include "ui_vacation.h"

Vacation::Vacation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Vacation)
{
    ui->setupUi(this);

    // Create a stacked widget index for seperate QWidget pages
    ui->stackedWidget->insertWidget(1, &bfsWindow);
    ui->stackedWidget->insertWidget(2, &dfsWindow);
    ui->stackedWidget->insertWidget(3, &mstWindow);
}

Vacation::~Vacation()
{
    delete ui;
}

void Vacation::on_packagesButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Vacation::on_bfsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Vacation::on_dfsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Vacation::on_mstButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void Vacation::on_exitButton_clicked()
{
    this->close();
}

void Vacation::on_packageAButton_clicked()
{
    packageAWindow = new vPackageA();
    packageAWindow->show();
}

void Vacation::on_packageBButton_clicked()
{
    packageBWindow = new vPackageB();
    packageBWindow->show();
}

void Vacation::on_packageCButton_clicked()
{
    packageCWindow = new vPackageC();
    packageCWindow->show();
}
