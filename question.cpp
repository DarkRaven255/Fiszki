#include "question.h"

Question::Question(const int &noQuestion, const QString &userBox, const int &noBox):
    isChanged(false)
{
    dbmanager->returnQuestion(noQuestion,noBox,userBox,q_id,q_en,e_en,q_pl,e_pl);
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

bool Question::qet_isChanged()
{
    return isChanged;
}

void Question::set_isChanged()
{
    isChanged=true;
}
