#ifndef QUESTION_H
#define QUESTION_H

#include "dbmanager.h"

class Question
{
public:
    Question(const int &noQuestion, const QString &userBox, const int &noBox);
    Question();
    int getQ_id();
    QString getQ_en();
    QString getE_en();
    QString getQ_pl();
    QString getE_pl();

    bool qet_isChanged();
    void set_isChanged();

    int get_noBox;


private:
    int q_id;
    QString q_en;
    QString e_en;
    QString q_pl;
    QString e_pl;
    int noBox;

    bool isChanged;

    DbManager *dbmanager;
};

#endif // QUESTION_H
