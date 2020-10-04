#include "nbaconfdiv.h"
#include "ui_nbaconfdiv.h"


nbaConfDiv::nbaConfDiv(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::nbaConfDiv)
{
    ui->setupUi(this);
    myDB = QSqlDatabase::database();
}

nbaConfDiv::~nbaConfDiv()
{
    delete ui;
}

void nbaConfDiv::on_pushButton_2_clicked()
{
        QSqlQueryModel* model = new QSqlQueryModel();

        QSqlQuery * qry = new QSqlQuery(myDB);
        qry->prepare("select * from 'Team Info' where `Conference` = 'Eastern' order by `Team Name`");
        qry->exec();

        model->setQuery(*qry);
        ui->dataView->setModel(model);
}

void nbaConfDiv::on_pushButton_clicked()
{
        qDebug() << "ASD";
        QSqlQueryModel* model = new QSqlQueryModel();

        QSqlQuery * qry = new QSqlQuery(myDB);


        qry->prepare("select * from 'Team Info' where `Division` = 'Southeast' and `Conference` = 'Eastern' order by `Team Name`");
        qry->exec();

        model->setQuery(*qry);
        ui->dataView->setModel(model);
}
