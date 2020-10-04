#include "vsimulation.h"
#include "ui_vsimulation.h"


vSimulation::vSimulation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vSimulation)
{
    ui->setupUi(this);
}

vSimulation::vSimulation(QString *tList, double *dList, int t, QWidget *parent):
    QWidget(parent), ui(new Ui::vSimulation),destinations(tList), distances(dList), total(t)
{
    ui->setupUi(this);

    // Get database connection
    myDB = QSqlDatabase::database();
    index=0;
    totalCost = 0;
    totalDistance = 0;
    currCost = 0;
    //qDebug() << "TOTAL: " << total;
    ui->endVacationButton->setEnabled(false);
    ui->goodbyeLabel->setVisible(false);
    defaultTeamList();
    updateSouvenirs();

    QString currBegTeam = destinations[0];
    //qDebug()<< "curr Beg Team" << currBegTeam;

     QSqlQuery * qry0 = new QSqlQuery(myDB);

     QString tempTeam;

    qry0->prepare("SELECT * "
                 "FROM 'Team Info' "
                 "WHERE `Team Name` = '"+currBegTeam+"' ");

    // Execute query if valid
    if(qry0->exec())
    {
        //qDebug()<<"Qry executing?";
       while(qry0->next())
       {

            ui->currArenaLabel_3->setText(qry0->value(4).toString());

       }
    }

}

vSimulation::~vSimulation()
{
    delete ui;
}

void vSimulation::on_cancelButton_clicked()
{

    this->close();
}

void vSimulation::on_endVacationButton_clicked()
{

    this->close();
}

/****************************************************************************
 * default Team List
 * --------------------------------------------------------------------------
 * populates team list widget from passed destinations array
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      myDB is is open and connected
 *
 * POST-CONDITIONS
 *      ==> populated team list widget
 ***************************************************************************/

void vSimulation::defaultTeamList()
{

    for(int i =0; i < total; i++)
    {
        ui->destinationWidget->addItem(destinations[i]);
        //qDebug() << "LOOK: " << destinations[i];
    }


    font.setBold(true);
    ui->destinationWidget->item(index)->setFont(font);
    ui->destinationWidget->item(index)->setForeground(Qt::magenta);
    ui->destinationWidget->item(index)->setBackground(Qt::black);

}

void vSimulation::updateSouvenirs()
{

    int row = 0;
    int rowCount = 0;
    currTeam = ui->destinationWidget->item(index)->text();
    QString tableName = currTeam + " Store";
    QSqlQuery qry;
    qry.prepare("SELECT * "
                    "FROM '"+tableName+"'" );

    // Count each row from the query
    if(qry.exec())
    {
        while(qry.next())
        {

            rowCount++;
            //qDebug() << "ROW COUNT " << rowCount;

        }
    }

    // Sets column and row
    ui->souvenirTableWidget->setColumnCount(3);
    ui->souvenirTableWidget->setRowCount(rowCount);
    // Sets name labels
    ui->souvenirTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Qty"));
    ui->souvenirTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Souvenir"));
    ui->souvenirTableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Price"));

    // Sets column widths
    ui->souvenirTableWidget->setColumnWidth(0, 50);
    ui->souvenirTableWidget->setColumnWidth(1, 200);
    ui->souvenirTableWidget->setColumnWidth(2, 50);

    qry.prepare("SELECT * "
                    "FROM '"+tableName+"'" );

    if(qry.exec())
    {
        while(qry.next())
        {

           QTableWidgetItem * item1 = new QTableWidgetItem;
           //item1->setFlags(item1->flags() ^ Qt::ItemIsEnabled);
           //item1->setTextColor(Qt::black);

          // Sets up column 2 view and text to display
          // Makes it uneditable
          // Includes two decimal place floating point precision
          QTableWidgetItem *item2 = new QTableWidgetItem;
          item2->setTextAlignment(Qt::AlignRight);
          //item2->setFlags(item2->flags() ^ Qt::ItemIsEnabled);
          //item2->setTextColor(Qt::black);
          item1->setText(qry.value(0).toString());
          //qDebug()<< "item 1: " << qry.value(0).toString();
          item2->setText(QString::number(qry.value(1).toDouble(), 'f', 2));
          //qDebug() << "item 2: " <<qry.value(1).toFloat();

          // Populate one row of the souvenirTableWidget
          ui->souvenirTableWidget->setCellWidget(row,0, new QSpinBox(ui->souvenirTableWidget));
          ui->souvenirTableWidget->setItem(row, 1, item1);
          ui->souvenirTableWidget->setItem(row, 2, item2);

          row++;

        }
    }

    //new

    /*///////////////////////////////////////*/
    QSqlQuery * qry1 = new QSqlQuery(myDB);
    QString tempString;



        // Set up the query to create ordered list of teams
        qry1->prepare("SELECT * "
                     "FROM Distances "
                     "WHERE `Beg Team` = '"+currTeam+"' ");

        // Execute query if valid
        if(qry1->exec())
        {
            //qDebug()<<"Qry executing?";
           while(qry1->next())
           {

               //qDebug()<<"ARENA QRY VALUE 4";
               ui->currArenaLabel_3->setText(qry1->value(1).toString());

           }

        } //end if

    //new


    ui->nextDestButton->setEnabled(false);//disable next destination button

}



