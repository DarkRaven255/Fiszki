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

    qDebug()<<"Liczba pytan:"<<testQuestions+unknownQuestions;
    qDebug()<<"Liczba nieznanych pytan:"<<unknownQuestions;
    qDebug()<<"Liczba znanych pytan:"<<testQuestions;


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
    return static_cast<int>(static_cast<float>(testCounterQuestions)/static_cast<float>(testQuestions)*100);
}

//Funkcja pobierająca pytania do nauki słówek
void Session::learnWords()
{
    qList.resize(toLearnWords+1);
    qList[toLearnWords] = new Question(nullptr,toLearnWords,-1);
    question=qList.at(toLearnWords);
    position=toLearnWords;
    toLearnWords++;
}

//void Session::randomTable()
//{
//    bool isRepeated;
//    int random=0;

//    randomList.resize(unknownQuestions);
//    do
//    {
//        int j=0;
//        isRepeated=false;
//        random=randomInt(0,unknownQuestions-1);
//        for(int i=0;i<unknownQuestions;i++)
//        {
//            if(randomList[i]==random)
//            {
//                isRepeated=true;
//            }
//        }
//        random[j]
//    }while(isRepeated);

//}


void Session::nextLearnBtn()
{
    if(position<toLearnWords-1)
    {
        position++;
        question=qList.at(position);
    }
    else if(position==unknownQuestions-1)
    {
        question=qList.at(position);
    }
    else
    {
        learnWords();
    }
}

void Session::backLearnBtn()
{
    position--;
    question=qList.at(position);
}

void Session::getButtonStatus(bool &back, bool &remember, bool &next, bool &noQuestionsInDB, bool &noTestQuestions)
{
    qDebug()<<position;

    noQuestionsInDB=false;
    noTestQuestions=false;

    if(position==0) back=false;
    if(position>0) back=true;
    if(position==unknownQuestions-1) next=false;
    if(position<unknownQuestions-1) next=true;
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

    if(position!=0||toLearnWords!=0)
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
    //if(testQuestions==0) noTestQuestions=true;

    if(testCounterQuestions==testQuestions) noTestQuestions=true;
}

//Funkcja zmieniająca "pudełko"
void Session::markQuestion()
{
    question->set_isChanged();
}

//Funkcja pobierająca pytania do testu
void Session::testWords()
{
    recalculateQuestions();
    if(testCounterQuestions<testQuestions)
    {
        question = new Question(nullptr,randomInt(0,testQuestions),6);
    }
    testCounterQuestions++;
}

//Funkcja sprawdzająca odpowiedź
void Session::checkAnswer(const QString &answer)
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
    toLearnWords=0;
    position=0;
}

void Session::exportBoxToDB()
{
    for(int i=0;i<toLearnWords;i++)
    {
        if(qList.at(i)->qet_isChanged())
        {
            dbmanager->setBox(qList.at(i)->getQ_id());
        }
        delete qList.at(i);
    }
    recalculateQuestions();
}

int Session::randomInt(int min, int max)
{
    QTime time = QTime::currentTime();
    qsrand(static_cast<uint>(time.msec()));
    return qrand() % ((max + 1) - min) + min;
}
