#include "nbacoaches.h"
#include "ui_nbacoaches.h"

nbaCoaches::nbaCoaches(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::nbaCoaches)
{
    ui->setupUi(this);



//    ui->label->setStyleSheet("QLabel { background-color : #8a2be2; color : white; font: bold;}");

//    ui->selectTeamLabel->setStyleSheet("QLabel {  color : black; font: bold;}");

    ui->viewAllButton->setStyleSheet("QPushButton { background-color : #8a2be2; color : white; font: bold;}");
    //POPULATE THE item name combo box
    QSqlQuery * qry1 = new QSqlQuery(myDB);
    QSqlQueryModel * combo = new QSqlQueryModel();
    qry1->prepare("SELECT `Team Name` "
                  "FROM 'Team Info' "
                  "ORDER BY `Team Name` ASC ");
    qry1->exec();

    combo->setQuery(*qry1);

    ui->coachCB->setModel(combo);

    ui->coachCB->setStyleSheet("QComboBox { background-color: #8a2be2; color: white; font: bold; }");


    on_viewAllButton_clicked();//default view

}

nbaCoaches::~nbaCoaches()
{
    delete ui;
}


/****************************************************************************
 * on_viewAllButton_clicked()
 * --------------------------------------------------------------------------
 * Sets view to all teams and their corresponding coaches
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      myDB is is open and connected
 *
 * POST-CONDITIONS
 *      ==> outputs all teams and their corresponding coaches
 ***************************************************************************/
void nbaCoaches::on_viewAllButton_clicked()
{
    myDB = QSqlDatabase::database();

    QSqlQueryModel *model = new QSqlQueryModel;

    model->setQuery("SELECT Coach, `Team Name` "
                    "FROM 'Team Info' "
                    "ORDER BY `Team Name` ASC ");

    if(model->lastError().isValid())
        qDebug() << model->lastError();

    ui->coachesTV->setModel(model);

         // Set Table Column Width
    ui->coachesTV->setColumnWidth(0,150);
    ui->coachesTV->setColumnWidth(1,150);

    // Set Table Column Header Text
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Coaches"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Team Name"));

}

/****************************************************************************
 * on_coachCB_currentIndexChanged()
 * --------------------------------------------------------------------------
 * Sets view to a specific team and coach based on coach selection
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      myDB is is open and connected
 *
 * POST-CONDITIONS
 *      ==> outputs selected team and its corresponding coach
 ***************************************************************************/
void nbaCoaches::on_coachCB_currentIndexChanged()
{

    QSqlQuery * qry1 = new QSqlQuery(myDB);

    QSqlQueryModel * newModel = new QSqlQueryModel();

    QString teamName = ui->coachCB->currentText();

    qry1->prepare("SELECT Coach, `Team Name` "
                  "FROM 'Team Info' "
                  "WHERE `Team Name` = '"+teamName+"' "
                  "ORDER BY `Team Name` ASC ");
    qry1->exec();

    newModel->setQuery(*qry1);

    ui->coachesTV->setModel(newModel);
}
