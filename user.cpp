#include "user.h"

User::User(QString setName,QString setNoBox, QObject *parent):
  name(setName),
  noBox(setNoBox)
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
