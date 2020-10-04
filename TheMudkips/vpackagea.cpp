#include "vpackagea.h"
#include "ui_vpackagea.h"

vPackageA::vPackageA(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vPackageA)
{
    ui->setupUi(this);

    // Get database connection
    myDB = QSqlDatabase::database();

    defaultPackage(); // call default view set up

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


    // Set right side to disabled
    ui->sortedGroup->setEnabled(false);
}

vPackageA::~vPackageA()
{
    delete ui;
}

void vPackageA::on_cancelButton_clicked()
{
    this->close();
}

void vPackageA::on_startButton_clicked()
{
    simulationWindow = new vSimulation(sortedTeams,sortedDistance,2);
    simulationWindow->show();

    this->close();
}


void vPackageA::defaultPackage()
{
    // Create new database query
    QSqlQuery * qry = new QSqlQuery(myDB);

    // Set up the query to create ordered list of teams, except startTeam
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
            tName = qry->value(0).toString();
            ui->teamList->addItem(tName);
        }
    }
    else
    {
        qDebug() << ("vpackagea Error: qry failed.");
    }
}


void vPackageA::sortTeam()
{
//    qDebug() << "size: " << teamNum;
//    QString temp; // used as temporary storage while swapping
    sortedTeams = new QString[teamNum]; // new object for dynamic
                                        // team array
    sortedDistance = new double[teamNum];  // new object for dynamic
                                        // distance array

    /************************************************************************
     * PROCESS: Copy contents of Selection List (fan's selected teams)
     *          into dynamic array.
     ***********************************************************************/
    // Put startTeam at index 0
    sortedTeams[0] = startTeam;
    sortedDistance[0] = 0;

    // Copy teams from Selection List to dynamic array (sortedTeams)
    for(int i = 1; i < teamNum; i++)
    {
        // Selection List needs i-1 because it starts at 0, but the iteration
        // starts at 1 because [0] is where startTeam was placed.
        sortedTeams[i] = customList.at(i-1)->text();
    }
}



void vPackageA::on_confirmButton_clicked()
{

    // Stores selected items from teamList into customList
    ui->sortedTeamList->clear();
    customList = ui->teamList->selectedItems();
    teamNum = customList.count();
    teamNum++; // Need to add 1 to count the startTeam

    sortTeam();

//    // Load all selected, sorted teams into list view
//    for(int i = 0; i < teamNum; i++)
//    {
//        ui->sortedTeamList->addItem(sortedTeams[i]);
//    }

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
    sortedTeams = new QString[teamNum]; // new object for dynamic
                                        // team array
    sortedDistance = new double[teamNum];  // new object for dynamic
                                           // distance array

    // Put startTeam at index 0
    sortedTeams[0] = startTeam;
    sortedDistance[0] = 0;

    // Copy teams from customList to dynamic array (sortedTeams)
    for(int i = 1; i < teamNum; i++)
    {
        // customList needs i-1 because it starts at 0, but the iteration
        // starts at 1 because [0] is where startTeam was placed.
        sortedTeams[i] = customList.at(i-1)->text();
    }

    sortTeamNormal();

    /************************************************************************
     * OUTPUT: Display ordered result of the sortedTeams and total distance.
     ***********************************************************************/
    double totalDistance = 0.0;

    // Load all selected, sorted teams into list view
    for(int i = 0; i < teamNum; i++)
    {
        ui->sortedTeamList->addItem(sortedTeams[i]);
        totalDistance = totalDistance + sortedDistance[i];
    }

    ui->distanceLine->setText(QString::number(totalDistance, 'f', 2));
    ui->sortedGroup->setEnabled(true);

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
void vPackageA::createMatrix()
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
        qDebug() << ("Package A createMatrix: qry failed.");
    }
}

/****************************************************************************
 * doDijkstras
 * --------------------------------------------------------------------------
 * Creates a subgraph of shortest distance using dijkstra's algorithm.
 * This is used to determine shortest distance algorithm for this package.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      myDB is is open and connected
 *      adj[][] : adjacency matrix was already created and has data
 *      sTeamIndex: index of starting node.
 *
 * POST-CONDITIONS
 *      ==> Nothing.
 ***************************************************************************/
void vPackageA::doDijkstras()
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
int vPackageA::minDistance(double lowDist[], bool cloudT[])
{
    // Initialize min value
    double min = 9000; // ensures it is below 9999.9 easier on julis eyes to be 9000/ peace of mind
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
 * sortTeamNormal
 * --------------------------------------------------------------------------
 * Uses dijkstra's algorithm to create a dynamic array for team order
 * and distance traveled using customList plus startTeam.  This is the
 * NORMAL PACKAGE.  Order is fan choice.
 * --------------------------------------------------------------------------
 * POST-CONDITIONS
 *      ==> Creates sorted dynamic arrays for teams and destinations.
 ***************************************************************************/
// Display dynamic sorted array that will be passed in list
void vPackageA::sortTeamNormal()
{
    // Variables used for the below FOR loop
    bool   isFound;       // check if index of next in sorted Teams is found
    bool   isEnd;         // check if v -> u is done
    double tempDistance;  // store total distance between u and v
    int    nextIndex;     // Used to iterate through index from v to u
    int    vIndex;        // Used to keep the v Index

    // Find transitive distance between each index in the sortedTeams
    // Store result in sortedDistances
    for(int i = 1; i < teamNum; i++)
    {
        // Initialize variables for each index change in sortedTeam
        isFound      = false;
        isEnd        = false;
        tempDistance = 0.0;
        nextIndex    = 0;

        // GENERATE DIJKSTRA'S using sTeamIndex and adjacency matrix
        doDijkstras();

        // Get masterlist index value for nextIndex in sortedTeams array
        while(!isFound)
        {
            if(teamList[nextIndex] == sortedTeams[i].toStdString())
                isFound = true;
            else
                nextIndex++;
        }

        vIndex = nextIndex;  // save the original v index

//        qDebug() << "Find transitive distance from " << QString::fromStdString(teamList[sTeamIndex]) << " to " << QString::fromStdString(teamList[nextIndex]);

        // Get total distance from nextIndex -> sTeamIndex
        // Use parentArr to go up the parentage to sTeamIndex
        while(!isEnd)
        {
//            qDebug() << "from " << QString::fromStdString(teamList[nextIndex]) << "to parent " << QString::fromStdString(teamList[parentArr[nextIndex]]) << "distance is " << adj[parentArr[nextIndex]][nextIndex];
            tempDistance = tempDistance + adj[parentArr[nextIndex]][nextIndex];

            // Check if nextIndex parent is same as the sTeamIndex
            // If same, stop looping
            // Else, set nextIndex as the new sTeamIndex
            if(sTeamIndex == parentArr[nextIndex])
                isEnd = true;
            else
                nextIndex = parentArr[nextIndex];
        }

        // Store transitive distance between u and v.
        sortedDistance[i] = tempDistance;

        sTeamIndex = vIndex; // set u = previous v
    }
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
void vPackageA::getTeamTotal()
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
        teamTotal= qry->value(0).toInt();
    }
    else
    {
        qDebug() << ("Package A getTeamTotal Error: qry failed.");
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
void vPackageA::getTeamList()
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
        qDebug() << ("Package A getTeamList Error: qry failed.");
    }
}
