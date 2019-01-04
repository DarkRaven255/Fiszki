#include "dbmanager.h"

DbManager::DbManager(const QString &name)
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(name);

    if (!database.open())
    {
       qDebug() << "ERROR: " << database.lastError();
    }
    if(!database.tables().contains(QLatin1String("users")))
    {
        QSqlQuery query("CREATE TABLE users(id integer primary key, name text, level integer)");
        if(!query.isActive())
        {
            qDebug() << "ERROR: " << query.lastError().text();
        }
    }
    if(!database.tables().contains(QLatin1String("questions")))
    {
        qDebug() << "NO QUESTIONS IN DB!";
//        QSqlQuery query("CREATE TABLE questions(id integer primary key, question_en text, explanation_en text, question_pl text, explanation_pl text, box integer)");
    }

}

DbManager::~DbManager()
{

}

bool DbManager::addUser(const QString &name)
{
    if(!findUser(name))
    {
        QSqlQuery query;

        query.prepare("INSERT INTO users (name, level) VALUES (:name, :level)");
        query.bindValue(":name", name);
        query.bindValue(":level", 0);
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

QStringList DbManager::displayAllUsers()
{
    QStringList userlist;
    QSqlQuery query("SELECT * FROM users");
    int idName = query.record().indexOf("name");
//    int idLevel = query.record().indexOf("level");
    while (query.next())
    {
//       qDebug()<<query.value(idName).toString()<<"    "<<query.value(idLevel).toString();
       userlist += (query.value(idName).toString());
    }
    return userlist;
}

void DbManager::closeUserDB()
{
    database.close();
    qDebug() << "DB closed";
}

void DbManager::returnQuestion(int &noQuestion, int &noBox, int &q_id, QString &q_en, QString &e_en, QString &q_pl, QString &e_pl)
{
    QSqlQuery query;

    switch(noBox)
    {
    case -1: case 0: case 1: case 2: case 3: case 4: case 5:
        query.prepare("SELECT * FROM questions WHERE box is (:box)");
        query.bindValue(":box",noBox);
        break;
    case 6:
        query.prepare("SELECT * FROM questions WHERE box is not (:box)");
        query.bindValue(":box",-1);
        break;
    }

    query.exec();
    query.first();

    int idQuestion=query.record().indexOf("id");
    int idQuestionEN = query.record().indexOf("question_en");
    int idExplanationEN = query.record().indexOf("explanation_en");
    int idQuestionPL = query.record().indexOf("question_pl");
    int idExplanationPL = query.record().indexOf("explanation_pl");

    query.seek(noQuestion);

    q_id=query.value(idQuestion).toInt();
    q_en=query.value(idQuestionEN).toString();
    e_en=query.value(idExplanationEN).toString();
    q_pl=query.value(idQuestionPL).toString();
    e_pl=query.value(idExplanationPL).toString();
}

void DbManager::setBox(int &q_id)
{
    QSqlQuery query;
    query.prepare("UPDATE questions SET box = box+1 WHERE id = (:q_id)");
    query.bindValue(":q_id", q_id);
    query.exec();
}

int DbManager::countQuestions(int noBox)
{
    QSqlQuery query;
    int noQuestions=0;

    switch(noBox)
    {
    case -1: case 0: case 1: case 2: case 3: case 4: case 5:
        query.prepare("SELECT * FROM questions WHERE box is (:box)");
        query.bindValue(":box",noBox);
        break;
    case 6:
        query.prepare("SELECT * FROM questions WHERE box is not (:box)");
        query.bindValue(":box",-1);
        break;
    }

    query.exec();
    if(query.last())
    {
        noQuestions = query.at()+1;
        query.first();
    }
    return noQuestions;
}
