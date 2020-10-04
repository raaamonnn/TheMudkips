#include "login.h"
#include "ui_login.h"
#include <QSound>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

/****************************************************************************
 * METHOD - connectToDB
 * --------------------------------------------------------------------------
 * This method creates and opens a database connection and displays a
 * qDebug message that the database access is open.  This method is used
 * with a valid login (traveler or admin).  If database is already
 * connected and open, then only the qDebug message is displayed.
 * NOTE: Use with valid login username/password only (traveler or admin)
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      No parameters are required.
 *
 * POST-CONDITIONS
 *      ==> Returns nothing.
 *      ==> Creates and connects to SQLite database if not open
 ***************************************************************************/
void Login::connectToDB()
{
    bool isOpen = false; // CALC - Set database open status to false

    // Create and open a database connection
    // Return if database opened successfully or not
    isOpen = myDB.openDB();

    // Display message if database connection was successful
    if(isOpen)
    {
        qDebug() << "Success: Able to access open database";
    }
}

/****************************************************************************
 * METHOD - on_nbaInfoButton_clicked
 * --------------------------------------------------------------------------
 * This method creates and opens a database connection and displays a
 * qDebug message that the database access is open.  This method is used
 * to open a window (no valid login required) to the NBA information window.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      No parameters are required.
 *
 * POST-CONDITIONS
 *      ==> Returns nothing.
 *      ==> Creates and connects to SQLite database if not open
 ***************************************************************************/
void Login::on_nbaInfoButton_clicked()
{
    connectToDB();
    nbaWindow = new NBAInfo();
    nbaWindow->show();
}

/****************************************************************************
 * METHOD - on_loginButton_clicked
 * --------------------------------------------------------------------------
 * This method creates and opens a database connection and displays a
 * qDebug message that the database access is open.  This method is used
 * to validate a login name and password.  If the combination is valid,
 * it opens the window that it is valid for.  If the login name and/or
 * password is invalid, a message is displayed that it was invalid.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      No parameters are required.
 *
 * POST-CONDITIONS
 *      ==> Returns nothing.
 *      ==> Creates and connects to SQLite database if not open
 ***************************************************************************/
void Login::on_loginButton_clicked()
{
    QString username;   //IN, CALC - username
    QString password;   //IN, CALC - password

    //sound testing begin

    QSound::play(":/images/mudkipSound.wav");

    //sound testing end

    username = ui->usernameLineEdit->text();
    password = ui->passwordLineEdit->text();

    // Swap below if statements when changing functionality
    //if(username == "fan" && password == "123")
    if(username == "fan" && password == "123")
    {
        ui->passwordLineEdit->setText("");
        ui->usernameLineEdit->setText("");

        ui->errorLabel->setText("");

        connectToDB();
        vacationWindow = new Vacation();
        vacationWindow->show();

    }
    // Swap below if statements when changing functionality
    //else if(username == "admin" && password == "456")
    else if(username == "admin" && password == "456")
    {
        ui->passwordLineEdit->setText("");
        ui->usernameLineEdit->setText("");

        ui->errorLabel->setText("");

        connectToDB();
        adminWindow = new Admin();
        adminWindow->show();
    }
    else
    {
        ui->errorLabel->setText("** Invalid Username or Password");
        ui->passwordLineEdit->setText("");
        ui->usernameLineEdit->setText("");
    }
}

/****************************************************************************
 * METHOD - on_exitProgramButton_clicked
 * --------------------------------------------------------------------------
 * This method closes database connection and displays a qDebug message
 * that the database access is closed.  This method is used
 * to close the login window (thus close the entire program).
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      No parameters are required.
 *
 * POST-CONDITIONS
 *      ==> Returns nothing.
 *      ==> Closes the connection to SQLite database if open
 ***************************************************************************/
void Login::on_exitProgramButton_clicked()
{
    myDB.closeDB();
    this->close();
}

/****************************************************************************
 * METHOD - on_usernameLineEdit_returnPressed
 * --------------------------------------------------------------------------
 * This method acts the same as a button push, but executes when the
 * enter key is pressed in the login name line.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      No parameters are required.
 *
 * POST-CONDITIONS
 *      ==> Returns nothing.
 ***************************************************************************/
void Login::on_usernameLineEdit_returnPressed()
{
    on_loginButton_clicked();
}

/****************************************************************************
 * METHOD - on_passwordLineEdit_returnPressed
 * --------------------------------------------------------------------------
 * This method acts the same as a button push, but executes when the
 * enter key is pressed in the password name line.
 * --------------------------------------------------------------------------
 * PRE-CONDITIONS
 *      No parameters are required.
 *
 * POST-CONDITIONS
 *      ==> Returns nothing.
 ***************************************************************************/
void Login::on_passwordLineEdit_returnPressed()
{
    on_loginButton_clicked();
}


