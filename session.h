#ifndef SESSION_H
#define SESSION_H
#include <QTime>
#include <QDebug>
#include "dbmanager.h"
#include "question.h"
#include "enums.h"

class Session : public QObject
{
    Q_OBJECT

public:
    explicit Session(QObject *parent = nullptr);
    ~Session();

    QStringList getUserList();

    int getProgressPercent();

    infoQuestions infoQuestions();

    void learnWords();
    void markQuestion();
    void deleteQuestion();
    void testWords();
    void checkAnswer(QString answer);

    Question *question;


private:
    int unknownQuestions;
    int testQuestions;
    int learnQuestions;
    int testCounterQuestions;

    QStringList userList;

    int randomInt(int min, int max);
    void setUserList();
    void recalculateQuestions();

    DbManager *dbmanager = new DbManager("database.db");

signals:

public slots:

};

#endif // SESSION_H
