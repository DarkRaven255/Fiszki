#include "dbmanager.h"
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtDebug>
#include "enums.h"

DbManager::DbManager(const QString &name)
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(name);

    if (!database.open())
    {
       qDebug() << "ERROR: " << database.lastError();
    }
    if(!database.tables().contains(QLatin1String("questions")))
    {
        qDebug() << "NO QUESTIONS IN DB!";
    }
}

DbManager::~DbManager()
{

}

//Funkcja dodająca nowego użytkownika
bool DbManager::addUser(const QString &name, const int &noBox, const long long &setStartDate)
{
    if(!findUser(name))
    {
        QSqlQuery query;

        query.prepare("INSERT INTO users (name, noBox, start_date, last_used) VALUES (:name, :noBox, :setStartDate, :setLastUsed)");
        query.bindValue(":name", name);
        query.bindValue(":noBox", noBox);
        query.bindValue(":setStartDate", setStartDate);
        query.bindValue(":setLastUsed", setStartDate);
        if(query.exec())
        {
            return true;
        }
        else
        {
             qDebug() << "addUser error:  " << query.lastError();
        }
    }
    return false;
}

//Funkcja szukająca użytkownika po jego nazwie
bool DbManager::findUser(const QString &name)
{
    QSqlQuery query;
    query.prepare("SELECT name FROM users WHERE name = (:name)");
    query.bindValue(":name", name);

    if (query.exec())
    {
       if (query.next())
       {
          return true;
       }
    }
    return false;
}

//Funkcja zwracająca wartość o którą pytamy przez zmienna parameter
long long DbManager::returnUserInfo(const QString &name, const QString parameter)
{
    QSqlQuery query;
    query.prepare("SELECT " + parameter + " FROM users WHERE name = (:name)");
    query.bindValue(":name", name);
    query.exec();
    query.first();
    int idInfo = query.record().indexOf(parameter);
    return query.value(idInfo).toInt();
}

//Funkcja szukająca konkretnego pytania po nazwie angielskiej
bool DbManager::findWord(const QString &q_en)
{
    QSqlQuery query;
    query.prepare("SELECT question_en FROM questions WHERE question_en = (:Q_EN)");
    query.bindValue(":Q_EN", q_en);

    if (query.exec())
    {
       if (query.next())
       {
          return true;
       }
    }
    return false;
}

//Funkcja dodająca nowe słówka do bazy danych
void DbManager::addWord(const QString &q_en, const QString &e_en, const QString &q_pl, const QString &e_pl)
{
    QSqlQuery query;
    query.prepare("INSERT INTO questions (question_en, explanation_en, question_pl, explanation_pl) VALUES (:Q_EN, :E_EN, :Q_PL, :E_PL)");
    query.bindValue(":Q_EN", q_en);
    query.bindValue(":E_EN", e_en);
    query.bindValue(":Q_PL", q_pl);
    query.bindValue(":E_PL", e_pl);
    query.exec();
}

//Funkcja usuwająca wszystkich użytkowników
bool DbManager::removeAllUsers()
{
    QSqlQuery query;
    query.prepare("DELETE FROM users");
    if(query.exec())
    {
        return true;
    }
    return false;
}

//Funkcja usuwająca danego użytkownika
bool DbManager::removeUser(const QString &name)
{
    if (findUser(name))
    {
       QSqlQuery query;
       query.prepare("DELETE FROM users WHERE name = (:name)");
       query.bindValue(":name", name);
       bool success = query.exec();

       if(!success)
       {
           qDebug() << "removeUser error: "<< query.lastError();
           return false;
       }
    }
    return true;
}

//Funkcja ustawiająca -1 w boxie dla danego użytkownika
void DbManager::resetUserBox(const QString &userBox)
{
    QSqlQuery query;
    query.exec("UPDATE questions SET " + userBox + "=-1");
    query.exec("UPDATE questions SET f" + userBox + "=0");
}

//Funkcja ustawiająca ostatnią aktywność użytkownika w bazie danych
void DbManager::setUserLastAction(const QString &name, const int &action)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET last_action = (:ACTION) WHERE name = (:name)");
    query.bindValue(":ACTION", action);
    query.bindValue(":name", name);
    query.exec();
}

void DbManager::setUserLastUsed(const QString &name, const long long &lastUsed)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET last_used = (:LASTUSED) WHERE name = (:name)");
    query.bindValue(":LASTUSED", lastUsed);
    query.bindValue(":name", name);
    query.exec();
}

void DbManager::setUserUnknownQuestions(const QString &name, const int &number)
{
    QSqlQuery query;
    query.prepare("UPDATE users SET unknown_questions = unknown_questions + (:NUMBER) WHERE name = (:NAME)");
    query.bindValue(":NAME", name);
    query.bindValue(":NUMBER", number);
    query.exec();
}

void DbManager::incrementUserLastFibonacci(const int &q_id, const QString &userBox)
{
    QSqlQuery query;
    query.prepare("UPDATE questions SET f"+ userBox +" = f"+ userBox +" +1 WHERE id = (:q_id)");
    query.bindValue(":q_id", q_id);
    query.exec();
}

