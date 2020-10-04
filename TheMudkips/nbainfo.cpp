#include "nbainfo.h"
#include "ui_nbainfo.h"

NBAInfo::NBAInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NBAInfo)
{
    ui->setupUi(this);

    // Create a stacked widget index for seperate QWidget pages
    ui->stackedWidget->insertWidget(1, &teamWindow);
    ui->stackedWidget->insertWidget(2, &arenaWindow);
    ui->stackedWidget->insertWidget(3, &confDivWindow);
    ui->stackedWidget->insertWidget(4, &coachWindow);
    ui->stackedWidget->insertWidget(5, &souvenirsWindow);
}

NBAInfo::~NBAInfo()
{
    delete ui;
}

void NBAInfo::on_homeButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void NBAInfo::on_teamsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void NBAInfo::on_arenasButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void NBAInfo::on_confDivButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void NBAInfo::on_coachesButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void NBAInfo::on_souvenirsButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void NBAInfo::on_exitButton_clicked()
{
    this->close();
}
