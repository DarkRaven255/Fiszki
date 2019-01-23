#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>

class DbManager
{
public:
    DbManager(const QString &name);
    ~DbManager();

    bool addUser(const QString &name, const int &noBox, const long long &setStartDate);
    bool findUser(const QString &name);

    bool findWord(const QString &q_en);
    void addWord(const QString &q_en, const QString &e_en, const QString &q_pl, const QString &e_pl);

    bool removeAllUsers();
    bool removeUser(const QString &name);
    void resetUserBox(const QString &userBox);

    void setUserLastAction(const QString &name, const int &action);
    void setUserLastUsed(const QString &name, const long long &lastUsed);
    void setBox(const int &q_id, const QString &userBox, const unsigned long long &noBox=0);
    void setUnknownQuestions(const QString &name, const int &number);

    int countQuestions(const int &noBox, const QString &userBox, const long long &courseDay);
    int countUsers();

    void returnBoxesInUse(QVector<int> &listFreeBoxes);
    QStringList returnUserList();
    void returnQuestion(const int &noQuestion, const int &noBox, const QString &userBox/*, const long long &courseDay*/,
                        int &q_id, QString &q_en, QString &e_en, QString &q_pl, QString &e_pl);
    long long returnUserInfo(const QString &name, const QString parameter);

    void closeUserDB();

private:
    QSqlDatabase database;
};

#endif // DBMANAGER_H
