#ifndef SESSION_H
#define SESSION_H

#include "dbmanager.h"
#include "question.h"
#include "enums.h"
#include <iostream>

#include <QVector>
#include <QObject>

class Session : public QObject
{
    Q_OBJECT

public:
    explicit Session(QObject *parent = nullptr);
    ~Session();

    QStringList getUserList();

    int getProgressPercent();

    void learnWords();
    void markQuestion();
    void testWords();
    void checkAnswer(const QString &answer);
    void nextLearnBtn();
    void backLearnBtn();

    void nextTestBtn();

    void getButtonStatus(bool &back, bool &remember, bool &next, bool &noQuestionsInDB, bool &noTestQuestions, bool &check);
    void exportBoxToDB();
    void exportBoxToDB2();

    Question *question;

private:
    int noMinusOneWords;
    int noZeroWords;
    int noOneWords;
    int noTwoWords;
    int noThreeWords;
    int noFourWords;
    int noFiveWords;
    int noSixWords;
    int noTestWords;

    int toLearnWords;
    int testCounterQuestions;
    int position;

    QVector<Question*> qList;
    QVector<int> testWordsList;

    QStringList userList;

    int randomInt(int min, int max);

    void randomTable();
    void setUserList();
    void recalculateQuestions();

    DbManager *dbmanager = new DbManager("database.db");

signals:

public slots:

};

#endif // SESSION_H
