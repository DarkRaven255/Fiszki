#include "session.h"
#include <QTime>
#include <QDebug>

Session::Session(QObject *parent):
    QObject(parent),
    toLearnWords(0),
    testCounterQuestions(0),
    position(0)
{
    recalculateQuestions();
    setUserList();

    qDebug()<<"Liczba pytan:"<<noTestWords+noMinusOneWords;
    qDebug()<<"Liczba nieznanych pytan:"<<noMinusOneWords;
    qDebug()<<"Liczba znanych pytan:"<<noTestWords;
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
    return static_cast<int>(static_cast<float>(position-1)/static_cast<float>(noTestWords)*100);
}

//Funkcja zmieniająca "pudełko"
inline void Session::markQuestion()
{
    question->set_isChanged();
}

//Funkcja ustawiająca flagi przycisków
void Session::getButtonStatus(bool &back, bool &remember, bool &next, bool &noQuestionsInDB, bool &noTestQuestions, bool &check)
{
    noQuestionsInDB=false;
    noTestQuestions=false;
    check=true;

    if(position==0) back=false;
    if(position>0) back=true;
    if(position==noMinusOneWords-1) next=false;
    if(position<noMinusOneWords-1) next=true;
    if(noMinusOneWords==1)
    {
        next=false;
        back=false;
    }

    if(noMinusOneWords==0)
    {
        next=false;
        back=false;
        remember=false;
        noQuestionsInDB=true;
    }

    if((position!=0&&position<noTestWords)||toLearnWords!=0)
    {
        if(qList[position]->qet_isChanged())
        {
            remember=false;
        }
        else
        {
            remember=true;
        }
    }

    if(position>noTestWords)
    {
        noTestQuestions=true;
        check=false;
    }

    if(noTestWords==0)
    {
        noTestQuestions=true;
    }

    if(noTestWords!=0)
    {
        noTestQuestions=false;
    }
}

//Funkcja przeliczająca pytania w bazie danych
void Session::recalculateQuestions()
{
    noMinusOneWords=dbmanager->countQuestions(-1);
    noZeroWords=dbmanager->countQuestions(0);
    noOneWords=dbmanager->countQuestions(1);
    noTwoWords=dbmanager->countQuestions(2);
    noThreeWords=dbmanager->countQuestions(3);
    noFourWords=dbmanager->countQuestions(4);
    noFiveWords=dbmanager->countQuestions(5);
    noSixWords=dbmanager->countQuestions(6);

    noTestWords=dbmanager->countQuestions(7);

    toLearnWords=0;
    position=0;
}

//Funkcja pobierająca pytania do nauki słówek
void Session::learnWords()
{
    qList.resize(toLearnWords+1);
    qList[toLearnWords] = new Question(toLearnWords,-1);
    question=qList.at(toLearnWords);
    position=toLearnWords;
    toLearnWords++;
}

//Funkcja pokazująca następne pytanie
void Session::nextLearnBtn()
{
    if(position<toLearnWords-1)
    {
        position++;
        question=qList.at(position);
    }
    else if(position==noMinusOneWords-1)
    {
        question=qList.at(position);
    }
    else
    {
        learnWords();
    }
}

//Funkcja cofająca pytanie
void Session::backLearnBtn()
{
    position--;
    question=qList.at(position);
}


//Funkcja pokazująca następne pytanie w opcji testu
void Session::nextTestBtn()
{
    if(position<noTestWords)
    {
        question=qList.at(position);
        position++;
    }
    else
    {
        position++;
    }
}

//Funkcja pobierająca pytania do testu
void Session::testWords()
{
    qList.resize(noTestWords);
    if(noTestWords==1)
    {
        qList[0] = new Question(0,7);
    }
    else
    {
        randomTable();
        for(int i=0;i<noTestWords;i++)
        {
            qList[i] = new Question(testWordsList.at(i),7);
        }
    }
    nextTestBtn();
}

//Funkcja generująca losową tablicę bez powtórzeń
void Session::randomTable()
{
    bool isRepeated;
    int random;

    testWordsList.resize(noTestWords);

    for(int j=0;j<noTestWords;j++)
    {
        do
        {
            isRepeated=false;
            random=randomInt(0,noTestWords-1);
            for(int i=0;i<noTestWords;i++)
            {
                if(testWordsList.at(i)==random)
                {

                    isRepeated=true;
                }
            }
            testWordsList[j]=random;
        }while(isRepeated);
    }
}

//Funkcja sprawdzająca odpowiedź
void Session::checkAnswer(const QString &answer)
{
    if(answer==question->getQ_pl())
    {
        markQuestion();
    }
    nextTestBtn();
}

//Funkcja pobierająca listę użytkowników z bazy danych
void Session::setUserList()
{
    userList=dbmanager->returnAllUsers();
}

//Funkcja do przeniesienia informacji do DB
void Session::exportBoxToDB(Status status)
{
    int size=0;
    //qDebug()<<status;

    if(status == LearnMode)
    {
        size = toLearnWords;
    }
    else if(status == TestMode)
    {
        size = noTestWords;
    }

    for(int i=0;i<size;i++)
    {
        if(qList.at(i)->qet_isChanged())
        {
            dbmanager->setBox(qList.at(i)->getQ_id());
        }
        delete qList.at(i);
    }
    recalculateQuestions();
}

//Generator liczb losowych
int Session::randomInt(int min, int max)
{
    QTime time = QTime::currentTime();
    qsrand(static_cast<uint>(time.msec()));
    return qrand() % ((max + 1) - min) + min;
}
