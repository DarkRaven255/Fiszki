#include "user.h"

User::User(QString setName,QObject *parent):
  name(setName)/*,
  noBox(setNoBox)*/
{

}

User::~User()
{

}

QString User::getUserName()
{
    return name;
}

int User::getNoBox()
{
    return noBox;
}
