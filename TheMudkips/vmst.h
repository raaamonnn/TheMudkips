#ifndef VMST_H
#define VMST_H

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
class vMST;
}

class vMST : public QWidget
{
    Q_OBJECT

public:
    explicit vMST(QWidget *parent = nullptr);
    ~vMST();
    void getNumTeams();
    //!< Gets number of teams (distinct) in the distance db
    void getTeamList();
    //!< Creates master list of distinct team names asc order
    void createMatrix();
    //!< Populates adjacency matrix with edges
    void Kruskals();
    //!< Performs Kruskals algorithm

    void join(int u,int v);
    //!< Union checking sub function for Kruskals
    int find(int u);
    //!< Another union checking sub function for Kruskals

private:
    Ui::vMST *ui;
    QSqlDatabase myDB;      /*!< database connection */

    QString uTeam;          /*!< origin team name            : u    */
    QString vTeam;          /*!< destination team name       : v    */
    float   distance;       /*!< distance between u and v    : wt   */
    int     numTeams;       /*!< number of teams             : V    */
    //int     rootIndex;      /*!< starting team index for BFS : root */
    float   tDistance;      /*!< bfs total distance */

    std::string *teamList;  /*!< array (master) list of team names */
    float       **adj;            /*!< adjacency matrix for weighted edges */
    int * set;             // set pointer to array holding sets for kruskals
    bool * included;       // included pointer to array holding included values

};

#endif // VMST_H
