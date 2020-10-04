#ifndef NBACONFDIV_H
#define NBACONFDIV_H

#include <QWidget>
#include "QtSql"
#include "databasemanager.h"
#include "QtDebug"

namespace Ui {
class nbaConfDiv;
}

class nbaConfDiv : public QWidget
{
    Q_OBJECT

public:
    explicit nbaConfDiv(QWidget *parent = nullptr);
    //!< Constructor

    ~nbaConfDiv();
    //!< Destructor

private slots:

    void on_pushButton_2_clicked();
    //!<Displays Team Info from all Eastern teams ordered by Team Name

    void on_pushButton_clicked();
    //!<Displays Team Info from all Easter teams in the Southeast Division ordered by Team Name

private:
    Ui::nbaConfDiv *ui; /*!< class object */
    QSqlDatabase myDB; /*!< database connection */
};

#endif // NBACONFDIV_H
