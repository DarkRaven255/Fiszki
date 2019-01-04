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
    void closeUserDB();
    void returnQuestion(int &noQuestion, int &noBox, int &q_id, QString &q_en, QString &e_en, QString &q_pl, QString &e_pl);
    void setBox(int &q_id);
    int countQuestions(int noBox);

private:
    QSqlDatabase database;
};

#endif // DBMANAGER_H
