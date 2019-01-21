#include "user.h"

User::User(const QString &setName, const QString &setNoBox, const long long &setStartDate,
           const long long &setlastUsed, const int &setLastAction, const int &setUnknownQuestions, QObject *parent):
  name(setName),
  noBox(setNoBox),
  lastUsed(setlastUsed),
  startDate(setStartDate),
  lastAction(setLastAction),
  unknownQuestions(setUnknownQuestions)
{

}

User::~User()
{

}

QString User::getUserName()
{
    return name;
}

QString User::getNoBox()
{
    return noBox;
}

long long User::getLastUsed()
{
    return lastUsed;
}

void User::setLastUsed(const int &setLastUsed)
{
    lastUsed=setLastUsed;
}

long long User::getStartDate()
{
    return startDate;
}

int User::getLastAction()
{
    return lastAction;
}

void User::setLastAction(const int &action)
{
    lastAction=action;
}

int User::getUnknownQuestions()
{
    return unknownQuestions;
}

void User::setUnknownQuestions(const int &setUnknownQuestions)
{
    unknownQuestions = setUnknownQuestions;
}
