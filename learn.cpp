#include "learn.h"

Learn::Learn()
{

}

//Funkcja pobierająca pytania do nauki słówek
void Learn::learnWords()
{
    qList.resize(toLearnWords+1);
    qList[toLearnWords] = new Question(toLearnWords,-1);
    question=qList.at(toLearnWords);
    position=toLearnWords;
    toLearnWords++;
}

//Funkcja pokazująca następne pytanie
void Learn::nextLearnBtn()
{
    if(position<toLearnWords-1)
    {
        position++;
        question=qList.at(position);
    }
    else if(position==noMinusOneWords-1)
    {
        question=qList.at(position);
    }
    else
    {
        learnWords();
    }
}

//Funkcja cofająca pytanie
void Learn::backLearnBtn()
{
    position--;
    question=qList.at(position);
}
