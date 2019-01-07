#include "session.h"
#include <QTime>
#include <QDebug>

Session::Session(QObject *parent):
    QObject(parent),
    learnQuestions(0),
    testCounterQuestions(0),
    position(0)

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
    foreach (Question *question, qList)
    {
        delete question;
    }
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

//Funkcja pobierająca pytania do nauki słówek
void Session::learnWords()
{
    bool isRepeated;
    int random;
    if(learnQuestions>2)
    {
        do
        {
            isRepeated=false;
            random=randomInt(0,unknownQuestions-1);
            for(int i=1;i<learnQuestions;i++)
            {
                if(qList[i]->getQ_id()==random)
                {
                    isRepeated=true;
                }
            }
        }while(isRepeated);

        qList.resize(learnQuestions+1);
        qList[learnQuestions] = new Question(nullptr,random,-1);
        question=qList[learnQuestions];
        position=learnQuestions;
    }
    qList.resize(learnQuestions+1);
    qList[learnQuestions] = new Question(nullptr,randomInt(0,unknownQuestions-1),-1);
    question=qList[learnQuestions];
    position=learnQuestions;
}


void Session::nextLearnBtn()
{
    if(position!=learnQuestions)
    {
        position++;
        question=qList[position];
    }
    else
    {
        learnQuestions++;
        position++;
        learnWords();
    }
}

void Session::backLearnBtn()
{
    position--;
    question=qList[position];
}

void Session::getButtonStatus(bool &back, bool &remember, bool &next, bool &noQuestionsInDB)
{

    qDebug()<<"Learn Questions:"<<learnQuestions;
    qDebug()<<"position:"<<position;

    noQuestionsInDB=false;

    if(position==0) back=false;
    if(position>0) back=true;
    if(position==testQuestions+unknownQuestions) next=false;
    if(position<testQuestions+unknownQuestions) next=true;
    if(unknownQuestions==1)
    {
        next=false;
        back=false;
    }
    if(unknownQuestions==0)
    {
        next=false;
        back=false;
        remember=false;
        noQuestionsInDB=true;
    }
    if(position!=0)
    {
        //qDebug()<<qList[position]->qetQ_box();
        if(qList[position]->qetQ_box()>-1)
        {
            remember=false;
        }
        else
        {
            remember=true;
        }
    }


}

//Funkcja zmieniająca "pudełko"
void Session::markQuestion()
{
    dbmanager->setBox(question->getQ_id());
    question->setQ_box();
    recalculateQuestions();
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
