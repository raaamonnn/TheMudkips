#include "vbfs.h"
#include "ui_vbfs.h"

/****************************************************************************
 * CONSTRUCTOR
 * --------------------------------------------------------------------------
 * Displays default view for window.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      Database class created to connect to database.
 *
 * POST-CONDITIONS
 *      ==> Displays default view.
 ***************************************************************************/
vBFS::vBFS(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vBFS)
{
    ui->setupUi(this);

    // initialize needed variables
    rootIndex = 0;
    bool isFound = false;

    // get number of distinct team names in db
    getNumTeams();

    // create dynamic master list for team names
    teamList = new std::string [numTeams];

    // populate team master list from db
    getTeamList();

    // gets the rootIndex of the starting team by finding it in master list
    while(!isFound)
    {
        if(teamList[rootIndex] == "Los Angeles Lakers")
            isFound = true;
        else
            rootIndex++;
    }

    // create dynamic adjacency matrix (pointers array)
    adj = new float* [numTeams];

    // create columns for adjacency matrix
    for(int i = 0; i < numTeams; i++)
    {
        // creates the columns in the 2d dynamic array
        adj[i] = new float[numTeams];
    }

    // initialize adjacenency matrix to 9999.9
    for(int i = 0; i < numTeams; i++)
    {
        for(int j = 0; j < numTeams; j++)
        {
            adj[i][j] = -1;
        }
    }

    // populate adjacency matrix (add edges)
    createMatrix();

    // do breath first search
    // display results to list and calculate total distance
    BFS();

    // display total distance
    ui->distanceLine->setText(QString::number(tDistance,'f', 2));
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
vBFS::~vBFS()
{
    delete [] teamList;
    delete ui;
}

/****************************************************************************
 * getNumTeams
 * --------------------------------------------------------------------------
 * Gets the number of distinct beginning team names from distance db.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      myDB is is open and connected
 *
 * POST-CONDITIONS
 *      ==> Nothing.
 ***************************************************************************/
void vBFS::getNumTeams()
{
    // Create new database query
    QSqlQuery * qry = new QSqlQuery(myDB);

    // Set up the query to create ordered list of teams, except startTeam
    qry->prepare("SELECT COUNT (DISTINCT `Beg Team`) "
                 "FROM Distances");

    // Execute query if valid
    if(qry->exec())
    {
        qry->next();
        // get total team number
        numTeams = qry->value(0).toInt();
    }
    else
    {
        qDebug() << ("BFS getNumTeams Error: qry failed.");
    }
}

/****************************************************************************
 * getTeamList
 * --------------------------------------------------------------------------
 * Create team name master list, sorted in ascending order.  Their index
 * will be used for referencing the team name associated with it.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      myDB is is open and connected
 *
 * POST-CONDITIONS
 *      ==> Nothing.
 ***************************************************************************/
void vBFS::getTeamList()
{
    int index = 0;

    // Create new database query
    QSqlQuery * qry = new QSqlQuery(myDB);

    // Set up the query to create ordered list of teams, except startTeam
    qry->prepare("SELECT DISTINCT `Beg Team` "
                 "FROM Distances "
                 "ORDER BY `Beg Team` COLLATE NOCASE ASC ");

    // Execute query if valid
    if(qry->exec())
    {
        // Populating master team list from query
        while(qry->next())
        {
            uTeam = qry->value(0).toString();
            teamList[index] = uTeam.toStdString();
            index++;
        }
    }
    else
    {
        qDebug() << ("BFS getTeamList Error: qry failed.");
    }
}

/****************************************************************************
 * createMatrix
 * --------------------------------------------------------------------------
 * Creates an adjacency matrix for beginning team name as column (u),
 * ending team name as rows (v), and the distance between them as the weight
 * in the cell.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      myDB is is open and connected
 *
 * POST-CONDITIONS
 *      ==> Nothing.
 ***************************************************************************/
void vBFS::createMatrix()
{
    int sIndex = 0; // start team index value
    int dIndex = 0; // destination team index value
    bool isFound = false;

    // Create new database query
    QSqlQuery * qry = new QSqlQuery(myDB);

    // Set up the query to create ordered list of teams, except startTeam
    qry->prepare("SELECT * "
                 "FROM Distances");

    // Execute query if valid
    if(qry->exec())
    {
        // Populating list from query
        while(qry->next())
        {
            sIndex = 0; // start team index value
            dIndex = 0; // destination team index value

            // Find start team in team list for its index
            isFound = false;
            uTeam = qry->value(0).toString();
            while(!isFound && sIndex < numTeams)
            {
                // check if there is a match
                if(teamList[sIndex] == uTeam.toStdString())
                    isFound = true;
                else
                    sIndex++;
            }

            // Find destination team in team list for its index
            isFound = false;
            vTeam = qry->value(2).toString();
            while(!isFound && dIndex < numTeams)
            {
                // check if there is a match
                if(teamList[dIndex] == vTeam.toStdString())
                    isFound = true;
                else
                    dIndex++;
            }

            distance = qry->value(3).toFloat();

            // Add weighted edge into adjacency matrix
            adj[sIndex][dIndex] = distance;
        }
    }
    else
    {
        qDebug() << ("BFS createMatrix: qry failed.");
    }
}

/****************************************************************************
 * BFS
 * --------------------------------------------------------------------------
 * Does the breath first search and displays the result in a list view.
 * Also, keeps track of total distance traveled.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      myDB is is open and connected
 *
 * POST-CONDITIONS
 *      ==> Displays the result of the bfs.
 ***************************************************************************/
void vBFS::BFS()
{
    std::vector<bool> visited(numTeams,false); // tracks visited status
    std::vector<int> q;                        // query
    std::vector<std::tuple<int, int, float>> temp; // u, v, wt
    int u, v;      // u and v are the vertex
    float wt;      // the weighted edge
    int vis;       // current team being visited (u)
    QString tempStr;  // used for temporary holding a string value

    // put first vertex on queue, set it to visited, and display in list
    q.push_back(rootIndex);
    visited[rootIndex] = true;
    tempStr = QString::fromStdString(teamList[rootIndex]);
    ui->bfsList->addItem(tempStr);

    // continue until all vertex have been visited (queue is empty)
    while(!q.empty())
    {
        // get all destinations from origin (vis)
        while(!q.empty())
        {
            vis = q[0];
            q.erase(q.begin());

            // find all teams that are destinations to vis
            for(int i = 0; i < numTeams; i++)
            {
                if(adj[vis][i] >= 0 && (!visited[i]))
                    temp.push_back(std::make_tuple(vis,i,adj[vis][i]));
            }

            // sort the teams by lowest to highest on weighted edge
            // using lambda for third argument
            sort(temp.begin(), temp.end(),
                 [](std::tuple<int, int, float> const &a,
                    std::tuple<int, int, float> &b)
                    {
                        return (std::get<2>(a) < std::get<2>(b));
                    });
        }

        ui->bfsList->addItem("--------- Level Change ---------");

        // display information if not visited and put on queue
        // adds to total distance if visiting
        for(auto it = temp.begin(); it != temp.end(); ++it)
        {
            u = std::get<0>(*it);
            v = std::get<1>(*it);
            wt = std::get<2>(*it);

            // if the team is not visited yet, visit and put on queue
            if(!visited[v])
            {
                tDistance = tDistance + wt;
                q.push_back(v);
                visited[v] = true;
                tempStr = QString::fromStdString(teamList[v]);
                ui->bfsList->addItem(tempStr);
            }
        }

        // ensures temp list is empty before being used again
        temp.clear();
    }
}
