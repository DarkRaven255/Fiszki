#include "session.h"
#include <QTime>
#include <QDebug>

Session::Session(QObject *parent):
    QObject(parent),
    question(nullptr),
    toLearnWords(0),
    testCounterQuestions(0),
    position(0),
    addToLearn(0),
    date(QDate::currentDate().toJulianDay()),
    user(nullptr)
{
    setUserList();
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
            addToLearn=ConstansMaxLearnWords-2;
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

void Session::setUserAction(const LastAction &action)
{
    if(user->getLastAction()==LastActionNone)
    {
        user->setLastAction(action);
    }
    else if((user->getLastAction()==LastActionLearn&&action==LastActionTest)||(user->getLastAction()==LastActionTest&&action==LastActionLearn))
    {
        user->setLastAction(LastActionLearnTest);
    }
    dbmanager->setUserLastAction(user->getUserName(),user->getLastAction());
    dbmanager->setUserLastUsed(user->getUserName(),date);
}

//Funkcja dodająca nowe pytania do bazy danych
void Session::addWord(const QString &q_en, const QString &e_en, const QString &q_pl, const QString &e_pl)
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
void Session::markWord()
{
    question->set_isChanged();
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

//Funkcja przeliczająca pytania w bazie danych
void Session::recalculateQuestions()
{
    noMinusOneWords=dbmanager->countQuestions(-1,user->getNoBox());
    noTestWords = dbmanager->countQuestions(-3,user->getNoBox(),courseDay);

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
    qDebug()<<"dnie"<<courseDay;
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
void Session::checkAnswer(const QString &answer)
{
    if(answer==question->getQ_pl())
    {
        markWord();
    }
}

//Funkcja do przeniesienia informacji do DB
void Session::exportBoxToDB(const Status &status)
{
    int size=0;
    int unknownCounter=0;

    if(status == StatusLearnMode)
    {
        size = toLearnWords -1;
        for(int i=0;i<size;i++)
        {
            if(qList.at(i)->qet_isChanged())
            {
                dbmanager->setBox(qList.at(i)->getQ_id(),user->getNoBox());
                unknownCounter++;
            }
        }
        if(unknownCounter>=0)
        {
            int toAdd=ConstansMaxLearnWords-unknownCounter-2+addToLearn;
            if(toAdd<=0)toAdd=0;
            dbmanager->setUserUnknownQuestions(user->getUserName(),toAdd);
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
    recalculateQuestions();
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
