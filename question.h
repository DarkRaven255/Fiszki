#ifndef QUESTION_H
#define QUESTION_H

#include <QObject>
#include "dbmanager.h"

class Question : public QObject
{
    Q_OBJECT
public:
    Question(QObject *parent, const int &noQuestion, const int &noBox);
    int getQ_id();
    QString getQ_en();
    QString getE_en();
    QString getQ_pl();
    QString getE_pl();
    int qetQ_box();

    //DO POPRAWY!
    void setQ_box();

private:
    int q_id;
    QString q_en;
    QString e_en;
    QString q_pl;
    QString e_pl;
    int q_box;

    DbManager *dbmanager;
};

#endif // QUESTION_H
