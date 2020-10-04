#include "ateam.h"
#include<addteam.h>
#include "ui_ateam.h"
#include<QDebug>
aTeam::aTeam(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::aTeam)
{
    ui->setupUi(this);
    myDb=QSqlDatabase::database();

    QSqlQuery* query = new QSqlQuery(myDb);
    QSqlQueryModel* model = new QSqlQueryModel;
    query->prepare("select `Team Name` from 'Team Info'");
    if(query->exec()){std::cout<<"query executed";}
    else{std::cout<<"query failed";}
    model->setQuery(*query);
    ui->comboBox->setModel(model);

    QSqlQuery* qry = new QSqlQuery(myDb);
    QSqlQueryModel* mdl = new QSqlQueryModel;
    qry->prepare("select * from 'Team Info'");
    if(qry->exec()){std::cout<<"Tabel loaded";}
    else{std::cout<<"Table failed to load";}
    mdl->setQuery(*qry);
    ui->tableView->setModel(mdl);
}

aTeam::~aTeam()
{
    delete ui;

}


void aTeam::on_comboBox_currentIndexChanged(const QString &arg1) //loads team data into line_edits
{
    QString name = ui->comboBox->currentText();
    QSqlQuery* qry = new QSqlQuery(myDb);

    qry->prepare("select * from 'Team Info' where `Team Name` = '"+name+"'");
    qry->exec();
    if(qry->exec()){

    while(qry->next())
    {
        ui->lineEdit->setText(qry->value(0).toString());
        ui->lineEdit_2->setText(qry->value(1).toString());
        ui->lineEdit_3->setText(qry->value(2).toString());
        ui->lineEdit_4->setText(qry->value(3).toString());
        ui->lineEdit_5->setText(qry->value(4).toString());
        ui->lineEdit_6->setText(qry->value(5).toString());
        ui->lineEdit_7->setText(qry->value(6).toString());
        ui->lineEdit_8->setText(qry->value(7).toString());
    }
    }
    else{
        std::cout << "failed to connect";
    }
}

void aTeam::on_pushButton_clicked() //update the table
{
    QString name,coach,division,year,arena,conference,location,stadiumcap;
    conference = ui->lineEdit->text();
    division = ui->lineEdit_2->text();
    name = ui->lineEdit_3->text();
    location = ui->lineEdit_4->text();
    arena = ui->lineEdit_5->text();
    stadiumcap = ui->lineEdit_6->text();
    year = ui->lineEdit_7->text();
    coach = ui->lineEdit_8->text();

    QSqlQuery* query = new QSqlQuery(myDb);
    query->prepare("update 'Team Info' set `Team Name` = '"+name+"', Conference ='"+conference+"', Division='"+division+"', `Team Name`='"+name+"' , Location='"+location+"', `Arena Name`='"+arena+"', `Stadium Capacity`='"+stadiumcap+"', `Joined League`='"+year+"', Coach='"+coach+"' where `Team Name` = '"+name+"'");
            if(query->exec()){
        std::cout << "updated succesfully";

        QSqlQuery* qry = new QSqlQuery(myDb);
        QSqlQueryModel* mdl = new QSqlQueryModel;
        qry->prepare("select * from 'Team Info'");
        if(qry->exec()){std::cout<<"Tabel loaded";}
        else{std::cout<<"Table failed to load";}
        mdl->setQuery(*qry);
        ui->tableView->setModel(mdl);

        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit_4->clear();
        ui->lineEdit_5->clear();
        ui->lineEdit_6->clear();
        ui->lineEdit_7->clear();
        ui->lineEdit_8->clear();
            }
    else{std::cout<<"failed to update";}
}

