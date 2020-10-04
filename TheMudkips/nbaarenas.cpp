#include "nbaarenas.h"
#include "ui_nbaarenas.h"

nbaArenas::nbaArenas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::nbaArenas)
{
    ui->setupUi(this);

    //POPULATE THE item name combo box
    QSqlQuery * qry1 = new QSqlQuery(myDB);
    QSqlQueryModel * combo = new QSqlQueryModel();
    qry1->prepare("SELECT `Arena Name` "
                  "FROM 'Team Info' "
                  "ORDER BY `Arena Name` ASC ");
    qry1->exec();

    combo->setQuery(*qry1);

    ui->arenaCB->setModel(combo);

    ui->arenaCB->setStyleSheet("QComboBox { background-color: LightBlue; }");

    on_arenaButton_clicked(); //default view

    totalCapacity(); //grand total initialized


}

nbaArenas::~nbaArenas()
{
    delete ui;
}

/****************************************************************************
 * on_arenaButton_clicked()
 * --------------------------------------------------------------------------
 * Displays all teams, arenas and seating capacities sorted by arena in alphabetical
 * order.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      myDB is is open and connected
 *
 * POST-CONDITIONS
 *      ==> outputs all teams, arenas and seating capacities sorted by arena
 * in alphabetical order
 ***************************************************************************/

void nbaArenas::on_arenaButton_clicked()
{
       myDB = QSqlDatabase::database();

       QSqlQueryModel *model = new QSqlQueryModel;

       model->setQuery("SELECT `Arena Name`, `Team Name`,  `Stadium Capacity` "
                       "FROM 'Team Info' "
                       "ORDER BY `Arena Name` ASC ");

       if(model->lastError().isValid())
           qDebug() << model->lastError();

       ui->arenaTV->setModel(model);

            // Set Table Column Width
       ui->arenaTV->setColumnWidth(0,150);
       ui->arenaTV->setColumnWidth(1,150);
       ui->arenaTV->setColumnWidth(2,150);


       // Set Table Column Header Text
       model->setHeaderData(0, Qt::Horizontal, QObject::tr("Arena Name"));
       model->setHeaderData(1, Qt::Horizontal, QObject::tr("Team Name"));
       model->setHeaderData(2, Qt::Horizontal, QObject::tr("Seating Capacity"));

}
/****************************************************************************
 * on_stadiumCapacityButton_clicked()
 * --------------------------------------------------------------------------
 * Displays all teams, arenas and seating capacities sorted by seating capacity
 * lowest to highest.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      myDB is is open and connected
 *
 * POST-CONDITIONS
 *      ==> outputs all teams, arenas and seating capacities sorted by seating capacity
 * lowest to highest.
 ***************************************************************************/
void nbaArenas::on_stadiumCapacityButton_clicked()
{
    myDB = QSqlDatabase::database();

    QSqlQueryModel *model = new QSqlQueryModel;

    model->setQuery("SELECT `Stadium Capacity`, `Arena Name`,`Team Name` "
                    "FROM 'Team Info' "
                    "ORDER BY `Stadium Capacity` ASC ");

    if(model->lastError().isValid())
        qDebug() << model->lastError();

    ui->arenaTV->setModel(model);

         // Set Table Column Width
    ui->arenaTV->setColumnWidth(0,150);
    ui->arenaTV->setColumnWidth(1,150);
    ui->arenaTV->setColumnWidth(2,150);


    // Set Table Column Header Text
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Seating Capacity"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Arena Name"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Team Name"));
}
/****************************************************************************
 * totalCapacity()
 * --------------------------------------------------------------------------
 * Calculates total seating capacity
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      myDB is is open and connected
 *
 * POST-CONDITIONS
 *      ==> outputs total seating capacity
 ***************************************************************************/
void nbaArenas::totalCapacity()
{
    total = 0;
    QSqlQuery qry;
    qry.prepare("SELECT * "
                "FROM 'Team Info' "
                "ORDER BY `Stadium Capacity` ASC ");
    qry.exec();

    while(qry.next())
    {
        total = total+ qry.value(5).toInt();
        qDebug() << "total" << total;
    }

    ui->totalLabel1->setNum(total);
}

/****************************************************************************
 * on_arenaCB_currentIndexChanged()
 * --------------------------------------------------------------------------
 * Displays one team, arena and seating capcity based on arena selected.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      myDB is is open and connected
 *
 * POST-CONDITIONS
 *      ==> outputs one team, arena and seating capacity based on arena selected.
 ***************************************************************************/
void nbaArenas::on_arenaCB_currentIndexChanged()
{

    //Update Table View based selected arena.
    QSqlQuery * qry1 = new QSqlQuery(myDB);

    QSqlQueryModel * newModel = new QSqlQueryModel();

    QString name = ui->arenaCB->currentText();

    qry1->prepare("SELECT `Arena Name`, `Team Name`, `Stadium Capacity` "
                  "FROM 'Team Info' "
                  "WHERE `Arena Name` = '"+name+"' "
                  "ORDER BY `Arena Name` ASC ");
    qry1->exec();

    newModel->setQuery(*qry1);

    ui->arenaTV->setModel(newModel);

  }
