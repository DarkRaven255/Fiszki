#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtDebug>

class DbManager
{
public:
    DbManager(const QString& name);
    DbManager();
    ~DbManager();
    bool addUser(const QString& name);
    bool findUser(const QString& name);
    bool removeAllUsers();
    bool removeUser(const QString& name);
    QStringList displayAllUsers();
    void closeDB();
private:
    QSqlDatabase user_db;
    QSqlDatabase questions_db;
};

#endif // DBMANAGER_H
