#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>

class DbManager
{
public:
    DbManager(const QString &name);
    ~DbManager();

    bool addUser(const QString &name, const int &noBox);
    bool findUser(const QString &name);
    int findUserBox(const QString &name);

    bool findWord(const QString &q_en);
    void addWord(const QString &q_en, const QString &e_en, const QString &q_pl, const QString &e_pl);

    bool removeAllUsers();
    bool removeUser(const QString &name);
    void resetUserBox(const QString &userBox);

    void setBox(const int &q_id, const QString &userBox);

    int countQuestions(const int noBox, const QString &userBox);
    int countUsers();

    void returnBoxesInUse(QVector<int> &listFreeBoxes);
    QStringList returnUserList();
    void returnQuestion(const int &noQuestion, const int &noBox, const QString &userBox, int &q_id, QString &q_en, QString &e_en, QString &q_pl, QString &e_pl);

    void closeUserDB();

private:
    QSqlDatabase database;
};

#endif // DBMANAGER_H
