#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "databasemanager.h"
#include "admin.h"
#include "vacation.h"
#include "nbainfo.h"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    void connectToDB();
    //!< Gets the connection to database

private slots:
    void on_nbaInfoButton_clicked();
    //!< Opens NBA information window

    void on_loginButton_clicked();
    //!< Checks for valid login name and password
    /*!< If valid, opens the window the login is valid for. */

    void on_exitProgramButton_clicked();
    //!< Closes database connection and closes login window

    void on_usernameLineEdit_returnPressed();
    //!< Checks for valid login name and password
    /*!< If valid, opens the window the login is valid for. */

    void on_passwordLineEdit_returnPressed();
    //!< Checks for valid login name and password
    /*!< If valid, opens the window the login is valid for. */

private:
    Ui::Login *ui;              //!< Login window object
    databaseManager myDB;       //!< Database connection object
    Vacation *vacationWindow;   //!< Vacation window object
    Admin   *adminWindow;       //!< Admin window object
    NBAInfo *nbaWindow;         //!< NBA window object
};

#endif // LOGIN_H
