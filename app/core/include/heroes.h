#ifndef HEROES_H
#define HEROES_H

#include <QObject>

#include "character.h"

class Heroes
{
public:
    Heroes() = default;
    std::vector<Character*> m_HeroesList;
};

#endif // HEROES_H
