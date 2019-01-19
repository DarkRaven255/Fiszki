#include "user.h"

User::User(const QString &setName, const QString &setNoBox,
           const long long &setStartDate, const long long &setlastUsed, QObject *parent):
  name(setName),
  noBox(setNoBox),
  lastUsed(setlastUsed),
  startDate(setStartDate)
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
