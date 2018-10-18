#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "mainwindow.h"

class dbManager
{
public:
    void initializeDB();
    void closeDatabase();
    static dbManager* getInstance();

    int getTotalColleges();
    QVector<College> getColleges();
    QVector<souvenirItem> getSouvenirsByCollegeID(int collegeID);
    College getCollegeByID(int college_ID);
    souvenirItem getSouvenirByID(int souvenirItem_ID);
    QVector<Distance> getDistancesFrom(int sourceCollege_ID);
    bool authenticateAdminLoginRequest(QString name, QString password);

    void addCollege(College college, QVector<Distance> distances);
    void addSouvenirItem(souvenirItem newSouvenir, College toAddTo);

    void modifySouvenirItem(souvenirItem newSouvenir);

    void deleteSouvenirItem(souvenirItem deletedItem);

private:
    dbManager();
    QSqlDatabase database;
    static dbManager *instance;
};



#endif // DBMANAGER_H
