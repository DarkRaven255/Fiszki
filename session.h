#ifndef SESSION_H
#define SESSION_H

#include "dbmanager.h"
#include "question.h"
#include "enums.h"
#include "user.h"

#include <QVector>
#include <QObject>

class Session : public QObject
{
    Q_OBJECT

public:
    explicit Session(QObject *parent = nullptr);
    ~Session();

    int getProgressPercent();
    void markWord();
    void exportBoxToDB(const Status &status);
    void getButtonStatus(bool &back, bool &remember, bool &next, bool &noQuestionsInDB, bool &noTestQuestions, bool &check, bool &learn, bool &testBtn);

    void learnWords();
    void nextLearnBtn();
    void backLearnBtn();

    void testWords();
    void nextTestBtn();
    void checkAnswer(const QString &answer);

    bool addUser(const QString &name);
    void setUser(const QString &name);
    QString getUser();
    void deleteUser();

    void setUserAction(const LastAction &action);

    void addWord(const QString &q_en, const QString &e_en, const QString &q_pl, const QString &e_pl);

    QStringList getUserList();

    Question *question;

private:
    int noMinusOneWords;
//    int noZeroWords;
//    int noOneWords;
//    int noTwoWords;
//    int noThreeWords;
//    int noFourWords;
//    int noFiveWords;
//    int noSixWords;
    int noTestWords;

    int toLearnWords;
    int testCounterQuestions;
    int position;
    int addToLearn;

    long long date;
    long long courseDay;

    QVector<Question *> qList;
    QVector<Question *> qTestList;
    QVector<int> testWordsList;
    QVector<int> boxesInUse;

    QStringList userList;

    int randomInt(int min, int max);

    void randomTable();
    void setUserList();
    void recalculateQuestions();

    unsigned long long fibonacci(const int &n);

    DbManager *dbmanager = new DbManager("database.db");
    User *user;

signals:

public slots:

};

#endif // SESSION_H
