#include "dbmanager.h"

dbManager* dbManager::instance = nullptr;


dbManager::dbManager()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("colleges.db");

    if(!database.open())
    {
        qDebug() << ("Database Connection Failed");
    }
    else
    {
        qDebug() << ("Database Connected");
        initializeDB();
    }
}

dbManager* dbManager::getInstance()
{
    if (instance == nullptr)
    {
        instance = new dbManager();
    }
    return instance;
}

void dbManager::closeDatabase()
{
    database.close();
    database.removeDatabase(QSqlDatabase::defaultConnection);
}

void dbManager::initializeDB()
{
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS colleges   (ID INTEGER PRIMARY KEY, Name VARCHAR(35), DistanceToSaddleback REAL)");
    query.exec("CREATE TABLE IF NOT EXISTS souvenir_items (ID INTEGER PRIMARY KEY, College_ID integer, Name VARCHAR(35), Price REAL)");
    query.exec("CREATE TABLE IF NOT EXISTS distances (CollegeA_ID integer, CollegeB_ID integer, DistanceBetween REAL)");
    query.exec("CREATE TABLE IF NOT EXISTS admins ( name VARCHAR(12), password VARCHAR(12) )");
}

int dbManager::getTotalColleges()
{
    // simply returns an integer representing the total number of the colleges in the db
    QSqlQuery query;
    query.exec("SELECT Name FROM colleges");
    int collegeTotal = 0;
    if (query.exec())
    {
        if(query.first())
        {
            while (query.isValid())
            {
                collegeTotal++;
                query.next();
            }
        }
    }
    return collegeTotal;
}

QVector<College> dbManager::getColleges()
{
    QSqlQuery query;
    query.prepare("SELECT ID, Name, DistanceToSaddleback FROM colleges");
    QVector<College> colleges;

    if (query.exec())
    {
        while (query.next())
        {
            College newCollege;
            newCollege.id = query.value(0).toInt();
            newCollege.name = query.value(1).toString();
            newCollege.distanceToSaddleback = query.value(2).toDouble();

            QVector<souvenirItem> souvenirItems = this->getSouvenirsByCollegeID(newCollege.id);

            newCollege.souvenirItems = souvenirItems;

            colleges.push_back(newCollege);
        }

    }
    else
    {
        qDebug() << "Failed to execute query\n";
    }
    return colleges;
}

QVector<souvenirItem> dbManager::getSouvenirsByCollegeID(int collegeID)
{
    QVector<souvenirItem> souvenirItems;
    QSqlQuery query;

    query.prepare("SELECT Name, Price, id FROM Souvenir_Items WHERE College_ID=:college_ID");
    query.bindValue(":college_ID", collegeID);

    if (query.exec())
    {
        if (query.first())
        {
            while(query.isValid())
            {
                souvenirItem tempItems;
                tempItems.name = query.value(0).toString();
                tempItems.price = query.value(1).toDouble();
                tempItems.id = query.value(2).toInt();
                souvenirItems.push_back(tempItems);
                query.next();
            }
        }
    }
    return souvenirItems;
}

College dbManager::getCollegeByID(int college_ID)
{
    QSqlQuery query;
    query.prepare("SELECT id, Name, DistanceToSaddleback FROM colleges WHERE id=:college_ID");
    query.bindValue(":college_ID", college_ID);

    if(query.exec())
    {
        if (query.first())
        {
            College returnCollege;
            returnCollege.id = query.value(0).toInt();
            returnCollege.name = query.value(1).toString();
            returnCollege.souvenirItems = getSouvenirsByCollegeID(returnCollege.id);
            returnCollege.distanceToSaddleback = query.value(2).toDouble();

            return returnCollege;
        }
    }
}

souvenirItem dbManager::getSouvenirByID(int souvenirItem_ID)
{
    QSqlQuery query;
    query.prepare("SELECT id, Name, Price FROM Souvenir_Items WHERE id=:souvenirItem_ID");
    query.bindValue(":souvenirItem_ID", souvenirItem_ID);

    if(query.exec())
    {
        if (query.first()) {
            souvenirItem returnItem;
            returnItem.id = query.value(0).toInt();
            returnItem.name = query.value(1).toString();
            returnItem.price = query.value(2).toDouble();

            return returnItem;
        }
    }

}

