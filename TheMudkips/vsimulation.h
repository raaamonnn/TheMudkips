#ifndef VSIMULATION_H
#define VSIMULATION_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QListWidget>
#include <QSpinBox>
#include <QTableWidget>
#include <QDebug>

namespace Ui {
class vSimulation;
}

class vSimulation : public QWidget
{
    Q_OBJECT

public:
    explicit vSimulation(QWidget *parent = nullptr);
    explicit vSimulation(QString * tList, double* dList, int t, QWidget * parent =nullptr);
    //!< non default constructor passing team list, distance list and # of teams;

    void defaultTeamList(); //populate team list
    //!< populate team list

    void updateSouvenirs(); // poulate souvenirs relevant to selected team name.
    //!< populate souvenirs relevant to selected team name

    ~vSimulation();

private slots:


    void on_cancelButton_clicked();
     //!< closes out travel simulation window

    void on_endVacationButton_clicked();
     //!< Triggers end of simulation and prints out end vacation message

    void on_confirmSouvenirButton_clicked();
    //!< Confirms souvenirs and quantities selected

    void on_nextDestButton_clicked();
    //!< Moves simulation to the next destination.

private:
    Ui::vSimulation *ui;
    QSqlDatabase myDB;

    QString* destinations = nullptr;
                                /*!< Pointer for dynamic destination list */
    double* distances = nullptr;   /*!< Pointer for dynamic distance list*/
    QString currTeam;           /*!< Current team being visited */
    //int currDistance;           /*!< Distance from last to current team */
    double totalDistance;          /*!< Total distance traveled thus far */
    double currCost;             /*!< Cost of souvenirs at current team */
    double totalCost;            /*!< Souvenir cost in simulation thus far */
    int total;                  /*!< Number of destinations for simulation */
    int index;                  /*!< Used to locate index of current team */
    QFont font;                 /*!< Modified font attributes in QListView */

};

#endif // VSIMULATION_H
