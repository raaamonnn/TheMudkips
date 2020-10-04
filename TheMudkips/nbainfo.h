#ifndef NBAINFO_H
#define NBAINFO_H

#include <QWidget>

#include "nbateams.h"
#include "nbaarenas.h"
#include "nbaconfdiv.h"
#include "nbacoaches.h"
#include "nbasouvenirs.h"

namespace Ui {
class NBAInfo;
}

class NBAInfo : public QWidget
{
    Q_OBJECT

public:
    explicit NBAInfo(QWidget *parent = nullptr);
     //!< constructor

    ~NBAInfo();
     //!< destructor

private slots:
    void on_homeButton_clicked();
    //!< Displays home page

    void on_teamsButton_clicked();
    //!< Shows team information widget

    void on_arenasButton_clicked();
    //!< Shows arena information widget

    void on_confDivButton_clicked();
    //!< Shows conference and division information widget

    void on_coachesButton_clicked();
    //!< Shows team coaches information widget

    void on_souvenirsButton_clicked();
    //!< Shows team souvenirs information widget

    void on_exitButton_clicked();
    //!< Closes NBA info widget

private:
    Ui::NBAInfo *ui;                /*!< NBA info class object */

    nbaTeams teamWindow;            /*!< team information object */
    nbaArenas arenaWindow;          /*!< arena information object */
    nbaConfDiv confDivWindow;       /*!< conference division object */
    nbaCoaches coachWindow;         /*!< coach information object */
    nbaSouvenirs souvenirsWindow;   /*!< souvenirs object */
};

#endif // NBAINFO_H
