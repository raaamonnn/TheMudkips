#include "vmst.h"
#include "ui_vmst.h"

vMST::vMST(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vMST)
{
    ui->setupUi(this);
    getNumTeams();
    // create dynamic master list for team names
    teamList = new std::string [numTeams];
    getTeamList();

    // create dynamic adjacency matrix (pointers array)
    adj = new float* [numTeams];
    set = new int [numTeams]; //initialize dynamic set of team nums for kruskals
    included = new bool [numTeams]; //create dynamic array to store included values

    // create columns for adjacency matrix
    for(int i = 0; i < numTeams; i++)
    {
        // creates the columns in the 2d dynamic array
        adj[i] = new float[numTeams];
    }


    // initialize adjacenency matrix to
    for(int i = 0; i < numTeams; i++)
    {
        for(int j = 0; j < numTeams; j++)
        {
            adj[i][j] = -1;
        }
    }

    for(int i = 0; i<numTeams; i++) //initialize set
    {
        set[i] = -1;
    }


    for(int i=0; i<numTeams; i++) //initialize included
    {
        included[i] = false;
    }



   createMatrix();

  Kruskals(); //calling kruskals

}

vMST::~vMST()
{
    delete ui;
}

void vMST::getNumTeams()
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
       // qDebug() <<"NUMTEAMS: " << numTeams;
    }
    else
    {
        qDebug() << ("MST getNumTeams Error: qry failed.");
    }
}

void vMST::getTeamList()
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
            //qDebug()<<"TEAM" << uTeam;
        }
    }
    else
    {
        qDebug() << ("MST getTeamList Error: qry failed.");
    }

}

void vMST::createMatrix()
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
        qDebug() << ("MST createMatrix: qry failed.");
    }
}

void vMST::Kruskals()
{

    //create two vectors holding tuples start dest, end dest and weight
    std::vector<std::tuple<int,int,float>> temp;
    std::vector<std::tuple<int,int,float>> mst;

    int u, v;
    float wt = 0; //u start, v ending dest wt weight
    int i=0; //loop control varaible for while loop to determine MST edges
    int k=0; //index for our edge list

    float tDistance = 0;

    //int t[2][V];

    for(int i = 0; i<numTeams; i++) //initialize set
    {
        set[i] = -1;
    }

    for(int i=0; i<numTeams; i++) //initialize included
    {
        included[i] = false;
    }

    for(int i =0; i<numTeams;i++)
    {
        for(int j =i; j<numTeams; j++)
        {

            if(adj[i][j] != -1) //made a change 12-8-19 2:43pm
            {
                temp.push_back(std::make_tuple(i, j, adj[i][j])); //graph weight at i j

            }


        }
    }

     //lambda sort
    sort(temp.begin(), temp.end(),
     [](std::tuple<int, int, float> const &a,
        std::tuple<int, int, float> &b)
        {
            return (std::get<2>(a) < std::get<2>(b));
        });



    while(i < numTeams-1)
    {
        for(auto it = temp.begin(); it!=temp.end(); ++it)
        {
            u = std::get<0>(*it); // start city vertex
            v = std::get<1>(*it); // end city vertex
            wt = std::get<2>(*it); // edge weight

            if(find(u)!= find(v))
            {
                //t[0][i]=u;
                //t[1][i]=v;
                mst.push_back(std::make_tuple(u, v, wt));
                tDistance += wt;
                //cout << endl << "FIND (U)" << find(u) << "  " << "FIND (V)"<< find(v) << endl << endl;
                join(find(u),find(v));
                included[k]=1;
                i++;
                // printf("%d %d %d %d\n",u,v,find(u),find(v));
            }
            else
            {
                included[k]=1;
            }


        }

    }

    std::string tempPrint;
    QString tempStr;
    QString weight;

    for(auto it = mst.begin(); it!=mst.end(); ++it)
            {
                u = std::get<0>(*it); // start city vertex
                v = std::get<1>(*it); // end city vertex
                wt = std::get<2>(*it); // edge weight

                weight.setNum(wt);

                tempPrint = (teamList[u] + " <-> " + teamList[v] + " = " + weight.toStdString());
                //std::cout << "temp Print" << tempPrint;

                tempStr = QString::fromStdString(tempPrint);
                //ui->mstList->addItem(tempPrint);
                ui->mstList->addItem(tempStr);
                //std::cout << "\tAdding Edge: " << teamList[u] << " <-> " << teamList[v];
                //std::cout << " " << wt << " " << std::endl << std::endl;

            }

        //std::cout << endl << "THE TOTAL DISTANCE IS: " << tDistance;
        ui->distanceLine->setText(QString::number(tDistance));
}


//MST join function
void vMST::join(int u,int v)
{

    if(set[u]<set[v])
    {
        set[u]+=set[v];
        set[v]=u;
    }

    else
    {
        set[v]+=set[u];
        set[u]=v;
    }

}

//MST FIND FUNCTIOn
int vMST::find(int u)
{
    int x=u,v=0;
    while(set[x]>0)
    {
        x=set[x];
    }
    while(u!=x)
    {
        v=set[u];
        set[u]=x;
        u=v;
    }

    return x;
}
