#ifndef PLAYERS__MANAGER_H
#define PLAYERS__MANAGER_H

#include <QObject>

#include "character.h"

class PlayersManager
{
public:
    PlayersManager() = default;
    void InitHeroes();
    void InitBosses();

    std::vector<Character*> m_HeroesList;
    std::vector<Character*> m_BossesList;
    Character* m_SelectedHero = nullptr;

};

#endif // PLAYERS__MANAGER_H
