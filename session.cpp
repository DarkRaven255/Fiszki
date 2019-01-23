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

//Funkcja generująca losową tablicę bez powtórzeń
void Session::randomTable()
{
    bool isRepeated;
    int random;
    recalculateQuestions();
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
qDebug()<<user->getLastUsed()<<date;
    if (user->getLastUsed()<date)
    {

        user->setLastAction(0);
        dbmanager->setUserLastAction(user->getUserName(),user->getLastAction());
        if (user->getLastUsed()>user->getStartDate())
        {
            addToLearn=MaxLearnWords-2;
        }
    }
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
    if(user->getLastAction()==None)
    {
        user->setLastAction(action);
    }
    else if((user->getLastAction()==Learn&&action==Test)||(user->getLastAction()==Test&&action==Learn))
    {
        user->setLastAction(LearnTest);
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
    //if(position>0) back=true;

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

    if(user!=nullptr&&(user->getLastAction()==Learn||user->getLastAction()==LearnTest))
    {
        learn = false;
    }

    if(user!=nullptr&&(user->getLastAction()==Test||user->getLastAction()==LearnTest))
    {
        testBtn = false;
    }

//    if(position==noMinusOneWords-1) next=false;
//    if(position<noMinusOneWords-1) next=true;
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

//    if(position>noTestWords)
//    {
//        noTestQuestions=true;
//        check=false;
//    }

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
    noMinusOneWords=dbmanager->countQuestions(-1,user->getNoBox(),0);
    noTestWords=dbmanager->countQuestions(-2,user->getNoBox(),0);

    toLearnWords=0;
    position=0;
}

//Funkcja pobierająca pytania do nauki słówek
void Session::learnWords()
{   
    if(position<MaxLearnWords-1+user->getUnknownQuestions()+addToLearn)
    {
        qList.resize(toLearnWords+1);
        qList[toLearnWords] = new Question(toLearnWords,user->getNoBox(),-1);
        question=qList.at(toLearnWords);
        position=toLearnWords;
        toLearnWords++;
    }
    else if(position==MaxLearnWords-1+user->getUnknownQuestions()+addToLearn)
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
        question=qList.at(position);
        position++;
    }
    else
    {
        position++;
    }
}

//Funkcja pobierająca pytania do testu
//void Session::testWords()
//{
//    qList.resize(noTestWords);
//    if(noTestWords==1)
//    {
//        qList[0] = new Question(0,user->getNoBox(),-2);
//    }
//    else
//    {
//        randomTable();
//        for(int i=0;i<noTestWords;i++)
//        {
//            qList[i] = new Question(testWordsList.at(i),user->getNoBox(),-2);
//        }
//    }
//    nextTestBtn();
//}
void Session::testWords()
{
    long long courseDay = date - user->getStartDate();
    qDebug()<<"dnie"<<courseDay;
    int counter =0;
    noTestWords = dbmanager->countQuestions(-3,user->getNoBox(),courseDay);
    qDebug()<<noTestWords;
    if(noTestWords==1)
    {
        qList.push_back(new Question(0,user->getNoBox(),-2));
    }
    else
    {
        for(int i=0;i<noTestWords;i++)
        {
            int noIQuestions = dbmanager->countQuestions(i,user->getNoBox(),NULL) + counter;
            int k=0;
            int j=counter;
//            qDebug()<<i<<"noIQ"<<noIQuestions;
            for(j,k;j<noIQuestions;j++,k++)
            {
                qList.push_back(new Question(k,user->getNoBox(),i));
            }
            counter=j;
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
    nextTestBtn();
}

//Funkcja do przeniesienia informacji do DB
void Session::exportBoxToDB(const Status &status)
{
    int size=0;
    int unknownCounter=0;
    //qDebug()<<status;

    if(status == LearnMode)
    {
        size = toLearnWords -1;
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
            unknownCounter++;
        }
        delete qList.at(i);
    }
//tu ma byc >= pacanie popraw to potem
    if(unknownCounter> 0 && status == LearnMode)
    {
        int toAdd=MaxLearnWords-unknownCounter-2+addToLearn;
        if(toAdd<=0)toAdd=0;
        dbmanager->setUnknownQuestions(user->getUserName(),toAdd);
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
unsigned long long Session::fibonacci(const int &n)
{
    if(n <= 0) return 0;
    if(n > 0 && n < 3) return 1;

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
