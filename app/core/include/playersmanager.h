#ifndef PLAYERS__MANAGER_H
#define PLAYERS__MANAGER_H

#include <QObject>

#include "character.h"

#include <unordered_map>
#include <vector>

struct GameAtkEffects {
  effectParam allAtkEffects;
  AttaqueType atk;
  QString launcher;
};

class PlayersManager {
public:
  PlayersManager() = default;
  void InitHeroes();
  void InitBosses();
  void LoadAllEquipmentsJson();
  Character *GetCharacterByName(const QString &name);
  void AddGameEffectOnAtk(
      const QString &launcherName, const AttaqueType &atk,
      const QString &targetName, const std::vector<effectParam> &effects);
  QStringList RemoveTerminatedEffects();
  QStringList ApplyEffects();
  void ApplyRegenStats();

  static QString FormatAtkOnEnnemy(const int damage);
  static QString FormatAtkOnAlly(const int damage);
  static QString FormatAtk(const QString player2, const QString &atkName);
  int GetNbOfStatsInEffectList(const Character *chara,
                               const QString &statsName) const;
  void ResetCounterOnOneStatsEffect(const Character *chara,
                                    const QString &statsName);
  QString DeleteOneBadEffect(const Character *chara);
  void DecreaseCoolDownEffects();
  QString DeleteAllBadEffect(const Character *chara);
  void ImproveHotsOnPlayers(const int valuePercent,
                            const characType launcherType);

  std::vector<Character *> m_HeroesList;
  std::vector<Character *> m_BossesList;
  Character *m_SelectedHero = nullptr;
  Character *m_ActivePlayer = nullptr;
  std::unordered_map<QString, Stuff> m_Equipments;
  std::unordered_map<QString, std::vector<GameAtkEffects>> m_AllEffectsOnGame;
};

#endif // PLAYERS__MANAGER_H
