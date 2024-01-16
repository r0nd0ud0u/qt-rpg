#ifndef PLAYERS__MANAGER_H
#define PLAYERS__MANAGER_H

#include <QObject>

#include "character.h"

class PlayersManager
{
public:
    PlayersManager() = default;
    std::vector<Character*> m_HeroesList;
    std::vector<Character*> m_BossesList;
};

#endif // PLAYERS__MANAGER_H
