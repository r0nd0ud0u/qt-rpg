#ifndef PLAYERS__MANAGER_H
#define PLAYERS__MANAGER_H

#include <QObject>

#include "character.h"

#include <unordered_map>
#include <vector>

#include "rust-rpg-bridge/players_manager.h"

class GameAtkEffects {
public:
  effectParam allAtkEffects;
  AttaqueType atk;
  QString launcher;
  QString target;
  int launchingTurn = 0;
};

class PlayersManager {
public:
  PlayersManager() = default;
  void InitHeroes(std::vector<Character*>& heroList);
  void InitBosses(std::vector<Character*>& bossList);
  void ClearHeroBossList();
  void LoadEquipmentsJson(const QString& filepath);
  Character *GetActiveCharacterByName(const QString &name);
  Character *GetCharacterByName(const QString &name);
  void AddGameEffectOnAtk(const QString &launcherName, const AttaqueType &atk,
                          const QString &targetName,
                          const std::vector<effectParam> &effects,
                          const int currentTurn);
  QStringList RemoveTerminatedEffectsOnPlayer(const QString &curPlayerName);
  QStringList ApplyEffectsOnPlayer(const QString &curPlayerName,
                                   const int currentTurn,
                                   const bool fromLaunch);
  void ApplyRegenStats(const characType &type);

  static QString FormatAtkOnEnnemy(const int damage);
  int GetNbOfStatsInEffectList(const Character *chara,
                               const QString &statsName) const;
  void ResetCounterOnOneStatsEffect(const Character *chara,
                                    const QString &statsName);
  QString DeleteOneBadEffect(const Character *chara);
  QString DeleteAllBadEffect(const Character *chara);
  void ImproveHotsOnPlayers(const int valuePercent,
                            const characType launcherType);
  void IncrementCounterEffect();
  QStringList CheckDiedPlayers(const characType &launcherType);
  void AddSupAtkTurn(const characType &launcherType,
                     std::vector<QString> &playerOrderTable) const;
  std::tuple<bool, QString, QStringList>
  IsDodging(const std::vector<TargetInfo *> &targetList,
            const AttaqueType &atk); // pair1 isDodging?, pair2 playerName
  void AddExpForHeroes(const int exp);
  QString ProcessDamageTXHealNeedyAlly(const characType &launcherType,
                                       const int damageTX);
  std::vector<Character *> *GetPlayerListByType(const characType &launcherType);
  std::optional<std::vector<QString>>
  GetAllDeadliestAllies(const characType &launcherType) const;
  void ProcessIsRandomTarget() const;
  void ResetIsFirstRound() const;
  bool UpdateStartingPlayers(const bool isLoadingGame);
  std::vector<Stuff> LootNewEquipments(const QString &name);
  void InitRandomEquip();
  std::optional<std::pair<QString, int>> GetHeroMostAggro() const;
  void OutputCharactersInJson(const std::vector<Character *> &l,
                              const QString &outputPath) const;
  void LoadAllCharactersJson(const bool isLoadingGame, const QString& pathForLoadingGame);
  void ResetAllEffectsOnPlayer(const Character *chara);
  int GetNbOfActiveHotsOnHeroes() const;
  void SetSelectedHero(const QString &name);
  QString ProcessNewDefaultName() const;
  int GetMaxIndexDefaultName() const;
  void OutputAllOnGoingEffectToJson(const QString& filepath) const;
  void Reset();
  void LoadAllEffects(const QString& filepath);

  // Available characters to create a party
  std::vector<Character *> m_AllHeroesList;
  std::vector<Character *> m_AllBossesList;
  // Used characters in a party
  std::vector<Character *> m_HeroesList;
  std::vector<Character *> m_BossesList;
  Character *m_SelectedHero = nullptr;
  Character *m_ActivePlayer = nullptr;
  std::unordered_map<QString, std::map<QString, Stuff>>
      m_Equipments; // key 1 body name, key2 {equipName, equip value-stats}
  std::unordered_map<QString, std::map<QString, Stuff>>
      m_LootEquipments; // key 1 body name, key2 {equipName, equip value-stats}
  std::unordered_map<QString, std::vector<GameAtkEffects>>
      m_AllEffectsOnGame; // key target
  std::unordered_map<QString, std::vector<QString>> m_RandomEquipName;
};
#endif // PLAYERS__MANAGER_H
