#ifndef VBFS_H
#define VBFS_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

namespace Ui {
class vBFS;
}

class vBFS : public QWidget
{
    Q_OBJECT

public:
    explicit vBFS(QWidget *parent = nullptr);
    //!< Constructor

    ~vBFS();
    //!< Destructor

    void getNumTeams();
    //!< Gets number of teams (distinct) in the distance db

    void getTeamList();
    //!< Creates master list of distinct team names asc order

    void createMatrix();
    //!< Populates adjacency matrix with edges

    void BFS();
    //!< Do the breath first search and display to list
    //!< Also calculate total distance and display

private:
    Ui::vBFS *ui;
    QSqlDatabase myDB;      /*!< database connection */

    QString uTeam;          /*!< origin team name            : u    */
    QString vTeam;          /*!< destination team name       : v    */
    float   distance;       /*!< distance between u and v    : wt   */
    int     numTeams;       /*!< number of teams             : V    */
    int     rootIndex;      /*!< starting team index for BFS : root */
    float   tDistance;      /*!< bfs total distance */

    std::string *teamList;  /*!< array (master) list of team names */
    float       **adj;            /*!< adjacency matrix for weighted edges */

};

#endif // VBFS_H
