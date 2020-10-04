#include "addteam.h"
#include "ui_addteam.h"

addTeam::addTeam(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addTeam)
{
    ui->setupUi(this);
    myDb=QSqlDatabase::database();
}

addTeam::~addTeam()
{
    delete ui;
}

void addTeam::on_pushButton_clicked() //confirm button
{
    QString conference = ui->comboBox->currentText();
    QString division = ui->comboBox_2->currentText();
    QString team_name = ui->name_input->text();
    QString location = ui->location_input->text();
    QString arena_nname = ui->arena_input->text();
    QString stadium_cap = ui->cap_input->text();
    QString year = ui->year_input->text();
    QString coach = ui->coach_input->text();

    QSqlQuery* query = new QSqlQuery;
    query->prepare("insert into 'Team Info' (Conference,Division,`Team Name`,Location,`Arena Name`,`Stadium Capacity`,`Joined League`,Coach)"
                   "values ('"+conference+"','"+division+"','"+team_name+"','"+location+"','"+arena_nname+"','"+stadium_cap+"','"+year+"','"+coach+"')");
    if(query->exec())
    {
        std::cout << "added team succesfully";

    }

}

void addTeam::on_pushButton_2_clicked()
{
    this->hide();
}
