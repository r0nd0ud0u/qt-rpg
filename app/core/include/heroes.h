#ifndef HEROES_H
#define HEROES_H

#include <QObject>

#include "character.h"

class Heroes
{
public:
    Heroes();
    std::vector<Character*> m_HeroesList;
};

#endif // HEROES_H
