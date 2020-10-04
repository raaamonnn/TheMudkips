#ifndef VPACKAGEB_H
#define VPACKAGEB_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QListWidget>
#include "vsimulation.h"
#include <cfloat>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>



namespace Ui {
class vPackageB;
}

class vPackageB : public QWidget
{
    Q_OBJECT

public:
    explicit vPackageB(QWidget *parent = nullptr);

    ~vPackageB();

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

    void sortTeamOptimize();
    //!< Creates the sorted dynamic team and destination arrays

    void populateList();
    //!< Populates array of teams/distances, calls sort and displays sorted list of teams in q list Widget


private slots:
    void on_cancelButton_clicked();
    //!< Exits confirmation screen
    void on_startButton_clicked();
    //!< Starts Travel Simulation


private:

    Ui::vPackageB *ui;

    QSqlDatabase myDB;
    /*!< database connection */

    vSimulation *simulationWindow;
    /*!< class object for simulation window */

    int *parentArr;
    /*!< parentage array with child index, parent value */

    QString teamName;
    /*!< team name */

    QString startTeam = "Detroit Pistons"; //hard code to detroit pistons
    /*!< starting team name */

    int sTeamIndex;
    /*!< starting team index from master list */

    QString closestTeam;
    /*!< team closest to starting team */

    // The two passed dynamic arrays to vSimulation

    QString* sortedTeams = nullptr;
    /*!< pointer to point to list of sorted teams */

    double* sortedDistance = nullptr;
    /*!< pointer to point to list of sorted destinations */

    // Used for creating adjacency matrix
    QString uTeam;          /*!< origin team name         : u */
    QString vTeam;          /*!< destination team name    : v  */
    double  distance;       /*!< distance between u and v : wt */
    int teamTotal;          /*!< total number of teams    : V */
    std::string *teamList;  /*!< array (master) list of team names */
    double      **adj;      /*!< adjacency matrix for weighted edges */

};



#endif // VPACKAGEB_H
