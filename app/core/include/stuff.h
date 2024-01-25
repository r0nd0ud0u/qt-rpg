#ifndef STUFF_H
#define STUFF_H

#include "common.h"

#include <QString>

enum class Body {
    head,
    necklace,
    chest,
    pants,
    shoes,
    arm_left,
    arm_right,
    weapon_left,
    weapon_right,
    ring
};

class Stuff
{
public:
    Stuff() = default;

    Stats m_Stats = {};
    Body m_type;
    QString m_Name;
    bool m_IsWorn = false;
};

#endif // STUFF_H