void aTeam::on_pushButton_3_clicked() //Delete a team
{
    QString name = ui->comboBox->currentText();
    QString variable = name + " Store";
    QSqlQuery* query = new QSqlQuery(myDb);
    query->prepare("delete from 'Team Info' where `Team Name` = '"+name+"'");
    if(query->exec())
    {
        std::cout << "deleted succesfully";

        QSqlQuery* qry = new QSqlQuery(myDb);
        QSqlQueryModel* mdl = new QSqlQueryModel;
        qry->prepare("select * from 'Team Info'");
        if(qry->exec()){std::cout<<"Tabel loaded";}
        else{std::cout<<"Table failed to load";}
        mdl->setQuery(*qry);
        ui->tableView->setModel(mdl);

        qry->clear();
        std::string test = variable.toStdString();
        std::cout << std::endl << test;
        qry->prepare("delete ''"+variable+"''");
    }
    else {std::cout<<"failed to delete";}
}



void aTeam::on_pushButton_2_clicked() //add a team
{

        //---------------------------------------------------------
        // ***1)and 2)read the both new cities and their distances***
        //---------------------------------------------------------
        auto excel     = new QAxObject("Excel.Application");
        auto workbooks = excel->querySubObject("Workbooks");
        auto workbook  = workbooks->querySubObject("Open(const QString&)","C:\\Users\\Juli\\Documents\\CS1DProject2\\TheMudkips\\files\\NBA Information");
//        auto workbook  = workbooks->querySubObject("Open(const QString&)","C:\\Users\\mrpou\\Desktop\\CS1D Project 2\\CS 1D Fall 2019 Basketball Project-4\\NBA Information"); //WHERE EXCEL FILE IS CHANGE HERE****
        auto sheets    = workbook->querySubObject("Worksheets");
        auto sheet     = sheets->querySubObject("Item(int)", 1);    // use first worksheet
        qDebug()<<"made it past the setup stage";
        //------------ DOES TEAM INFO

        for (int r = 34; (r <= 34); ++r) //GRABS EACH CELL FROM THESE ROWS
        {
            qDebug()<<"very begining";
            auto cCell = sheet->querySubObject("Cells(string,string,string,string,string,string,string,string)",r,1);
            QString conference = cCell->dynamicCall("Value()").toString();
            qDebug()<<conference;
                 cCell = sheet->querySubObject("Cells(string,string,string,string,string,string,string,string)",r,2);
            QString division = cCell->dynamicCall("Value()").toString();
            qDebug()<<division;
                 cCell = sheet->querySubObject("Cells(string,string,string,string,string,string,string,string)",r,3);
            QString teamName = cCell->dynamicCall("Value()").toString();
            qDebug()<<teamName;
                cCell = sheet->querySubObject("Cells(string,string,string,string,string,string,string,string)",r,4);
            QString location = cCell->dynamicCall("Value()").toString();
            qDebug()<<location;
                cCell = sheet->querySubObject("Cells(string,string,string,string,string,string,string,string)",r,5);
            QString arenaName = cCell->dynamicCall("Value()").toString();
            qDebug()<<arenaName;
                cCell = sheet->querySubObject("Cells(string,string,string,string,string,string,string,string)",r,6);
            QString stadiumCapacity = cCell->dynamicCall("Value()").toString();
            qDebug()<<stadiumCapacity;
            cCell = sheet->querySubObject("Cells(string,string,string,string,string,string,string,string)",r,7);
            QString joinedLeague = cCell->dynamicCall("Value()").toString();
            qDebug()<<joinedLeague;
            cCell = sheet->querySubObject("Cells(string,string,string,string,string,string,string,string)",r,8);
             QString coach = cCell->dynamicCall("Value()").toString();
             qDebug()<<coach;



            QSqlQuery* query = new QSqlQuery(myDb);
            query->prepare("insert into 'Team Info' (`Conference`, `Division`, `Team Name`, `Location`, `Arena Name`, `Stadium Capacity`, `Joined League`, `Coach`) values ('"+conference+"','"+division+"','"+teamName+"','"+location+"','"+arenaName+"','"+stadiumCapacity+"','"+joinedLeague+"','"+coach+"')"); //for some reason this does not add to the database
            if(query->exec()){
                qDebug()<<"Changes Made PART 1";

            }
            else{qDebug()<<"Changes failed PART 1";
            query->lastError();}

        }
        qDebug()<<"made it past team info shit";
        excel->dynamicCall("Quit()");
        delete excel;

        //-------------------- DOES DISTANCES

        auto excel2     = new QAxObject("Excel.Application");
        auto workbooks2 = excel2->querySubObject("Workbooks");

        auto workbook2  = workbooks2->querySubObject("Open(const QString&)","C:\\Users\\Juli\\Documents\\CS1DProject2\\TheMudkips\\files\\NBA Distances");
//        auto workbook2  = workbooks2->querySubObject("Open(const QString&)","C:\\Users\\mrpou\\Desktop\\CS1D Project 2\\CS 1D Fall 2019 Basketball Project-4\\NBA Distances"); //WHERE EXCEL FILE IS CHANGE HERE****
        auto sheets2   = workbook2->querySubObject("Worksheets");
        auto sheet2     = sheets2->querySubObject("Item(int)", 2);

        for (int r = 2; (r <= 5); ++r) //GRABS EACH CELL FROM THESE ROWS
        {
            auto cCell = sheet2->querySubObject("Cells(string,string,string,string)",r,1);
            QString begTeamName = cCell->dynamicCall("Value()").toString();
                 cCell = sheet2->querySubObject("Cells(string,string,string,string)",r,2);
            QString begArena = cCell->dynamicCall("Value()").toString();
                 cCell = sheet2->querySubObject("Cells(string,string,string,string)",r,3);
            QString endTeamName = cCell->dynamicCall("Value()").toString();
                cCell = sheet2->querySubObject("Cells(string,string,string,string)",r,4);
            QString distances = cCell->dynamicCall("Value()").toString();


            QSqlQuery* query = new QSqlQuery(myDb);
            query->prepare("insert into 'Distances' (`Beg Team`, `Beg Arena`, `End Team`, `Distance`) values ('"+begTeamName+"','"+begArena+"','"+endTeamName+"','"+distances+"')"); //for some reason this does not add to the database
            if(query->exec()){
                qDebug()<<"Changes Made PART 2";
            }
            else{qDebug()<<"Changes failed PART 2";
            query->lastError();}
        }
        excel2->dynamicCall("Quit()");
        delete excel2;

        //----------------------- DOES SHOP

        if(true)
        {
            QSqlQuery* query = new QSqlQuery(myDb);
            query->prepare("CREATE TABLE 'Seattle Supersonics Shop' (item TEXT, price INTEGER)");
            if(query->exec()){
                qDebug()<<"Changes Made";

            }
            else{qDebug()<<"Changes failed";
            query->lastError();}


            QString items[] = {"Autographed Basketball","Team Pennant","Team Pennant","Team Jersey" };
            double prices[] = {49.89,17.99,29.99,179.79};

            for (int r = 0; (r < 4); ++r)
            {
                QSqlQuery query;
                query.prepare("insert into 'Seattle Supersonics Shop' (item, price) values ('"+items[r]+"','"+QString::number(prices[r])+"')");

                if(query.exec()){
                    qDebug()<<"Changes Made Part 3";
                }
                else{qDebug()<<"Changes failed Part 3";
                query.lastError();}
            }
        }
        //========refresh table=============================
        QSqlQuery* query = new QSqlQuery(myDb);
        QSqlQueryModel* model = new QSqlQueryModel;
        query->prepare("select `Team Name` from 'Team Info'");
        if(query->exec()){std::cout<<"query executed";}
        else{std::cout<<"query failed";}
        model->setQuery(*query);
        ui->comboBox->setModel(model);

        QSqlQuery* qry = new QSqlQuery(myDb);
        QSqlQueryModel* mdl = new QSqlQueryModel;
        qry->prepare("select * from 'Team Info'");
        if(qry->exec()){std::cout<<"Tabel loaded";}
        else{std::cout<<"Table failed to load";}
        mdl->setQuery(*qry);
        ui->tableView->setModel(mdl);
        //===================================================

    }

