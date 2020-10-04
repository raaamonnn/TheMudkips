#ifndef AARENA_H
#define AARENA_H

#include <QWidget>
#include<iostream>
#include"databasemanager.h"
#include<QtSql>
namespace Ui {
class aArena;
}

class aArena : public QWidget
{
    Q_OBJECT

public:
    explicit aArena(QWidget *parent = nullptr);
    //!< Constructor
    ~aArena();
    //!< Destructor
private slots:


    void on_pushButton_3_clicked();
    //!< update the arena team relationships in DB
    void on_pushButton_2_clicked();
    //!< add a new arena and assign a team to it

private:
    Ui::aArena *ui; //!< class object
    QSqlDatabase myDb; //!< database connection
};

#endif // AARENA_H
