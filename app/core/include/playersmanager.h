#ifndef PLAYERS__MANAGER_H
#define PLAYERS__MANAGER_H

#include <QObject>

#include "character.h"

#include <unordered_map>

struct GameAtkEffects{
    effectParam allAtkEffects;
    QString atkName;
    QString launcher;
};

class PlayersManager
{
public:
    PlayersManager() = default;
    void InitHeroes();
    void InitBosses();
    void LoadAllEquipmentsJson();
    Character* GetCharacterByName(const QString& name);
    void AddGameEffectOnAtk(const Character* curPlayer, const QString &atkName);
    QStringList UpdateEffects();
    void ApplyEffects();
    void ApplyRegenStats();

    static QString FormatAtkOnEnnemy(const QString player1, const QString player2,  const QString &atkName, const int damage);
    static QString FormatAtkOnAlly(const QString player1, const QString player2,  const QString &atkName, const int damage);

    std::vector<Character*> m_HeroesList;
    std::vector<Character*> m_BossesList;
    Character* m_SelectedHero = nullptr;
    Character* m_ActivePlayer = nullptr;
    std::unordered_map<QString, Stuff> m_Equipments;
    std::unordered_map<QString, std::vector<GameAtkEffects>> m_AllEffectsOnGame;

};

#endif // PLAYERS__MANAGER_H
