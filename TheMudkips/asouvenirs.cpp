#include "asouvenirs.h"
#include "ui_asouvenirs.h"
#include "string"

aSouvenirs::aSouvenirs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::aSouvenirs)
{
    ui->setupUi(this);
    myDB = QSqlDatabase::database();

    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery * qry = new QSqlQuery(myDB);

    qry->prepare("select * from Souvenirs");
    qry->exec();

    model->setQuery(*qry);
    ui->tableView->setModel(model);

    ui->addName->setEnabled(false);
    ui->addMoney->setEnabled(false);
    ui->addSouvenir->setEnabled(false);

    ui->editMoney->setEnabled(false);
    ui->editSouvenir->setEnabled(false);
    ui->editComboBox->setEnabled(false);

    ui->deleteSouvenir->setEnabled(false);
    ui->deleteComboBox->setEnabled(false);
}

aSouvenirs::~aSouvenirs()
{
    delete ui;
}

void aSouvenirs::on_addSouvenir_clicked()
{
    QString team = ui->comboBox->currentText();
    QString name = ui->addName->toPlainText();
    double money = ui->addMoney->value();


    int weight = 0;

    //name cant be empty or existing
    //team has to be valid
    //money cant be negative

    QSqlQuery * qry = new QSqlQuery(myDB);


    qry->prepare("select * from '"+team+"'");
    qry->exec();

    if(!qry->last())
    {
        qWarning() << "INVALID INPUT";
        return;
    }

    if(name == "")
    {
        qWarning() << "EMPTY NAME";
        return;
    }

    if(money < 0)
    {
        qWarning() << "PRICE IS INVALID";
        return;
    }

//    qry->clear();
    qDebug() << money;

    Map<QString, int>* myMap = new Map<QString, int>;
    myMap->set(name,weight);
    ui->souvName->setText(myMap->update(name,weight));

    qry->prepare("insert into '"+team+"' (item, price) values (?,?)");
    qry->addBindValue(name);
    qry->addBindValue(money);

    if(qry->exec()){
        qDebug()<<"Changes Made";
    }
    else{qDebug()<<"Changes failed";
    qry->lastError();}



    qry->prepare("select * from '"+team+"'");
    qry->exec();

    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery(*qry);
    ui->tableView->setModel(model);

    ui->addName->clear();



    //FOR EDIT COMBO BOX
    QString editTeam = ui->comboBox->currentText();
    QSqlQueryModel* editModel = new QSqlQueryModel();

    QSqlQuery * editQry = new QSqlQuery(myDB);

    editQry->prepare("select item from '"+editTeam+"'");
    editQry->exec();

    editModel->setQuery(*editQry);
    ui->editComboBox->setModel(editModel);
    //FOR DELETE COMBO BOX
    QString deleteTeam = ui->comboBox->currentText();
    QSqlQueryModel* deleteModel = new QSqlQueryModel();

    QSqlQuery * deleteQry = new QSqlQuery(myDB);

    deleteQry->prepare("select item from '"+deleteTeam+"'");
    deleteQry->exec();

    deleteModel->setQuery(*deleteQry);
    ui->deleteComboBox->setModel(deleteModel);

}

void aSouvenirs::on_editSouvenir_clicked()
{
    QString team = ui->comboBox->currentText();
    QString name = ui->editComboBox->currentText();
    double money = ui->editMoney->value();
    //name cant be empty or existing
    //team has to be valid
    //money cant be negative

    QSqlQuery * qry = new QSqlQuery(myDB);

    qry->prepare("select * from '"+team+"'");
    qry->exec();

    if(!qry->last())
    {
        qWarning() << "INVALID INPUT";
        return;
    }


    if(money < 0)
    {
        qWarning() << "PRICE IS INVALID";
        return;
    }


    QString tempMoney = QString::number(money);

    qry->prepare("update '"+team+"' set Price='"+tempMoney+"' where item = '"+name+"'");
    qry->exec();

    QSqlQueryModel* model = new QSqlQueryModel();

    qry->prepare("select * from '"+team+"'");
    qry->exec();

    model->setQuery(*qry);
    ui->tableView->setModel(model);

//    ui->editName->clear();


    //FOR EDIT COMBO BOX
    QString editTeam = ui->comboBox->currentText();
    QSqlQueryModel* editModel = new QSqlQueryModel();

    QSqlQuery * editQry = new QSqlQuery(myDB);

    editQry->prepare("select item from '"+editTeam+"'");
    editQry->exec();

    editModel->setQuery(*editQry);
    ui->editComboBox->setModel(editModel);
    //FOR DELETE COMBO BOX
    QString deleteTeam = ui->comboBox->currentText();
    QSqlQueryModel* deleteModel = new QSqlQueryModel();

    QSqlQuery * deleteQry = new QSqlQuery(myDB);

    deleteQry->prepare("select item from '"+deleteTeam+"'");
    deleteQry->exec();

    deleteModel->setQuery(*deleteQry);
    ui->deleteComboBox->setModel(deleteModel);
}

