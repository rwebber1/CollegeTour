#ifndef MAINTENANCE_H
#define MAINTENANCE_H

#include <QWidget>
#include "dbmanager.h"
#include "mainwindow.h"

namespace Ui {
class maintenance;
}

class maintenance : public QWidget
{
    Q_OBJECT

public:
    explicit maintenance(bool modifying = true, MainWindow *listWidget = 0, souvenirItem souvenir = souvenirItem(),
                        College college = College(), QWidget *parent = 0);
    ~maintenance();

private slots:
    void on_add_button_clicked();

    void on_college_combo_box_currentIndexChanged(int index);

    void on_modify_name_edit_textChanged(const QString &arg1);

    void on_modify_price_valueChanged(double arg1);

    void on_save_button_clicked();

    void on_cancel_button_2_clicked();

    void on_cancel_button_clicked();

private:
    Ui::maintenance *ui;

    MainWindow* mainWindow;
    souvenirItem itemToModify;
    int selectedCollegeID;
    souvenirItem newSouvenir;
};

#endif // MAINTENANCE_H
