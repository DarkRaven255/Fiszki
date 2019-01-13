#include "session.h"
#include <QTime>
#include <QDebug>

Session::Session(QObject *parent):
    QObject(parent),
    toLearnWords(0),
    testCounterQuestions(0),
    position(0)
{
    setUserList();

    qDebug()<<"Liczba pytan:"<<noTestWords+noMinusOneWords;
    qDebug()<<"Liczba nieznanych pytan:"<<noMinusOneWords;
    qDebug()<<"Liczba znanych pytan:"<<noTestWords;
}

Session::~Session()
{
    dbmanager->closeUserDB();
}

//Funkcja pobierająca listę użytkowników z bazy danych
void Session::setUserList()
{
    userList=dbmanager->returnUserList();
}

//Funkcja zwracająca listę użytkowników
QStringList Session::getUserList()
{
    setUserList();
    return userList;
}

//Funkcja dodająca nowego użytkownika i przypisująca mu wolny noBox
bool Session::addUser(const QString &name)
{
    if(dbmanager->countUsers()>=MaxUsers)
    {
        return false;
    }

    boxesInUse.resize(MaxUsers);
    boxesInUse.fill(0,MaxUsers);

    dbmanager->returnBoxesInUse(boxesInUse);

    ///RANDOM TABLE DO UNIFIKACJI !!!////////////////////////////////////////////////////////////////////
    int isRepeated;
    int freeBox = 1;

    for(int j=0;j<dbmanager->countUsers();j++)
    {
        do
        {
            isRepeated=false;
            int random=randomInt(1,MaxUsers);
            for(int i=0;i<MaxUsers;i++)
            {
                if(boxesInUse.at(i)==random)
                {

                    isRepeated=true;
                }
            }
            freeBox=random;
        }while(isRepeated);
    }

    if(dbmanager->addUser(name,freeBox))
    {
        return true;
    }
    return false;
}

//Stworzenie użytkownika w sesji
void Session::setUser(const QString &name)
{
    user = new User(name,"box"+static_cast<QString>(dbmanager->findUserBox(name)+48),this);
    recalculateQuestions();
}

//Pobranie nazwy użytkownika w sesji
QString Session::getUser()
{
    return user->getUserName();
}

//Usunięcie użytkownika w sesji
void Session::deleteUser()
{
    delete user;
}

//Funkcja wyświetlająca procent przebiegu lekcji
int Session::getProgressPercent()
{
    return static_cast<int>(static_cast<float>(position-1)/static_cast<float>(noTestWords)*100);
}

//Funkcja zmieniająca "pudełko"
void Session::markQuestion()
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
    noMinusOneWords=dbmanager->countQuestions(-1,user->getNoBox());
    noZeroWords=dbmanager->countQuestions(0,user->getNoBox());
    noOneWords=dbmanager->countQuestions(1,user->getNoBox());
    noTwoWords=dbmanager->countQuestions(2,user->getNoBox());
    noThreeWords=dbmanager->countQuestions(3,user->getNoBox());
    noFourWords=dbmanager->countQuestions(4,user->getNoBox());
    noFiveWords=dbmanager->countQuestions(5,user->getNoBox());
    noSixWords=dbmanager->countQuestions(6,user->getNoBox());

    noTestWords=dbmanager->countQuestions(7,user->getNoBox());

    toLearnWords=0;
    position=0;
}

//Funkcja pobierająca pytania do nauki słówek
void Session::learnWords()
{
    qList.resize(toLearnWords+1);
    qList[toLearnWords] = new Question(toLearnWords,user->getNoBox(),-1);
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
        qList[0] = new Question(0,user->getNoBox(),7);
    }
    else
    {
        randomTable();
        for(int i=0;i<noTestWords;i++)
        {
            qList[i] = new Question(testWordsList.at(i),user->getNoBox(),7);
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
            dbmanager->setBox(qList.at(i)->getQ_id(),user->getNoBox());
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
