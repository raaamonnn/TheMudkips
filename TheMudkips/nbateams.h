#ifndef NBATEAMS_H
#define NBATEAMS_H

#include <QWidget>
#include "QtSql"
#include "databasemanager.h"
#include "QtDebug"

namespace Ui {
class nbaTeams;
}

class nbaTeams : public QWidget
{
    Q_OBJECT

public:
    explicit nbaTeams(QWidget *parent = nullptr);
       //!< Constructor

    ~nbaTeams();
     //!< Destructor

private slots:

    void on_sortNameButton_clicked();
     //!<Displays Team Info by Name

    void on_sortYearButton_clicked();
     //!<Displays Team Info by Year

    void on_teamComboBox_activated(const QString &arg1);
     //!<Updates the ListView

private:
    Ui::nbaTeams *ui;/*!< class object */
    QSqlDatabase myDB;/*!< database connection */
};

#endif // NBATEAMS_H
