#ifndef PLAYERS__MANAGER_H
#define PLAYERS__MANAGER_H

#include <QObject>

#include "character.h"

#include <unordered_map>

class PlayersManager
{
public:
    PlayersManager() = default;
    void InitHeroes();
    void InitBosses();
    void LoadAllEquipmentsJson();
    Character* GetCharacterByName(const QString& name);

    std::vector<Character*> m_HeroesList;
    std::vector<Character*> m_BossesList;
    Character* m_SelectedHero = nullptr;
    Character* m_ActivePlayer = nullptr;
    std::unordered_map<QString, Stuff> m_Equipments;

};

#endif // PLAYERS__MANAGER_H
