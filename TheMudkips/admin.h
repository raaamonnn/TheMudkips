#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>

#include "ateam.h"
#include "aarena.h"
#include "asouvenirs.h"

namespace Ui {
class Admin;
}

class Admin : public QWidget
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = nullptr);
    //!< constructor

    ~Admin();
    //!< destructor

private slots:
    void on_homeButton_clicked();
    //!< show home page

    void on_page1Button_clicked();
    //!< show admin team window

    void on_page2Button_clicked();
    //!< show add team window

    void on_exitButton_clicked();
    //!< show souvenir window

    void on_page3Button_clicked();
    //!< close admin window

private:
    Ui::Admin *ui;  /*!< admin class object */

    aTeam teamWindow;           /*!< admin team information object */
    aArena arenaWindow;         /*!< admin arena object */
    aSouvenirs souvenirsWindow; /*!< admin souvenirs object */
};

#endif // ADMIN_H
