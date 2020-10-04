#ifndef VPACKAGEA_H
#define VPACKAGEA_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QListWidget>
#include "vsimulation.h"

namespace Ui {
class vPackageA;
}

class vPackageA : public QWidget
{
    Q_OBJECT

public:
    explicit vPackageA(QWidget *parent = nullptr);
    //!< Constructor

    ~vPackageA();
    //!< Destructor

    void defaultPackage();
    //!< Sets default view for list on left side listing teams from db

    void sortTeam();
    //!< Creates the sorted dynamic team and destination arrays

    void getTeamTotal();
    //!< Gets total number of teams (distinct) in the distance db

    void getTeamList();
    //!< Creates master list of distinct team names asc order

    void createMatrix();
    //!< Populates adjacency matrix with edges

    void doDijkstras();
    //!< Does dikstra's algorithm to the adjacency matrix

    int minDistance(double lowDist[], bool cloudT[]);
    //!< Used by doDijkstras() to determine minimum distance node

    void sortTeamNormal();
    //!< Creates the sorted dynamic team and destination arrays
    //!< NOT OPTIMIZED: fan choice of order

private slots:
    void on_cancelButton_clicked();
    //!< Closes window and returns fan to package window

    void on_startButton_clicked();
    //!< Opens vacation simulation window and closes this window

    void on_confirmButton_clicked();
    //!< Creates list, calls sortTeamList(), and displays arrays in list

private:
    Ui::vPackageA *ui;/*!< class object */
    QSqlDatabase myDB;/*!< database connection */
    vSimulation *simulationWindow;/*!< class object for simulation window */

    QString tName; /*!< team name */
    QString startTeam = "Denver Nuggets"; /*!< starting team name */
    QList<QListWidgetItem *> customList;/*!< list of teams selected from list */
    int teamNum;/*!< number of teams selected */
    QString* sortedTeams = nullptr;
                            /*!< pointer to point to list of sorted teams */
    double* sortedDistance = nullptr;
                            /*!< pointer to point to list of sorted destinations */

    int *parentArr;
    /*!< parentage array with child index, parent value */

    QString teamName;       /*!< team name */
    int sTeamIndex;         /*!< starting team index from master list */
    QString closestTeam;    /*!< team closest to starting team */

    // Used for creating adjacency matrix
    QString uTeam;          /*!< origin team name         : u */
    QString vTeam;          /*!< destination team name    : v  */
    double  distance;       /*!< distance between u and v : wt */
    int teamTotal;          /*!< total number of teams    : V */
    std::string *teamList;  /*!< array (master) list of team names */
    double      **adj;      /*!< adjacency matrix for weighted edges */
};

#endif // VPACKAGEA_H
