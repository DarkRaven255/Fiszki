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
    void returnQuestion(QString &setTopic, int &noQuestion, QString &q_en, QString &e_en, QString &q_pl, QString &e_pl);
    void returnQuestion(int &noQuestion, QString &q_en, QString &e_en, QString &q_pl, QString &e_pl);
    void markAsKnown(QString &q_en);
    int countQuestions();
    int countAllQuestions();
private:
    QSqlDatabase database;
};

#endif // DBMANAGER_H