QVector<Distance> dbManager::getDistancesFrom(int sourceCollege_ID)
{
    QSqlQuery query;
    QVector<Distance> distances;
    query.prepare("SELECT CollegeB_ID, DistanceBetween FROM distances WHERE CollegeA_ID=:sourceCollege_ID ORDER BY DistanceBetween ASC");
    query.bindValue(":sourceCollege_ID", sourceCollege_ID);

    if (query.exec())
    {
        if (query.first())
        {
            while (query.isValid())
            {
                Distance tempDistance;
                tempDistance.destinationCollege_ID = query.value(0).toInt();
                tempDistance.distanceTo = query.value(1).toDouble();

                distances.push_back(tempDistance);
                query.next();
            }
        }
    }
    return distances;
}

bool dbManager::authenticateAdminLoginRequest(QString name, QString password)
{
    bool isAdmin = false;
    QSqlQuery query;
    query.prepare("SELECT name, password FROM admins WHERE lower(name)=lower(:name)");
    query.bindValue(":name", name);
    query.bindValue(":password", password);

    if (query.exec())
    {
        if (query.first())
        {
            if (query.value(1).toString().toLower() == password.toLower())
            {
                // the username and password inputted is an admin account
                isAdmin = true;
            }
        }
    } else {
        isAdmin = false;
    }

    return isAdmin;
}

void dbManager::addCollege(College college, QVector<Distance> distances)
{
    QSqlQuery query;
    QSqlQuery souvenirItemsQuery;

    // Adds the college name, ID, and distanceToSaddleback to colleges table in database
    query.prepare("INSERT INTO colleges (id, name, distanceToSaddleback) VALUES(:id, :name, :distanceToSaddleback)");
    query.bindValue(":id", college.id);
    query.bindValue(":distanceToSaddleback", college.distanceToSaddleback);
    query.bindValue(":name", college.name);


    // Adds all the colleges menu items to the menu_items table
    for (int index=0; index < college.souvenirItems.size(); index++)
    {
        souvenirItemsQuery.prepare("INSERT INTO menu_items (Name, Price, College_ID) VALUES(:name, :price, :college_ID)");
        souvenirItemsQuery.bindValue(":name", college.souvenirItems[index].name);
        souvenirItemsQuery.bindValue(":price", college.souvenirItems[index].price);
        souvenirItemsQuery.bindValue(":college_ID", college.id);
        souvenirItemsQuery.exec();
    }

    // Add distances for the college being added
    for (int index=0; index < distances.size(); index++)
    {

        query.prepare("INSERT INTO distances (CollegeA_ID, CollegeB_ID, DistanceBetween) VALUES(:CollegeA_ID, :CollegeB_ID, :DistanceBetween)");
        query.bindValue(":CollegeA_ID", college.id);
        //query.bindValue(":CollegeB_ID", distances[index].destinationCollege_ID);
        query.bindValue(":DistanceBetween", distances[index].distanceTo);
        //qDebug() << "Inserting distance from " << college.id << " to: " << distances[index].destinationCollege_ID;
        query.exec();

        query.prepare("INSERT INTO distances (CollegeA_ID, CollegeB_ID, DistanceBetween) VALUES(:CollegeA_ID, :CollegeB_ID, :DistanceBetween)");
        //query.bindValue(":CollegeA_ID", distances[index].destinationCollege_ID);
        query.bindValue(":CollegeB_ID", college.id);
        //query.bindValue(":DistanceBetween", distances[index].distanceTo);
        //qDebug() << "Inserting distance from " << distances[index].destinationCollege_ID<< " to: " << college.id;
        query.exec();
    }
}

void dbManager::addSouvenirItem(souvenirItem newSouvenir, College toAddTo)
{
    QSqlQuery souvenirItemQuery;
    souvenirItemQuery.prepare("INSERT INTO menu_items (Name, Price, College_ID) VALUES(:name, :price, :college_ID)");
    souvenirItemQuery.bindValue(":name", newSouvenir.name);
    souvenirItemQuery.bindValue(":price", newSouvenir.price);
    souvenirItemQuery.bindValue(":college_ID", toAddTo.id);
}

void dbManager::modifySouvenirItem(souvenirItem newSouvenirItem)
{
    QSqlQuery query;
    query.prepare("UPDATE menu_items SET name=:name, price=:price WHERE id=:oldID");
    query.bindValue(":name", newSouvenirItem.name);
    query.bindValue(":price", newSouvenirItem.price);
    query.bindValue(":oldID", newSouvenirItem.id);
}

void dbManager::deleteSouvenirItem(souvenirItem deletedItem)
{
    QSqlQuery query;
    query.prepare("DELETE FROM menu_items WHERE id=:ID");
    query.bindValue(":ID", deletedItem.id);
}
