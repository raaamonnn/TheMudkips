#ifndef VACATION_H
#define VACATION_H

#include <QWidget>
#include <vpackagea.h>
#include <vpackageb.h>
#include <vpackagec.h>
#include <vsimulation.h>
#include <vbfs.h>
#include <vdfs.h>
#include <vmst.h>

namespace Ui {
class Vacation;
}

class Vacation : public QWidget
{
    Q_OBJECT

public:
    explicit Vacation(QWidget *parent = nullptr);
    //!< Constructor
    ~Vacation();
    //!< destructor

private slots:
    void on_packagesButton_clicked();
    //!< Shows package widget in same window

    void on_bfsButton_clicked();
    //!< Shows bfs widget in same window

    void on_dfsButton_clicked();
    //!< Shows dfs widget in same window

    void on_mstButton_clicked();
    //!< Shows mst widget in same window

    void on_exitButton_clicked();
    //!< Closes vacation window

    void on_packageAButton_clicked();
    //!< Opens package A window

    void on_packageBButton_clicked();
    //!< Opens package B window

    void on_packageCButton_clicked();
    //!< Opens package C window

private:
    Ui::Vacation *ui; //!< vacation window object

    vPackageA *packageAWindow; //!< package A window object
    vPackageB *packageBWindow; //!< package B window object
    vPackageC *packageCWindow; //!< package C window object
    vBFS      bfsWindow; //!< bfs window object
    vDFS      dfsWindow; //!< dfs window object
    vMST      mstWindow; //!< mst window object
};

#endif // VACATION_H
