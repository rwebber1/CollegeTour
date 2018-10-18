#ifndef COLLEGEMODEL_H
#define COLLEGEMODEL_H

#include <QWidget>
#include <QPushButton>
#include <QListWidgetItem>
#include "QInputDialog"
#include <QMessageBox>
#include "mainwindow.h"
#include "dbmanager.h"
#include "cart.h"
#include "totalssheet.h"

#include <QMovie>
#include <QDateTime>

namespace Ui {
class CollegeModel;
}

class CollegeModel : public QWidget
{
    Q_OBJECT

public:
    explicit CollegeModel(College collegeClicked, bool asuTrip, QWidget *parent = 0);
    ~CollegeModel();

    void recursivePathPlanner(College currentCollege, QVector<College> &mostEfficientList);
    void getTripLength();
    void populateSouvenirMenu(int collegeID);
    void clearWidgets(QLayout *layout);
    bool vectorContains(QVector<College> colleges, College searchRest);
    void confirmPurchase(souvenirItem souvenir);
    void updateCart(souvenirItem item);

    void delay(int n);

protected slots:
    void loadingPage();

public slots:
    void souvenirButtonPressed();

private slots:
    void on_next_college_button_clicked();
    void on_cartList_clicked(const QModelIndex &index);
    void on_removeCartItemButton_clicked();

    void on_totals_button_clicked();

private:
    Ui::CollegeModel *ui;
    College collegeClicked;
    int totalCollegesToVisit;

    QMovie *drivingGif;

    Cart cart;

    QVector<College> mostEfficientTrip;
    QVector<College>::iterator tripIterator;
    QVector<double> totalDistancesIndexed;
    double totalDistance;
    int selectedRow;
};

#endif // COLLEGEMODEL_H
