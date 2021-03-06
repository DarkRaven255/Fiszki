#include "session.h"
#include <QTime>
#include <QDebug>
#include "mistakewindow.h"

Session::Session(QObject *parent):
    QObject(parent),
    question(nullptr),
    toLearnWords(0),
    position(0),
    addToLearn(0),
    mistake(false),
    date(QDate::currentDate().toJulianDay()),
    user(nullptr)
{
    setUserList();
}

Session::~Session()
{
    dbmanager->closeDB();
    if(dbmanager!=nullptr)delete dbmanager;
    if(user!=nullptr) delete user;
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
bool Session::addUserToDB(const QString &name)
{
    if(dbmanager->countUsers()>=ConstansMaxUsers)
    {
        return false;
    }

    boxesInUse.resize(ConstansMaxUsers);
    boxesInUse.fill(0,ConstansMaxUsers);

    dbmanager->returnBoxesInUse(boxesInUse);

    int isRepeated;
    int freeBox = 1;

    for(int j=0;j<dbmanager->countUsers();j++)
    {
        do
        {
            isRepeated=false;
            int random=randomInt(1,ConstansMaxUsers);
            for(int i=0;i<ConstansMaxUsers;i++)
            {
                if(boxesInUse.at(i)==random)
                {
                    isRepeated=true;
                }
            }
            freeBox=random;
        }while(isRepeated);
    }

    if(dbmanager->addUser(name,freeBox,date))
    {
        dbmanager->resetUserBox("box"+static_cast<QString>(freeBox+48));
        return true;
    }
    return false;
}

//Stworzenie użytkownika w sesji
void Session::setUser(const QString &name)
{
    user = new User(name,
                    "box"+static_cast<QString>(static_cast<int>(dbmanager->returnUserInfo(name,"noBox"))+48),
                    dbmanager->returnUserInfo(name,"start_date"),
                    dbmanager->returnUserInfo(name,"last_used"),
                    static_cast<int>(dbmanager->returnUserInfo(name,"last_action")),
                    static_cast<int>(dbmanager->returnUserInfo(name,"unknown_questions"))
                    ,this);

    if (user->getLastUsed()<date)
    {
        user->setLastAction(0);
        dbmanager->setUserLastAction(user->getUserName(),user->getLastAction());
        if (user->getLastUsed()>user->getStartDate())
        {
            addToLearn=user->getUnknownQuestions();
        }
    }
    courseDay = date - user->getStartDate();
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
    user=nullptr;
}

//Funkcja zapisaująca ostatnią akcję wykonaną przez użytkownika
void Session::setUserLastAction(const LastAction &action)
{
    if(user->getLastAction()==LastActionNone)
    {
        user->setLastAction(action);
    }
    else if((user->getLastAction()==LastActionLearn&&action==LastActionTest)||(user->getLastAction()==LastActionTest&&action==LastActionLearn))
    {
        user->setLastAction(LastActionLearnTest);
    }
}

//Funkcja dodająca nowe pytania do bazy danych
void Session::addWordToDB(const QString &q_en, const QString &e_en, const QString &q_pl, const QString &e_pl)
{
    if(!dbmanager->findWord(q_en))
    {
        dbmanager->addWord(q_en,e_en,q_pl,e_pl);
    }
}


//Funkcja wyświetlająca procent przebiegu lekcji
int Session::getProgressPercent()
{
    return static_cast<int>(static_cast<float>(position-1)/static_cast<float>(noTestWords)*100);
}

//Funkcja zmieniająca "pudełko"
void Session::toggleIsChanged()
{
    question->set_isChanged();
}

void Session::toggleMistake()
{
    mistake=true;
}

//Funkcja ustawiająca flagi przycisków
void Session::getButtonStatus(bool &back, bool &remember, bool &next, bool &noQuestionsInDB, bool &noTestQuestions, bool &check, bool &learn, bool &testBtn)
{
    noQuestionsInDB = false;
    noTestQuestions = false;
    check=true;
    next = true;
    back = true;
    remember = true;
    learn = true;
    testBtn = true;

    if(position==0) back=false;

    if(question!=nullptr&&question->getQ_id()==-1)
    {
        next = false;
        remember = false;
    }

    if(user==nullptr)
    {
        learn=false;
        testBtn=false;
    }

    if(user!=nullptr&&(user->getLastAction()==LastActionLearn||user->getLastAction()==LastActionLearnTest))
    {
        learn = false;
    }

    if(user!=nullptr&&(user->getLastAction()==LastActionTest||user->getLastAction()==LastActionLearnTest))
    {
        testBtn = false;
    }

    if(user!=nullptr&&(user->getLastAction()==LastActionNone))
    {
        testBtn = false;
    }

    if(noTestWords==0)
    {
        testBtn = false;
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
}

//Funkcja kończąca Naukę i Test
void Session::stopLearnTest(const Status &status)
{
    exportWordsToDB(status);
    recalculateQuestions();
    LastAction userAction;
    if(status==StatusLearnMode) userAction=LastActionLearn;
    else if(status==StatusTestMode) userAction=LastActionTest;
    setUserLastAction(userAction);
    exportUserToDB();
    recalculateQuestions();
}

//Funkcja przeliczająca pytania w bazie danych
void Session::recalculateQuestions()
{
    noLearnWords = dbmanager->countQuestions(-1,user->getNoBox());
    noTestWords = dbmanager->countQuestions(-3,user->getNoBox(),courseDay);
    noAllWords = dbmanager->countQuestions(-4,user->getNoBox());

    toLearnWords=0;
    position=0;
}

//Funkcja pobierająca pytania do nauki słówek
void Session::learnWords()
{   
    if(position<ConstansMaxLearnWords-1+user->getUnknownQuestions()+addToLearn)
    {
        qList.resize(toLearnWords+1);
        qList[toLearnWords] = new Question(toLearnWords,user->getNoBox(),-1);
        question=qList.at(toLearnWords);
        position=toLearnWords;
        toLearnWords++;
    }
    else if(position==ConstansMaxLearnWords-1+user->getUnknownQuestions()+addToLearn)
    {
        qList.resize(toLearnWords+1);
        qList[toLearnWords] = new Question();
        question=qList.at(toLearnWords);
        position=toLearnWords;
        toLearnWords++;
    }
}

//Funkcja pokazująca następne pytanie
void Session::nextLearnBtn()
{
    if(position<toLearnWords-1)
    {
        position++;
        question=qList.at(position);
    }
    else if(position==noLearnWords-1)
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
        question=qTestList.at(position);
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
    recalculateQuestions();
    if(noTestWords==1)
    {
        qTestList.push_back(new Question(0,user->getNoBox(),-2));
    }
    else
    {
        for(int i=0;i<=courseDay;i++)
        {
            int noIQuestions = dbmanager->countQuestions(i,user->getNoBox());
            for(int k=0;k<noIQuestions;k++)
            {
                qTestList.push_back(new Question(k,user->getNoBox(),i));
            }
        }
    }
    nextTestBtn();
}

//Funkcja sprawdzająca odpowiedź
bool Session::checkAnswer(const QString &answer)
{
    if(answer==question->getQ_pl())
    {
        toggleIsChanged();
        return true;
    }
    else
    {
        MistakeWindow mistakewindow(answer,question->getQ_pl(),this);
        mistakewindow.exec();
        if(mistake)
        {
            toggleIsChanged();
            mistake=false;
            return true;
        }
    }
    return false;
}

//Funkcja do przeniesienia informacji do DB
void Session::exportWordsToDB(const Status &status)
{
    int size=0;
    int knownCounter=0;

    if(status == StatusLearnMode)
    {
        size = toLearnWords -1;
        for(int i=0;i<size;i++)
        {
            if(qList.at(i)->qet_isChanged())
            {
                dbmanager->setBox(qList.at(i)->getQ_id(),user->getNoBox());
                knownCounter++;
            }
        }
        if(knownCounter>=0)
        {
            int toAdd=ConstansMaxLearnWords-knownCounter-2+addToLearn;
            if(toAdd<=0)toAdd=0;
            else if (toAdd>noLearnWords) toAdd=noLearnWords;
            user->setUnknownQuestions(toAdd);
        }
        qDeleteAll(qList);
        qList.clear();
    }
    else if(status == StatusTestMode)
    {
        size = noTestWords;
        for(int i=0;i<size;i++)
        {
            if(qTestList.at(i)->qet_isChanged())
            {
                dbmanager->incrementUserLastFibonacci(qTestList.at(i)->getQ_id(),user->getNoBox());

                int fbox = qTestList.at(i)->get_fBox();
                unsigned long long newBox = fibonacci(fbox);

                dbmanager->setBox(qTestList.at(i)->getQ_id(),user->getNoBox(),newBox);
            }
        }
        qDeleteAll(qTestList);
        qTestList.clear();
    }
}

//Generator liczb losowych
int Session::randomInt(int min, int max)
{
    QTime time = QTime::currentTime();
    qsrand(static_cast<uint>(time.msec()));
    return qrand() % ((max + 1) - min) + min;
}

//Funkcja zwracająca numer numer ciągu Fibonacciego o złożoności O(n)
unsigned long long Session::fibonacci(int &n)
{
    if(n >= 0 && n < 3) return 1;

    unsigned long long result = 0;
    unsigned long long preOldResult = 1;
    unsigned long long oldResult = 1;

    for (int i=2;i<n;i++)
    {
        result = preOldResult + oldResult;
        preOldResult = oldResult;
        oldResult = result;
    }
    return result;
}

//Funcja zapisująca zmiany w profilu użytkownika do bazy danych
void Session::exportUserToDB()
{
    dbmanager->setUserLastAction(user->getUserName(),user->getLastAction());
    dbmanager->setUserLastUsed(user->getUserName(),date);
    dbmanager->setUserUnknownQuestions(user->getUserName(),user->getUnknownQuestions());
}
