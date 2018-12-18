#include "dbmanager.h"

DbManager::DbManager(const QString &name)
{
    user_db = QSqlDatabase::addDatabase("QSQLITE");
    user_db.setDatabaseName(name);

    if (!user_db.open())
    {
       qDebug() << "ERROR: " << user_db.lastError();
    }
    if(!user_db.tables().contains(QLatin1String("users")))
    {
        QSqlQuery query("CREATE TABLE users(id integer primary key, name text, level integer)");
        if(!query.isActive())
        {
            qDebug() << "ERROR: " << query.lastError().text();
        }
    }

}

DbManager::DbManager()
{
    questions_db = QSqlDatabase::addDatabase("QSQLITE");
    questions_db.setDatabaseName("questions.db");
    if(questions_db.open())
    {
        qDebug() << "JESTOK " << questions_db.lastError();
    }

    QSqlQuery query("CREATE TABLE users(id integer primary key, name text, level integer)");
}

DbManager::~DbManager()
{

}

bool DbManager::addUser(const QString &name)
{
    if(!findUser(name))
    {
        QSqlQuery query;

        query.prepare("INSERT INTO users (name, level) VALUES (:name, :level)");
        query.bindValue(":name", name);
        query.bindValue(":level", 0);
        if(query.exec())
        {
            return true;
        }
        else
        {
             qDebug() << "addUser error:  " << query.lastError();
        }
    }
    return false;

}

bool DbManager::findUser(const QString &name)
{
    QSqlQuery query;
    query.prepare("SELECT name FROM users WHERE name = (:name)");
    query.bindValue(":name", name);

    if (query.exec())
    {
       if (query.next())
       {
          return true;
       }
    }
    return false;
}

bool DbManager::removeAllUsers()
{
    QSqlQuery query;
    query.prepare("DELETE FROM users");
    if(query.exec())
    {
        return true;
    }
    return false;
}

bool DbManager::removeUser(const QString &name)
{
    if (findUser(name))
    {
       QSqlQuery query;
       query.prepare("DELETE FROM users WHERE name = (:name)");
       query.bindValue(":name", name);
       bool success = query.exec();

       if(!success)
       {
           qDebug() << "removeUser error: "<< query.lastError();
           return false;
       }
    }
    return true;
}

QStringList DbManager::displayAllUsers()
{
    QStringList userlist;
    QSqlQuery query("SELECT * FROM users");
    int idName = query.record().indexOf("name");
    int idLevel = query.record().indexOf("level");
    while (query.next())
    {
       qDebug()<<query.value(idName).toString()<<"    "<<query.value(idLevel).toString();
       userlist += (query.value(idName).toString());
    }

    return userlist;
}

void DbManager::closeDB()
{
    user_db.close();
    qDebug() << "UserDB closed";
}


