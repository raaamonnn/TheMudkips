#include "vpackageb.h"
#include "ui_vpackageb.h"

//constructor
vPackageB::vPackageB(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vPackageB)
{
    ui->setupUi(this);
    // Get database connection
    myDB = QSqlDatabase::database();

    // get number of distinct team names in db
    getTeamTotal();

    // create dynamic master list for team names
    teamList = new std::string [teamTotal];

    // populate team master list from db
    getTeamList();

    // create dynamic adjacency matrix (pointers array)
    adj = new double* [teamTotal];

    // create columns for adjacency matrix
    for(int i = 0; i < teamTotal; i++)
    {
        // creates the columns in the 2d dynamic array
        adj[i] = new double[teamTotal];
    }

    // initialize adjacenency matrix to 9999.9
    for(int i = 0; i < teamTotal; i++)
    {
        for(int j = 0; j < teamTotal; j++)
        {
            adj[i][j] = 9999.9;
        }
    }

    // populate adjacency matrix (add edges)
    createMatrix();

    // Test the matrix
/*
    // initialize adjacenency matrix to 9999.9
    for(int i = 0; i < teamTotal; i++)
    {
        for(int j = 0; j < teamTotal; j++)
        {
            qDebug() << i << " " << j << " " << adj[i][j];
        }
    }
*/
    //populate team and distance lists, call optimized sort and then display sorted array in qlist widget.
    populateList();

}


vPackageB::~vPackageB()
{
    delete ui;
}

void vPackageB::on_cancelButton_clicked()
{
    this->close();
}

void vPackageB::on_startButton_clicked()
{

    simulationWindow = new vSimulation(sortedTeams, sortedDistance, teamTotal);
    simulationWindow->show();
    this->close();
}

/****************************************************************************
 * getTeamTotal
 * --------------------------------------------------------------------------
 * Gets the total number of distinct beginning team names from distance db.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      myDB is is open and connected
 *
 * POST-CONDITIONS
 *      ==> Nothing.
 ***************************************************************************/

void vPackageB::getTeamTotal()
{

    // Create new database query
    QSqlQuery * qry = new QSqlQuery(myDB);

    // Set up the query to create ordered list of teams
    qry->prepare("SELECT COUNT (DISTINCT `Beg Team`) "
                 "FROM Distances");

    // Execute query if valid
    if(qry->exec())
    {
        qry->next();
        // get total team number
        teamTotal= qry->value(0).toInt();
        //qDebug()<<"TEAM TOTAL" <<teamTotal;
    }
    else
    {
        qDebug() << ("Package B getTeamTotal Error: qry failed.");
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

void vPackageB::getTeamList()
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
        qDebug() << ("Package B getTeamList Error: qry failed.");
    }

    /*//Testing Team List

    for(int i=0;i<30;i++)
    {
        std::cout << teamList[i];
    }
    */

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

void vPackageB::createMatrix()
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
            while(!isFound && sIndex < teamTotal)
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
            while(!isFound && dIndex < teamTotal)
            {
                // check if there is a match
                if(teamList[dIndex] == vTeam.toStdString())
                    isFound = true;
                else
                    dIndex++;
            }

            distance = qry->value(3).toDouble();

            // Add weighted edge into adjacency matrix
            adj[sIndex][dIndex] = distance;
        }
    }
    else
    {
        qDebug() << ("Package B createMatrix: qry failed.");
    }

}

/****************************************************************************
 * doDijkstras
 * --------------------------------------------------------------------------
 * Completes dijkstras algorithm, storing lowest distances, visited vertices
 * and parantage.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      myDB is is open and connected
 *
 * POST-CONDITIONS
 *      ==> Parent array updated
 ***************************************************************************/

void vPackageB::doDijkstras()
{
    double *lowDist; // The output array. dist[i] will hold the
                                  // shortest distance from src to i

    bool *cloudT; // cloudT[i] will be true if vertex i is included in shortest
                            // path tree or shortest distance from src to i is finalized

    // create dynamic variable arrays
    lowDist = new double [teamTotal];
    cloudT = new bool [teamTotal];
    parentArr = new int [teamTotal];

    // Initialize all distances as INFINITE and cloudT[] as false
    for(int i = 0; i < teamTotal; i++)
    {
        lowDist[i] = 9999.9;
        cloudT[i] = false;
    }

    // Initializing variables for starting team index
    lowDist[sTeamIndex] = 0; // set lowest distance starting index to 0
    parentArr[0] = 0; // set starting index to 0

    // Find shortest path for all vertices
    for(int count = 0; count < teamTotal; count++)
    {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to cloudT in the first iteration.
        int u = minDistance(lowDist, cloudT);

        // Mark the picked vertex as processed
        cloudT[u] = true;

        // Update dist value of the adjacent vertices of the picked vertex.
        for(int v = 0; v < teamTotal; v++)
        {
            // Update lowDist[v] only if is not in cloudT, there is an edge from
            // u to v, and total weight of path from src to v through u is
            // smaller than current value of dist[v]
            if(!cloudT[v] && (adj[u][v] < 9000) && (lowDist[u] + adj[u][v] < lowDist[v]))
            {
                lowDist[v] = lowDist[u] + adj[u][v];
                parentArr[v] = u;//storing the parent u for child v
            }
        }
    }

    delete [] lowDist;
    delete [] cloudT;

}

