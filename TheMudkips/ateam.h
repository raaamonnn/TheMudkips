#ifndef ATEAM_H
#define ATEAM_H

#include<QtSql>
#include <QWidget>
#include"databasemanager.h"
#include"addteam.h"
#include<iostream>
#include<qaxobject.h>
namespace Ui {
class aTeam;
}

class aTeam : public QWidget
{
    Q_OBJECT

public:
    explicit aTeam(QWidget *parent = nullptr); //!< Constructor
    ~aTeam();   //!< Detructor

private slots:

    void on_comboBox_currentIndexChanged(const QString &arg1); //!< collects and displays team info for editing

    void on_pushButton_clicked();//!< Update 'Team Info' in DB

    void on_pushButton_3_clicked();//!< Delete selected team from DB

    void on_pushButton_2_clicked();//!< Add a new team from input file into DB

private:
    Ui::aTeam *ui; //!< class object
    QSqlDatabase myDb; //!< database connection
    addTeam window; //!< optional utility dialog box
};

#endif // ATEAM_H
