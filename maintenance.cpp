#include "maintenance.h"
#include "ui_maintenance.h"

maintenance::maintenance(bool modifying, MainWindow *mainWindow, souvenirItem souvenir, College college, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::maintenance)
{
    ui->setupUi(this);
    ui->maintenance_stacked_widget->setCurrentIndex(1);
    this->mainWindow = mainWindow;
    if(modifying)
    {
        ui->modify_price->setDecimals(2);
        ui->modify_price->setSingleStep(0.01);
        ui->college_label->setText("For college: " + college.name);
        ui->modify_name_edit->setText(souvenir.name);
        ui->modify_price->setValue(souvenir.price);
        this->itemToModify= souvenir;
    }
    else
    {
        // We are adding a new item instead of modifying an existing one
        ui->maintenance_stacked_widget->setCurrentIndex(0);
        QVector<College> colleges = dbManager::getInstance()->getColleges();
        for (int index=0; index < colleges.size(); index++)
        {
            ui->college_combo_box->addItem(colleges[index].name, colleges[index].id);
        }
    }
}

maintenance::~maintenance()
{
    delete ui;
}

void maintenance::on_add_button_clicked()
{
    this->newSouvenir.name = ui->new_item_name->text();
    this->newSouvenir.price = ui->new_item_price->value();

    if (newSouvenir.name.isEmpty())
    {
        QMessageBox errorMsg;
        errorMsg.setText("Item name cannot be empty!");
        errorMsg.exec();
    } else
    {
        QMessageBox::StandardButton confirm;
        confirm = QMessageBox::question(this, "Confirm Modifications", "Are you sure you want to modify the database?",
                                        QMessageBox::Yes|QMessageBox::No);
        if (confirm == QMessageBox::Yes)
        {
            College collegeToAddItemTo = dbManager::getInstance()->getCollegeByID(selectedCollegeID);
            qDebug() << "New menu item: " << newSouvenir.name;
            qDebug() << "Adding to college: " << collegeToAddItemTo.name;
            dbManager::getInstance()->addSouvenirItem(newSouvenir, collegeToAddItemTo);
            mainWindow->populateAdminMenu();
        }
        this->close();
    }

}

void maintenance::on_college_combo_box_currentIndexChanged(int index)
{
    this->selectedCollegeID = ui->college_combo_box->itemData(index, Qt::UserRole).toInt();
}

void maintenance::on_modify_name_edit_textChanged(const QString &newName)
{
    this->itemToModify.name = newName;
}

void maintenance::on_modify_price_valueChanged(double newPrice)
{
    this->itemToModify.price = newPrice;
}

void maintenance::on_save_button_clicked()
{
    QMessageBox::StandardButton confirm;
    confirm = QMessageBox::question(this, "Confirm Modifications", "Are you sure you want to commit the changes to this menu item?",
                                    QMessageBox::Yes|QMessageBox::No);
    if (confirm == QMessageBox::Yes) {
        dbManager::getInstance()->modifySouvenirItem(itemToModify);
        mainWindow->populateAdminMenu();
    }
    this->close();
}

void maintenance::on_cancel_button_2_clicked()
{
    this->close();
}

void maintenance::on_cancel_button_clicked()
{
    this->close();
}
