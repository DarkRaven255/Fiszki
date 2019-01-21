#ifndef ENUMS_H
#define ENUMS_H

enum Status
{
    StatusMenu = 0,
    StatusLearnMode = 1,
    StatusTestMode = 2,
};

enum Constans
{
    ConstansMaxUsers = 5,
    ConstansMaxLearnWords = 10,
};

enum LastAction
{
    LastActionNone = 0,
    LastActionLearn = 1,
    LastActionTest = 2,
    LastActionLearnTest = 3,
};

#endif // ENUMS_H
