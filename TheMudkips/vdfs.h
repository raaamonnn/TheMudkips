#ifndef VDFS_H
#define VDFS_H

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
class vDFS;
}

class vDFS : public QWidget
{
    Q_OBJECT

public:
    explicit vDFS(QWidget *parent = nullptr);
    ~vDFS();
    void getNumTeams();
    //!< Gets number of teams (distinct) in the distance db
    void getTeamList();
    //!< Creates master list of distinct team names asc order
    void createAdjList();
    //!< Populates adjacency list with weighted edges
    void DFS(int start);
    //!< Do the depth first search and display to list
    //!< Also calculate total distance and display

private:
    Ui::vDFS *ui;
    QSqlDatabase myDB;      /*!< database connection */

    QString uTeam;           /*!< origin team name    */
    QString vTeam;          /*!< destination team name       : v    */
    float   distance;       /*!< distance between u and v    : wt   */
    int     numTeams;       /*!< number of teams             : V    */
    int     rootIndex;      /*!< starting team index for DFS : root */
    float   tDistance;      /*!< dfs total distance */

    std::string *teamList;  /*!< array (master) list of team names */

    int V;                  /*!< vertices*/

    std::vector<std::pair<int,float>> * adj; /*!< adjacency list pointer */

    //int visited[12]={0};

    int * vis;//declare pointer to visited array
};

#endif // VDFS_H
