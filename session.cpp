#include "session.h"
#include <QTime>
#include <QDebug>

Session::Session(QObject *parent):
    QObject(parent),
    learnQuestions(0),
    testCounterQuestions(0),
    backLearnQuestions(0)

{
    recalculateQuestions();
    setUserList();

    qDebug()<<"Liczba pytan:"<<testQuestions+unknownQuestions;
    qDebug()<<"Liczba nieznanych pytan:"<<unknownQuestions;
    qDebug()<<"Liczba znanych pytan:"<<testQuestions;

    QTime time = QTime::currentTime();
    qsrand(static_cast<uint>(time.msec()));
}

Session::~Session()
{
    dbmanager->closeUserDB();
}

//Funkcja zwracająca listę użytkowników
QStringList Session::getUserList()
{
    return userList;
}

//Funkcja wyświetlająca procent przebiegu lekcji
int Session::getProgressPercent()
{
    return static_cast<int>(static_cast<float>(testCounterQuestions+1)/static_cast<float>(testQuestions)*100);
}

//Funkcja zwracająca informacje o stanie pytań w bazie
infoQuestions Session::infoQuestions()
{
    if(learnQuestions==unknownQuestions-1)
    {
        return LockNext;
    }
//    else if(learnQuestions==0)
//    {
//        return LockBack;
//    }
//    else if(learnQuestions>unknownQuestions-1)
//    {
//        learnQuestions=unknownQuestions-1;
//        return LockNext;
//    }
    else if(learnQuestions<0)
    {
        learnQuestions=0;
        return LockBack;
    }
    else if(unknownQuestions==1)
    {
        return LockNextBack;
    }
    else if(unknownQuestions==0)
    {
        return LockAll;
    }
    else return Null;
}

//Funkcja pobierająca pytania do nauki słówek
void Session::learnWords()
{
    qList.resize(learnQuestions+1);
    qList[learnQuestions] = new Question(nullptr,randomInt(0,unknownQuestions-1),-1);
    question=qList[learnQuestions];
    backLearnQuestions=learnQuestions;
}


void Session::nextLearnBtn()
{
    if(backLearnQuestions!=learnQuestions)
    {
        backLearnQuestions++;
        question=qList[backLearnQuestions];
    }
    else
    {
        learnQuestions++;
        learnWords();
    }
}

void Session::backLearnBtn()
{
    backLearnQuestions--;
    question=qList[backLearnQuestions];
}

//Funkcja zmieniająca "pudełko"
void Session::markQuestion()
{
    dbmanager->setBox(question->getQ_id());
    recalculateQuestions();
}

void Session::deleteQuestion()
{
    delete question;
}

//Funkcja pobierająca pytania do testu
void Session::testWords()
{
    recalculateQuestions();
    question = new Question(nullptr,randomInt(0,testQuestions-1),6);
    testCounterQuestions++;
}

//Funkcja sprawdzająca odpowiedź
void Session::checkAnswer(QString answer)
{
    if(answer==question->getQ_pl())
    {
        markQuestion();
    }
}

//Funkcja pobierająca listę użytkowników z bazy danych
void Session::setUserList()
{
    userList=dbmanager->returnAllUsers();
}

//Funkcja przeliczająca pytania w bazie danych
void Session::recalculateQuestions()
{
    unknownQuestions=dbmanager->countQuestions(-1);
    testQuestions=dbmanager->countQuestions(6);
}

//Funkcja zwracająca losowe liczby
int Session::randomInt(int min, int max)
{
    return qrand() % ((max + 1) - min) + min;
}