//Funkcja zwracająca listę zawierającą wszystkich użytkowników
QStringList DbManager::returnUserList()
{
    QStringList userlist;
    QSqlQuery query("SELECT * FROM users");
    int idName = query.record().indexOf("name");
    while (query.next())
    {
       userlist += (query.value(idName).toString());
    }
    return userlist;
}

//Funkcja zamykająca bazę danych
void DbManager::closeUserDB()
{
    database.close();
    qDebug() << "DB closed";
}

//Funkcja przekazująca pytanie odczytane z bazy danych
void DbManager::returnQuestion(const int &noQuestion, const int &noBox, const QString &userBox,
                               int &q_id, QString &q_en, QString &e_en, QString &q_pl, QString &e_pl, int &fBox)
{
    QSqlQuery query;

    switch(noBox)
    {
    case -1:/* case 0: case 1: case 2: case 3: case 4: case 5: case 6:*/
        query.prepare("SELECT * FROM questions WHERE " + userBox + " = (:noBox)");
        query.bindValue(":noBox",noBox);
        break;
    case -2:
        query.prepare("SELECT * FROM questions WHERE "+ userBox +" > -1 ORDER BY " + userBox);
        break;
//    case -3:
//        query.prepare("SELECT * FROM questions WHERE "+ userBox +" <= (:COURSEDAY)");
//        query.bindValue(":COURSEDAY",courseDay);
//        break;
    default:
        query.prepare("SELECT * FROM questions WHERE " + userBox + " = (:noBox)");
        query.bindValue(":noBox",noBox);
        break;
    }

    query.exec();
    query.first();
    query.seek(noQuestion);

    int idQuestion=query.record().indexOf("id");
    int idQuestionEN = query.record().indexOf("question_en");
    int idExplanationEN = query.record().indexOf("explanation_en");
    int idQuestionPL = query.record().indexOf("question_pl");
    int idExplanationPL = query.record().indexOf("explanation_pl");
    int idFBox = query.record().indexOf("f"+userBox);

    q_id=query.value(idQuestion).toInt();
    q_en=query.value(idQuestionEN).toString();
    e_en=query.value(idExplanationEN).toString();
    q_pl=query.value(idQuestionPL).toString();
    e_pl=query.value(idExplanationPL).toString();
    fBox=query.value(idFBox).toInt();
}

//Funkcja ustawiająca konkretny numer w pudełku dla danego pytania i użytkownika
void DbManager::setBox(const int &q_id, const QString &userBox, const unsigned long long &noBox)
{
    QSqlQuery query;
    if(noBox>0)
    {
        query.prepare("UPDATE questions SET "+ userBox +" = (:NOBOX) WHERE id = (:q_id)");
        query.bindValue(":NOBOX", noBox);
    }
    else
    {
        query.prepare("UPDATE questions SET "+ userBox +" = "+ userBox +" +1 WHERE id = (:q_id)");
    }
    query.bindValue(":q_id", q_id);
    query.exec();
}

//Funkcja zwracająca ilość słówek z bazie danych
int DbManager::countQuestions(const int &noBox, const QString &userBox, const long long &courseDay)
{
    QSqlQuery query;
    int noQuestions=0;

    switch(noBox)
    {
    case -1:/* case 0: case 1: case 2: case 3: case 4: case 5: case 6:*/
        query.prepare("SELECT * FROM questions WHERE " + userBox + " = (:noBox)");
        query.bindValue(":noBox",noBox);
        break;
    case -2:
        query.prepare("SELECT * FROM questions WHERE "+ userBox +" > -1");
        break;
    case -3:
        query.prepare("SELECT * FROM questions WHERE "+ userBox +" <= (:COURSEDAY) and "+ userBox + " is not -1");
        query.bindValue(":COURSEDAY",courseDay);
        break;
    default:
        query.prepare("SELECT * FROM questions WHERE " + userBox + " = (:noBox)");
        query.bindValue(":noBox",noBox);
        break;
    }

    query.exec();
    //qDebug() << "addUser error:  " << query.lastError();
    if(query.last())
    {
        noQuestions = query.at()+1;
        query.first();
    }
    return noQuestions;
}

//Funkcja zwracająca aktualną ilość użytkowników aplikacji
int DbManager::countUsers()
{
    QSqlQuery query;
    int noUsers=0;
    query.exec("SELECT * FROM users");
    if(query.last())
    {
        noUsers = query.at()+1;
        query.first();
    }
    return noUsers;
}

//Funkcja przekazująca wektor zawierający listę zajętych pudełek
void DbManager::returnBoxesInUse(QVector<int> &listFreeBoxes)
{
    int noUsers=countUsers();

    QSqlQuery query;
    query.exec("SELECT * FROM users");

    for(int i=0;i<noUsers && i<ConstansMaxUsers;i++)
    {
        query.seek(i);
        listFreeBoxes[i] = query.value(query.record().indexOf("noBox")).toInt();
    }
}
