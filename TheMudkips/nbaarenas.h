#ifndef NBAARENAS_H
#define NBAARENAS_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>


namespace Ui {
class nbaArenas;
}

class nbaArenas : public QWidget
{
    Q_OBJECT

public:
    explicit nbaArenas(QWidget *parent = nullptr);
    ~nbaArenas();

private slots:
    void on_arenaButton_clicked();
    //!< View all teams, arenas, stadium capacities sorted by arena in alphabetical order

    void on_stadiumCapacityButton_clicked();
    //!< View all teams, arenas, stadium capacities sorted by stadium capacities lowest to highest

    void totalCapacity();
    //!< Calculates  total stadium capacity

    void on_arenaCB_currentIndexChanged();
    //!< View a specific arena based on selected arena

private:
    Ui::nbaArenas *ui;
    QSqlDatabase myDB;
    int total;
};

#endif // NBAARENAS_H
