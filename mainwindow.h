#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>
#include <QMessageBox>
#include <fstream>
#include <QFileDialog>
#include <QFile>

struct souvenirItem {
    int id;                            //!< The integer ID for each items
    QString name;                      //!< A QString for the name of items
    double price;                      //!< A double to store each item's price
    int    quantity;                   //!< An integer to store quantity purchased
};

struct College {
    int id;                                    //!< The integer ID of each college
    QString name;                              //!< A QString for the name of college
    double distanceToSaddleback;               //!< Double for each restaurants distance to Saddleback
    QVector<souvenirItem> souvenirItems;       //!< QVector of souvenir items that has each colleges' souvenirs

    bool operator<(const College& otherCollege) const
    {
        return this->distanceToSaddleback < otherCollege.distanceToSaddleback;
    }

    College()
    {

    }

    explicit College(int id, QString name, double distanceToSaddleback, QVector<souvenirItem> items)
    {
        this->id = id;
        this->name = name;
        this->distanceToSaddleback = distanceToSaddleback;
        this->souvenirItems = items;
    }
};

struct Distance
{
    int destinationCollege_ID;      //!< An integer of the destination's ID
    double distanceTo;                 //!< A double of the distance to the destination
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void populateMenu();
    void populateAdminMenu();

    ~MainWindow();

signals:
    void collegeClickedSignal(College);

private slots:
    void on_startButton_clicked();

    void initialCollegeSelected();

    void on_header_icon_push_button_clicked();

    void on_ASU_trip_button_clicked();

    void on_shortest_trip_button_clicked();

    void on_admin_push_button_clicked();

    void on_back_button_clicked();

    void on_login_button_clicked();

    void on_username_line_edit_returnPressed();

    void on_password_line_edit_returnPressed();

    void on_username_line_edit_textEdited(const QString &arg1);

    void on_password_line_edit_textEdited(const QString &arg1);

    void on_add_souvenir_button_clicked();

    void on_souvenir_list_widget_clicked(const QModelIndex &index);

    void on_souvenir_list_widget_doubleClicked(const QModelIndex &index);

    void on_upload_college_button_clicked();

    void on_exit_button_clicked();

private:
    void validateLogin();
    Ui::MainWindow *ui;
    int selectedRow;
};



#endif // MAINWINDOW_H
