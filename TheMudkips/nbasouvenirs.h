#ifndef NBASOUVENIRS_H
#define NBASOUVENIRS_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>

namespace Ui {
class nbaSouvenirs;
}

class nbaSouvenirs : public QWidget
{
    Q_OBJECT

public:
    explicit nbaSouvenirs(QWidget *parent = nullptr);
     //!< constructor

    ~nbaSouvenirs();
     //!< destructor

private slots:
    void on_teamTable_currentTextChanged(const QString &currentText);
    //!< Updates team name label and souvenir prices for the selected team

private:
    Ui::nbaSouvenirs *ui;   /*!< class object */

    QSqlDatabase myDB;      /*!< database connection object */
};

#endif // NBASOUVENIRS_H
