#include "aarena.h"
#include "ui_aarena.h"

aArena::aArena(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::aArena)
{
    ui->setupUi(this);
    myDb=QSqlDatabase::database();

    QSqlQuery* qry = new QSqlQuery(myDb);
    QSqlQueryModel* mdl = new QSqlQueryModel;
    qry->prepare("select `Arena Name`,`Team Name`,`Stadium Capacity` from 'Team Info'");
    if(qry->exec())
    {
        std::cout<<"Arenas loaded";
        mdl->setQuery(*qry);
        ui->tableView->setModel(mdl);}
    else{std::cout<<"Arenas failed to load";}


    qry = new QSqlQuery(myDb);
    mdl = new QSqlQueryModel;
    qry->prepare("select `Arena Name` from 'Team Info'");
    qry->exec();
    mdl->setQuery(*qry);
    ui->ArenaBox_1->setModel(mdl);


    qry = new QSqlQuery(myDb);
    mdl = new QSqlQueryModel;
    qry->prepare("select `Team Name` from 'Team Info'");
    qry->exec();
    mdl->setQuery(*qry);
    ui->TeamBox->setModel(mdl);

}

aArena::~aArena()
{
    delete ui;
}

void aArena::on_pushButton_3_clicked() //update capacity
{
    QString name = ui->ArenaBox_1->currentText();
    QString capacity = ui->lineEdit->text();
    QSqlQuery* query = new QSqlQuery(myDb);
    query->prepare("update 'Team Info' set `Stadium Capacity` = '"+capacity+"' where `Arena Name` = '"+name+"'");
    query->exec();
    //refresh
    QSqlQuery* qry = new QSqlQuery(myDb);
    QSqlQueryModel* mdl = new QSqlQueryModel;
    qry->prepare("select `Arena Name`,`Team Name`,`Stadium Capacity` from 'Team Info'");
    if(qry->exec())
    {
        std::cout<<"Arenas loaded";
        mdl->setQuery(*qry);
        ui->tableView->setModel(mdl);}
    else{std::cout<<"Arenas failed to load";}


    qry = new QSqlQuery(myDb);
    mdl = new QSqlQueryModel;
    qry->prepare("select `Arena Name` from 'Team Info'");
    qry->exec();
    mdl->setQuery(*qry);
    ui->ArenaBox_1->setModel(mdl);


    qry = new QSqlQuery(myDb);
    mdl = new QSqlQueryModel;
    qry->prepare("select `Team Name` from 'Team Info'");
    qry->exec();
    mdl->setQuery(*qry);
    ui->TeamBox->setModel(mdl);
}

void aArena::on_pushButton_2_clicked() //add a new arena
{
    QString arena = ui->ArenaName->text();
    QString capacity = ui->ArenaCap->text();
    QString team = ui->TeamBox->currentText();

    //get the original stadium name
    QString oldName;
    QSqlQuery* qury = new QSqlQuery(myDb);
    qury->prepare("select `Arena Name` from 'Team Info' where `Team Name` ='"+team+"'");
    qury->exec();
    oldName = qury->value(0).toString();

    //update team info
    QSqlQuery* query = new QSqlQuery(myDb);
    query->prepare("update 'Team Info' set `Arena Name` = '"+arena+"', `Stadium Capacity` = '"+capacity+"' where `Team Name` = '"+team+"'");
    query->exec();

    //update distances
    QSqlQuery* qery = new QSqlQuery(myDb);
    qery->prepare("update 'Distances' set `Beg Arena` = '"+arena+"' where `Beg Team` = '"+team+"'");
    qery->exec();

    //refresh
    QSqlQuery* qry = new QSqlQuery(myDb);
    QSqlQueryModel* mdl = new QSqlQueryModel;
    qry->prepare("select `Arena Name`,`Team Name`,`Stadium Capacity` from 'Team Info'");
    if(qry->exec())
    {
        std::cout<<"Arenas loaded";
        mdl->setQuery(*qry);
        ui->tableView->setModel(mdl);}
    else{std::cout<<"Arenas failed to load";}


    qry = new QSqlQuery(myDb);
    mdl = new QSqlQueryModel;
    qry->prepare("select `Arena Name` from 'Team Info'");
    qry->exec();
    mdl->setQuery(*qry);
    ui->ArenaBox_1->setModel(mdl);


    qry = new QSqlQuery(myDb);
    mdl = new QSqlQueryModel;
    qry->prepare("select `Team Name` from 'Team Info'");
    qry->exec();
    mdl->setQuery(*qry);
    ui->TeamBox->setModel(mdl);
}