/****************************************************************************
 * minDistance
 * --------------------------------------------------------------------------
 * A utility function to find the vertex with minimum distance value, from
 * the set of vertices not yet included in shortest path tree
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      NONE
 *
 * POST-CONDITIONS
 *      ==> Nothing.
 ***************************************************************************/

int vPackageB::minDistance(double lowDist[], bool cloudT[])
{
    // Initialize min value
    double min = 9000; // it's over 9000!!!
    int minIndex = 0;

    for(int v = 0; v < teamTotal; v++)
        if(cloudT[v] == false && lowDist[v] < min)
        {
            min = lowDist[v];
            minIndex = v;
        }

    return minIndex;

}

/****************************************************************************
 * sortTeamOptimize
 * --------------------------------------------------------------------------
 * Uses dijkstra's algorithm to create a dynamic array for team order
 * and distance traveled using customList plus startTeam.  This is the
 * OPTIMIZED PACKAGE.  Shortest distance, not fan choice.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      adj[][] : adjacency matrix was already created and has data
 *      sTeamIndex: index of starting node.
 *      teamNum: total number of teams in sortedTeams array.
 *      sortedTeams[]: list of teams with index 0 being the start team
 *      sortedDistance[]: will have the transitive wt of each team in sortedTeams[]
 *
 * POST-CONDITIONS
 *      ==> Creates sorted dynamic arrays for teams and destinations.
 ***************************************************************************/
void vPackageB::sortTeamOptimize()
{
// Variables used for the below FOR loop
    bool   isFound;       // check if index of next in sorted Teams is found
    bool   isEnd;         // check if v -> u is done
    double tempDistance;  // store total distance between u and v
    int    nextIndex;     // Used to iterate through index from v to u
    QString tempStr;      // Used to store string for swap sort
    int cToPIndex; // child to parent index for parentage

    // vector for storing unsorted children of sTeamIndex for sorting
    std::vector<std::pair<int, double>> unsortedChildren;

    // Index 0 is the first starting team index to doDijkstra with
    // For index 0 to size-1, sort the teams into proper positions and get transitive wt
    // size-1 for condition because the last item in the array will be in its final position
    for(int i = 0; i < teamTotal-1; i++)
    {
        sTeamIndex = 0;

        // find start team index
        isFound = false;
        while(!isFound)
        {
            if(teamList[sTeamIndex] == sortedTeams[i].toStdString())
                isFound = true;
            else
                sTeamIndex++;
        }

//        qDebug() << "Starting Team: " << QString::fromStdString(teamList[sTeamIndex]) << " : index = " << sTeamIndex;

        // GENERATE DIJKSTRA'S using sTeamIndex and adjacency matrix
        doDijkstras();

        // Populate the unsortedChildren vector then sort by weight to find
        // the shortest distance to sTeamIndex
        for(int j = i+1; j < teamTotal; j++)
        {
            tempDistance = 0.0;
            nextIndex    = 0;
            isEnd        = false;
            isFound = false;

            // find nextIndex of string in sortedTeams index j
            while(!isFound)
            {
//                qDebug() << "while is found: " << sortedTeams[j] << " = " << QString::fromStdString(teamList[nextIndex]);
                if(teamList[nextIndex] == sortedTeams[j].toStdString())
                    isFound = true;
                else
                    nextIndex++;
            }

            // Get total distance from nextIndex -> sTeamIndex
            // Use parentArr to go up the parentage to sTeamIndex
            cToPIndex = nextIndex; // child to parent index for parentage
            while(!isEnd)
            {
//                qDebug() << "from " << QString::fromStdString(teamList[cToPIndex]) << "to parent " << QString::fromStdString(teamList[parentArr[cToPIndex]]) << "distance is " << adj[parentArr[cToPIndex]][cToPIndex];
                tempDistance = tempDistance + adj[parentArr[cToPIndex]][cToPIndex];

                // Check if nextIndex parent is same as the sTeamIndex
                // If same, stop looping
                // Else, set nextIndex as the new sTeamIndex
                if(sTeamIndex == parentArr[cToPIndex])
                    isEnd = true;
                else
                    cToPIndex = parentArr[cToPIndex];
            }

            // Add child to vector
            unsortedChildren.push_back(std::make_pair(nextIndex,tempDistance));
        } // for j loop

        // sort the children index of sTeamIndex (parent) by the smallest weighted child
        sort(unsortedChildren.begin(), unsortedChildren.end(),
             [](std::pair<int, double> const &a,
                std::pair<int, double> &b)
                {
                    return (a.second < b.second);
                });

        // nextIndex is the closest team to sTeamIndex
        nextIndex = unsortedChildren[0].first;

        //Do the swap (move nextIndex to sortedTeams[i+1]
        isFound = false;
        int k = i+1; // used to find index location of nextIndex in sortedTeams
        while(!isFound)
        {
            if(QString::fromStdString(teamList[nextIndex]) == sortedTeams[k])
            {
                // Swap time
                tempStr = sortedTeams[k];
                sortedTeams[k] = sortedTeams[i+1];
                sortedTeams[i+1] = tempStr;

                isFound = true;
            }
            else if (k >= teamTotal)
            {
                qDebug() << "wow, we didn't find it?";
            }
            else
                k++;
        }

        // Store unsortedChildren[0].second into sortedDistance[i+1]
        sortedDistance[i+1] = unsortedChildren[0].second;

        // Clears the unsortedChild vector
        unsortedChildren.clear();
    } // for i loop
}

