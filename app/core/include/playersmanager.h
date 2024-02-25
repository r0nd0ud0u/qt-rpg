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
  QString target;
  int launchingTurn = 0;
};

struct TargetInfo{
    QString m_Name;
    bool m_IsTargeted = false;
    bool m_IsBoss = false;
};

class PlayersManager {
public:
  PlayersManager() = default;
  void InitHeroes();
  void InitBosses();
  void LoadAllEquipmentsJson();
  Character *GetCharacterByName(const QString &name);
  void AddGameEffectOnAtk(const QString &launcherName, const AttaqueType &atk,
                          const QString &targetName,
                          const std::vector<effectParam> &effects,
                          const int currentTurn);
  QStringList RemoveTerminatedEffectsOnPlayer(const QString &curPlayerName);
  QStringList ApplyEffectsOnPlayer(const QString &curPlayerName,
                                   const int currentTurn);
  void ApplyRegenStats(const characType& type);

  static QString FormatAtkOnEnnemy(const int damage);
  static QString FormatAtkOnAlly(const int damage);
  static QString FormatAtk(const QString player2, const QString &atkName);
  int GetNbOfStatsInEffectList(const Character *chara,
                               const QString &statsName) const;
  void ResetCounterOnOneStatsEffect(const Character *chara,
                                    const QString &statsName);
  QString DeleteOneBadEffect(const Character *chara);
  void DecreaseCoolDownEffects(const QString &curPlayerName);
  QString DeleteAllBadEffect(const Character *chara);
  void ImproveHotsOnPlayers(const int valuePercent,
                            const characType launcherType);
  void IncrementCounterEffect();
  QStringList CheckDiedPlayers(const characType& launcherType);
  void AddSupAtkTurn(const characType &launcherType, std::vector<QString>& playerOrderTable)const;
  std::pair<bool, QString> IsDodging(const std::vector<TargetInfo>& targetList); // pair1 isDodging?, pair2 playerName
  void AddExpForHeroes(const int exp);

  std::vector<Character *> m_HeroesList;
  std::vector<Character *> m_BossesList;
  Character *m_SelectedHero = nullptr;
  Character *m_ActivePlayer = nullptr;
  std::unordered_map<QString, Stuff> m_Equipments;
  std::unordered_map<QString, std::vector<GameAtkEffects>>
      m_AllEffectsOnGame; // key target
};

#endif // PLAYERS__MANAGER_H
