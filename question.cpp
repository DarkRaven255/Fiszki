#include "question.h"

Question::Question(QObject *parent, const int &noQuestion, const int &noBox):
    QObject(parent)
{
    dbmanager->returnQuestion(noQuestion,noBox,q_id,q_en,e_en,q_pl,e_pl);
}

int Question::getQ_id()
{
    return q_id;
}

QString Question::getQ_en()
{
    return q_en;
}

QString Question::getE_en()
{
    return e_en;
}

QString Question::getQ_pl()
{
    return q_pl;
}

QString Question::getE_pl()
{
    return e_pl;
}