void vPackageB::populateList()
{
    /************************************************************************
        * PROCESS: Store selected teams in order into selectedList.
        *          Find total number of teams in customList. (teamNum)
        *          Stores startTeam from selected option in combobox.
        *          Find master list index for the startTeam. (sTeamIndex)
        ***********************************************************************/
       // Stores selected items from teamList into customList

       ui->sortedTeamList->clear();
       /*
       selectionList = ui->teamList->selectedItems();
       teamNum = customList.count();
       teamNum++; // Need to add 1 to count the startTeam
       startTeam = ui->startTeam->currentText();
       */

       // Initialize variables for finding the start team index in master list
       bool isFound = false;
       sTeamIndex = 0;
       // find start team index
       while(!isFound)
       {
           if(teamList[sTeamIndex] == startTeam.toStdString())
               isFound = true;
           else
               sTeamIndex++;
       }

       /************************************************************************
        * PROCESS: Create dynamic arrays for team names (sortedTeams) and
        *          distances (sortedDistance).
        *          Copy customList (fan's selected teams) into sortedTeams,
        *          with startTeam being index 0.
        ***********************************************************************/
       // dynamic arrays that need are sorted results of fan selection
       // that will be passed to vSimulation
       sortedTeams = new QString[teamTotal]; // new object for dynamic
                                           // team array
       sortedDistance = new double[teamTotal];  // new object for dynamic
                                              // distance array

       // Put startTeam at index 0
       sortedTeams[0] = startTeam;
       sortedDistance[0] = 0;

       /*
       // Copy teams from customList to dynamic array (sortedTeams)
       for(int i = 1; i < teamNum; i++)
       {
           // customList needs i-1 because it starts at 0, but the iteration
           // starts at 1 because [0] is where startTeam was placed.
           sortedTeams[i] = customList.at(i-1)->text();
       }
       */

       QSqlQuery * qry = new QSqlQuery(myDB);
       int i = 1;

          qry->prepare("SELECT DISTINCT `Beg Team` "
                       "FROM Distances "
                       "WHERE `Beg Team` != '"+startTeam+"'"
                       "ORDER BY `Beg Team` COLLATE NOCASE ASC ");

          // Execute query if valid
          if(qry->exec())
          {
              // Populating list from query

              while(qry->next())
              {
                  sortedTeams[i] = qry->value(0).toString();
                  i++; //iterate
              }


          }
          else
          {
              qDebug() << ("vpackageb Error: qry failed.");
          }

       //call the sort

           sortTeamOptimize();

       /************************************************************************
        * OUTPUT: Display ordered result of the sortedTeams and total distance.
        ***********************************************************************/
       double totalDistance = 0.0;

       // Load all selected, sorted teams into list view
       for(int i = 0; i < teamTotal; i++)
       {
           ui->sortedTeamList->addItem(sortedTeams[i]);
           totalDistance = totalDistance + sortedDistance[i];
       }

       ui->distanceLine->setText(QString::number(totalDistance, 'f', 2));
       ui->sortedGroup->setEnabled(true);
}






