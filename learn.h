#ifndef LEARN_H
#define LEARN_H

#include "session.h"

class Learn : public Session
{
public:
    Learn();

    void learnWords();
    void nextLearnBtn();
    void backLearnBtn();
};

#endif // LEARN_H
