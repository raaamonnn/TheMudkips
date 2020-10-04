#ifndef ADDTEAM_H
#define ADDTEAM_H

#include <QWidget>
#include"databasemanager.h"
#include<QtSql>
#include<iostream>
#include<cctype>
namespace Ui {
class addTeam;
}

class addTeam : public QWidget
{
    Q_OBJECT

public:
    explicit addTeam(QWidget *parent = nullptr);
    ~addTeam();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::addTeam *ui;
    QSqlDatabase myDb;
};

#endif // ADDTEAM_H
