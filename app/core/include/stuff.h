#ifndef STUFF_H
#define STUFF_H

#include "common.h"

#include <QString>

class Stuff
{
public:
    Stuff() = default;

    Stats m_Stats = {};
    QString m_Name;
};

#endif // STUFF_H
