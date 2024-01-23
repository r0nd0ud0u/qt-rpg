#ifndef PLAYERS__MANAGER_H
#define PLAYERS__MANAGER_H

#include <QObject>

#include "character.h"
#include "effect.h"

class PlayersManager
{
public:
    PlayersManager() = default;
    void InitHeroes();
    void InitBosses();

    std::vector<Character*> m_HeroesList;
    std::vector<Character*> m_BossesList;
    std::vector<Effect*> m_EffectsList;
    Character* m_SelectedHero = nullptr;

};

#endif // PLAYERS__MANAGER_H
