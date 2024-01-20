#ifndef STUFF_H
#define STUFF_H

#include "common.h"

#include <QString>
#include <vector>

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

    static QString GetStringBody(const Body& body);
};

#endif // STUFF_H