void vSimulation::on_confirmSouvenirButton_clicked()
{
    int i = 0;
    int itemQty = 0;
    float itemPrice = 0;
    currCost = 0;

    while(i < ui->souvenirTableWidget->rowCount())
    {
        itemQty = ui->souvenirTableWidget->cellWidget(i,0)->property("value").value<int>();
        itemPrice = (ui->souvenirTableWidget->item(i,2)->text()).toFloat();
        currCost = currCost + (itemQty * itemPrice);
        //qDebug() << itemQty << " " << currCost;
    i++;
    }
    ui->currDestCostLine->setText(QString::number(currCost));
    ui->nextDestButton->setEnabled(true);
}

void vSimulation::on_nextDestButton_clicked()
{
/************************************************************************
     * Only execute the following codes if there is another city after
     * the current index.  The index changes inside the code, so there
     * must be at least one more city needed to run these codes.
     ***********************************************************************/
    if(index+1 < total)
    {
        // Reset last dest to default in listWidget
        font.setBold(false);
        ui->destinationWidget->item(index)->setFont(font);
        ui->destinationWidget->item(index)->setForeground(Qt::black);
        ui->destinationWidget->item(index)->setBackground(Qt::white);




        index++;



        // Set current dest to active in listWidget
        font.setBold(true);
        ui->destinationWidget->item(index)->setFont(font);
        ui->destinationWidget->item(index)->setForeground(Qt::yellow);
        ui->destinationWidget->item(index)->setBackground(Qt::black);

        updateSouvenirs();

        // Updates distance lines
        totalDistance = totalDistance + distances[index];
        ui->totalDistLine->setText(QString::number(totalDistance));

        // Updates cost lines
        totalCost = totalCost + currCost;
        ui->totalCostLine->setText(QString::number(totalCost));

        // Reset values
        ui->currDestCostLine->clear();
        ui->nextDestButton->setEnabled(false);

        if(index+1 >= total)
        {
            ui->nextDestButton->setText("End Vacation");
        }
    }
    else
    {
        // Updates cost lines
        totalCost = totalCost + currCost;
        ui->totalCostLine->setText(QString::number(totalCost));

        // Reset values
        ui->currDestCostLine->clear();
        ui->nextDestButton->setEnabled(false);

        ui->endVacationButton->setEnabled(true);
        ui->goodbyeLabel->setVisible(true);
        ui->confirmSouvenirButton->setVisible(false); //make sure you can't click confirm anymore
        //ui->endVacationButton->setEnabled(false);
    }
}
