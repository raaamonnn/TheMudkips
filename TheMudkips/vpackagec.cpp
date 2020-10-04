#include "vpackagec.h"
#include "ui_vpackagec.h"

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
vPackageC::vPackageC(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vPackageC)
{
    ui->setupUi(this);

    // Get database connection
    myDB = QSqlDatabase::database();

    // Display default combobox view which also updates the left list view
    defaultComboBoxView();

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

    // Set right side to disabled
    ui->sortedGroup->setEnabled(false);
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
vPackageC::~vPackageC()
{
    delete ui;
}

/****************************************************************************
 * defaultListView
 * --------------------------------------------------------------------------
 * Loads all teams from database query into ListWidget, except the startTeam.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      database is open
 *
 * POST-CONDITIONS
 *      ==> Displays teams into List Widget (teamList) on left side.
 ***************************************************************************/
void vPackageC::defaultListView()
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
            teamName = qry->value(0).toString();
            ui->teamList->addItem(teamName);
        }
    }
    else
    {
        qDebug() << ("vpackagec Error: qry failed.");
    }
}

/****************************************************************************
 * defaultComboBoxView
 * --------------------------------------------------------------------------
 * Query of all teams is displayed in the combo box.
 * --------------------------------------------------------------------------
 * POST-CONDITIONS
 *      ==> Displays combobox with all team names.
 ***************************************************************************/
void vPackageC::defaultComboBoxView()
{
    QSqlQuery * qry1 = new QSqlQuery(myDB);
    QSqlQueryModel * combo = new QSqlQueryModel();
    qry1->prepare("SELECT DISTINCT `Beg Team` "
                  "FROM Distances "
                  "ORDER BY `Beg Team` ASC ");
    qry1->exec();

    combo->setQuery(*qry1);

    ui->startTeam->setModel(combo);
    teamName = ui->startTeam->currentText();
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
void vPackageC::getTeamTotal()
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
        qDebug() << ("Package C getTeamTotal Error: qry failed.");
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
void vPackageC::getTeamList()
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
        qDebug() << ("Package C getTeamList Error: qry failed.");
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
void vPackageC::createMatrix()
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
        qDebug() << ("Package C createMatrix: qry failed.");
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
void vPackageC::doDijkstras()
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
int vPackageC::minDistance(double lowDist[], bool cloudT[])
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
void vPackageC::sortTeamNormal()
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
// Display dynamic sorted array that will be passed in list
void vPackageC::sortTeamOptimize()
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
    for(int i = 0; i < teamNum-1; i++)
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
        for(int j = i+1; j < teamNum; j++)
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
            else if (k >= teamNum)
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

/****************************************************************************
 * on_cancelButton_clicked
 * --------------------------------------------------------------------------
 * Closes this window.
 * --------------------------------------------------------------------------
 * POST-CONDITIONS
 *      ==> Closes this window.
 ***************************************************************************/
void vPackageC::on_cancelButton_clicked()
{
    this->close();
}

/****************************************************************************
 * on_startButton_clicked
 * --------------------------------------------------------------------------
 * Closes this window and opens the simulation window.
 * --------------------------------------------------------------------------
 * POST-CONDITIONS
 *      ==> Closes this window and opens simulation window.
 ***************************************************************************/
void vPackageC::on_startButton_clicked()
{
    simulationWindow = new vSimulation(sortedTeams, sortedDistance, teamNum);
    simulationWindow->show();

    this->close();
}

/****************************************************************************
 * on_startTeam_currentIndexChanged
 * --------------------------------------------------------------------------
 * Updates startTeam to the selected item in the combobox and update
 * teamList with all team names, excluding the startTeam.
 * --------------------------------------------------------------------------
 * POST-CONDITIONS
 *      ==> Updates teamList view.
 ***************************************************************************/
void vPackageC::on_startTeam_currentIndexChanged(const QString &arg1)
{
    startTeam = arg1;
    ui->teamList->clear();
    defaultListView();
}

/****************************************************************************
 * on_confirmButton_clicked
 * --------------------------------------------------------------------------
 * Stores selected teams into an array list.  Get teamNum and set
 * startTeam.  Use sorting algorithm in sortTeamList to get dynamic
 * arrays.  And display the results in the dynamic arrays.
 * --------------------------------------------------------------------------
 * POST-CONDITIONS
 *      ==> Displays resulting dynamic arrays on right side of window.
 ***************************************************************************/
void vPackageC::on_confirmButton_clicked()
{
    /************************************************************************
     * PROCESS: Store selected teams in order into customList.
     *          Find total number of teams in customList. (teamNum)
     *          Stores startTeam from selected option in combobox.
     *          Find master list index for the startTeam. (sTeamIndex)
     ***********************************************************************/
    // Stores selected items from teamList into customList
    ui->sortedTeamList->clear();
    customList = ui->teamList->selectedItems();
    teamNum = customList.count();
    teamNum++; // Need to add 1 to count the startTeam
    startTeam = ui->startTeam->currentText();

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

    /************************************************************************
     * PROCESS: Do the sort type depending on optimization option selected.
     ***********************************************************************/
    // If checkbox is checked (true)
    if(ui->optimizeBox->isChecked())
    {
        qDebug() << "Optimization: true";
        sortTeamOptimize();
    }
    // If checkbox is unchecked (false)
    else
    {
        qDebug() << "Optimization: false";
        sortTeamNormal();
    }

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
