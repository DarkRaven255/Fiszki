#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>

class DbManager
{
public:
    DbManager(const QString& name);
    ~DbManager();
    bool addUser(const QString& name);
    bool findUser(const QString& name);
    bool removeAllUsers();
    bool removeUser(const QString& name);
    QStringList returnAllUsers();
    void closeUserDB();
    void returnQuestion(const int &noQuestion, const int &noBox, int &q_id, QString &q_en, QString &e_en, QString &q_pl, QString &e_pl);
    void setBox(const int &q_id);
    int countQuestions(const int noBox);

private:
    QSqlDatabase database;
};

#endif // DBMANAGER_H
