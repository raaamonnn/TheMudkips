#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

/****************************************************************************
 * SQLite FILENAME: NBA.db
 * --------------------------------------------------------------------------
 * TABLES:
 *      Distances
 *          Beg Team : Beginning Team (for distance)
 *          Beg Arena: Beginning Team's Arena
 *          End Team : Ending Team (for distance)
 *          Distance : The distance between the two teams
 *      Souvenirs
 *          Team Name
 *          Autographed Basketball
 *          Team Pennant
 *          Team Picture
 *          Team Jersey
 *      Team Info
 *          Conference      : Team's confernce
 *          Division        : Team's division
 *          Team Name       : Team's name
 *          Location        : Location of the team (city, state)
 *          Arena Name      : Team's arena
 *          Stadium Capacity: The number of seats in the stadium
 *          Joined League   : Date team joined the league
 *          Coach           : Team's coach
 ***************************************************************************/
class databaseManager : public QObject
{
public:
    explicit databaseManager();
    ~databaseManager();

    bool openDB();
    //!< Creates and opens a database connection if not open
    /*!< POSTCONDITONS: return database.open(); */

    bool closeDB();
    //!< Closes database connection
    /*!< POSTCONDITONS: return database.isOpen() */

    QSqlError lastError();
    //!< Returns last database related error
    /*!< POSTCONDITONS: return database.lastError(); */

    QSqlDatabase currentDB() const;
    //!< Returns connected database object
    /*!< POSTCONDITONS: return database */

private:
    QSqlDatabase database; //!< SQL Database Object created for connection
};

#endif // DATABASEMANAGER_H
