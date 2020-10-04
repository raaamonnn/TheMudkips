#include "nbasouvenirs.h"
#include "ui_nbasouvenirs.h"

/****************************************************************************
 * CONSTRUCTOR
 * --------------------------------------------------------------------------
 * Displays default view for window.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      Database class created to connect to database.
 *
 * POST-CONDITIONS
 *      ==> Displays window in default view.
 ***************************************************************************/
nbaSouvenirs::nbaSouvenirs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::nbaSouvenirs)
{
    ui->setupUi(this);

    // Get database connection
    myDB = QSqlDatabase::database();

    QString teamName; // Place holder for setting up List

    // Create new database query
    QSqlQuery * qry = new QSqlQuery(myDB);

    // Set up the query to create ordered list of destination cities
    qry->prepare("SELECT DISTINCT `Beg Team` "
                 "FROM Distances "
                 "ORDER BY `Beg Team` COLLATE NOCASE ASC ");

    // Execute query if valid
    if(qry->exec())
    {
        // Populating list from query
        while(qry->next())
        {
            teamName = qry->value(0).toString();
            ui->teamTable->addItem(teamName);
        }
    }
    else
    {
        qDebug() << ("nbaSouvenir Error: qry failed in defaultview().");
    }

    // Default select index 0 to display
    on_teamTable_currentTextChanged(ui->teamTable->item(0)->text());
    ui->teamTable->setCurrentRow(0);
}

/****************************************************************************
 * DESTRUCTOR
 * --------------------------------------------------------------------------
 * DELETES ui
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      ui exists
 *
 * POST-CONDITIONS
 *      ==> Deletes ui
 ***************************************************************************/
nbaSouvenirs::~nbaSouvenirs()
{
    delete ui;
}

/****************************************************************************
 * METHOD - on_teamTable_currentTextChanged
 * --------------------------------------------------------------------------
 * This method updates team name label and souvenir prices for the selected
 * team.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      currentText : selected team name from teamTable
 *
 * POST-CONDITIONS
 *      ==> Returns nothing.
 ***************************************************************************/
void nbaSouvenirs::on_teamTable_currentTextChanged(const QString &currentText)
{
    QString teamName = currentText;
    QString tableName = teamName + " Store";

    // Display current team selected in line
    ui->teamLine->setText(teamName);

    // Create new database query
    QSqlQuery * sQry = new QSqlQuery(myDB);
    QSqlQueryModel * model = new QSqlQueryModel();

    // Set up the query to create ordered list of NBA teams
    sQry->prepare("SELECT * "
                  "FROM '"+tableName+"' ");
    sQry->exec();
    model->setQuery(*sQry);
    ui->souvenirTable->setModel(model);

    // set table properties
    ui->souvenirTable->setColumnWidth(0, 200);
    ui->souvenirTable->verticalHeader()->resizeSections(QHeaderView::ResizeToContents);
}