void aSouvenirs::on_deleteSouvenir_clicked()
{

    QString team = ui->comboBox->currentText();
    QString name = ui->deleteComboBox->currentText();
    //name cant be empty or existing
    //team has to be valid
    //money cant be negative

    QSqlQuery * qry = new QSqlQuery(myDB);

    qry->prepare("select * from '"+team+"'");
    qry->exec();

    if(!qry->last())
    {
        qWarning() << "INVALID INPUT";
        return;
    }


    qry->prepare("delete from '"+team+"' where item = '"+name+"'");
    if (qry->exec())
    {
       qDebug() << "query success";
    }
    else {
        qDebug() << "query failed";
    }

    QSqlQueryModel* model = new QSqlQueryModel();

    qry->prepare("select * from '"+team+"'");
    qry->exec();

    model->setQuery(*qry);
    ui->tableView->setModel(model);

//    ui->deleteName->clear();


    //FOR EDIT COMBO BOX
    QString editTeam = ui->comboBox->currentText();
    QSqlQueryModel* editModel = new QSqlQueryModel();

    QSqlQuery * editQry = new QSqlQuery(myDB);

    editQry->prepare("select item from '"+editTeam+"'");
    editQry->exec();

    editModel->setQuery(*editQry);
    ui->editComboBox->setModel(editModel);
    //FOR DELETE COMBO BOX
    QString deleteTeam = ui->comboBox->currentText();
    QSqlQueryModel* deleteModel = new QSqlQueryModel();

    QSqlQuery * deleteQry = new QSqlQuery(myDB);

    deleteQry->prepare("select item from '"+deleteTeam+"'");
    deleteQry->exec();

    deleteModel->setQuery(*deleteQry);
    ui->deleteComboBox->setModel(deleteModel);
}

void aSouvenirs::on_comboBox_activated(const QString &arg1)
{
    QString team = arg1;
    QSqlQueryModel* model = new QSqlQueryModel();

    qDebug() << team;
    QSqlQuery * qry = new QSqlQuery(myDB);

    qry->prepare("select * from '"+team+"'");
    qry->exec();

    model->setQuery(*qry);
    ui->tableView->setModel(model);

    qDebug() << ui->comboBox->currentText();

    if(ui->comboBox->currentText() == "Choose Arena For Souvenirs")
    {
        ui->addName->setEnabled(false);
        ui->addMoney->setEnabled(false);
        ui->addSouvenir->setEnabled(false);

         ui->editMoney->setEnabled(false);
        ui->editSouvenir->setEnabled(false);
        ui->editComboBox->setEnabled(false);

        ui->deleteSouvenir->setEnabled(false);
        ui->deleteComboBox->setEnabled(false);

    }
    else if(!ui->addName->isEnabled())
    {
        ui->addName->setEnabled(true);
        ui->addMoney->setEnabled(true);
        ui->addSouvenir->setEnabled(true);

        ui->editMoney->setEnabled(true);
        ui->editSouvenir->setEnabled(true);
        ui->editComboBox->setEnabled(true);

        ui->deleteSouvenir->setEnabled(true);
        ui->deleteComboBox->setEnabled(true);
    }

    //FOR EDIT COMBO BOX
    QString editTeam = ui->comboBox->currentText();
    QSqlQueryModel* editModel = new QSqlQueryModel();

    QSqlQuery * editQry = new QSqlQuery(myDB);

    editQry->prepare("select item from '"+editTeam+"'");
    editQry->exec();

    editModel->setQuery(*editQry);
    ui->editComboBox->setModel(editModel);


    //FOR DELETE COMBO BOX
    QString deleteTeam = ui->comboBox->currentText();
    QSqlQueryModel* deleteModel = new QSqlQueryModel();

    QSqlQuery * deleteQry = new QSqlQuery(myDB);

    deleteQry->prepare("select item from '"+deleteTeam+"'");
    deleteQry->exec();

    deleteModel->setQuery(*deleteQry);
    ui->deleteComboBox->setModel(deleteModel);

}
