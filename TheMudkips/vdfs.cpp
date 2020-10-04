#include "vdfs.h"
#include "ui_vdfs.h"

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

vDFS::vDFS(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vDFS)
{
    ui->setupUi(this);


    // initialize needed variables
    rootIndex = 0;
    bool isFound = false;
    tDistance = 0; //set distance to 0

    getNumTeams(); //get number of teams
//    qDebug()<< "numTeams: " << numTeams;

    //create dynamic array of visited arrays
    vis = new int [numTeams];

    //initialize all to 0

    for(int i=0;i<numTeams;i++)
    {
        vis[i] = 0;
    }

    // create dynamic master list for team names
    teamList = new std::string [numTeams];

    // populate team master list from db
    getTeamList();

    // gets the rootIndex of the starting team by finding it in master list
    while(!isFound)
    {
        if(teamList[rootIndex] == "Orlando Magic")
            isFound = true;
        else
            rootIndex++;
    }

     // create adj list
     adj = new std::vector<std::pair<int,float>>[numTeams];

     createAdjList();

     //sort it

     //call DFS sort
     DFS(rootIndex);//passing rootIndex to DFS search

     ui->distanceLine->setText(QString::number(tDistance, 'f',2));
}
/****************************************************************************
 * Destructor
 * --------------------------------------------------------------------------
 * Deletes UI and removes memomry from heap for dynamic array
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      Database class created to connect to database.
 *
 * POST-CONDITIONS
 *      ==> Displays default view.
 ***************************************************************************/
vDFS::~vDFS()
{

    delete [] teamList;
    delete ui;

}

void vDFS::getNumTeams()
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
        qDebug() << ("DFS getNumTeams Error: qry failed.");
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
void vDFS::getTeamList()
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
//           std::cout<< teamList[index] << " " << index << " ";
            index++;
        }
    }
    else
    {
        qDebug() << ("DFS getTeamList Error: qry failed.");
    }



}

/****************************************************************************
 * createAdjList
 * --------------------------------------------------------------------------
 * Creates adjacency list to store weighted edges
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      myDB is is open and connected
 *
 * POST-CONDITIONS
 *      ==> Weighted edges added to adj list
 ***************************************************************************/
void vDFS::createAdjList()
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
            adj[sIndex].push_back(std::make_pair(dIndex,distance));

            sort(adj[sIndex].begin(), adj[sIndex].end(),
                 [](std::pair<int,float> const &a,
                    std::pair<int,float> &b)
                    {
                        return (a.second < b.second);
                    });

        }
    }
    else
    {
        qDebug() << ("DFS createMatrix: qry failed.");
    }
}

/****************************************************************************
 * DFS
 * --------------------------------------------------------------------------
 * Completes a depth first search traversal starting from Orlando Magic
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      myDB is is open and connected
 *
 * POST-CONDITIONS
 *      ==> DFS complete. Total distance calculated.
 ***************************************************************************/
void vDFS::DFS(int start)
{

    //std::vector<std::pair<int,float>> temp[];

//    qDebug()<<"Start" << start;

int v;
float wt;
QString tempStr;  // used for temporary holding a string value

//static array of visited.

if(vis[start] == 0)
   {
//       std::cout << "\tVisiting " << teamList[start] << std::endl << std::endl;

      vis[start] = 1;

      tempStr = QString::fromStdString(teamList[start]);
      ui->dfsList->addItem(tempStr);
//      std::cout<<"BEFORE THE ITERATION";

            for(auto it = adj[start].begin(); it!=adj[start].end(); it++) //iterate through adj list
            {
               // std::cout <<"INSIDE THE ITERATION";
                v = it->first;
               // std::cout << "Testing v" << v;
                wt = it->second;
               // std::cout<<"Testing wt" << wt;

               // std::cout << "\tNow checking: " << teamList[start] << " -> " << teamList[v];
                if(vis[v] == 0)
                {
                    //std::cout << " - (DISCOVERY EDGE)\n";
                    tDistance = tDistance + wt;
//                    qDebug()<<"tDistance" << tDistance;

                    DFS(v);
                }
                //else
                   // std::cout << " - (BACK EDGE)\n\n";
            }
        }
//        std::cout << "\tBack track.\n\n";


}

