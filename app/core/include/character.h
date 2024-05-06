#ifndef CHARACTER_H
#define CHARACTER_H

#include <QColor>
#include <QString>

#include <deque>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "bossclass.h"
#include "common.h"
#include "effect.h"
#include "stuff.h"
#include "utils.h" // for EffectsTypeNb struct

#include "rust-rpg-bridge/attaque.h"
#include "rust-rpg-bridge/character.h"
#include "rust-rpg-bridge/powers.h"

// forward declaration
class GameAtkEffects;

enum class characType { Hero, Boss };

class AttaqueType {
public:
  QString name = "Atq";
  uint8_t level = 1;
  uint32_t manaCost = 0;
  uint32_t vigorCost = 0;
  uint32_t berseckCost = 0;
  QString target = TARGET_ENNEMY;
  QString reach = REACH_INDIVIDUAL;
  QString namePhoto = "default.png";
  std::vector<effectParam> m_AllEffects = {};
  QString form = STANDARD_FORM;
  AttaqueNature nature;

  static constexpr double COEFF_CRIT_STATS = 1.5;
};

enum class InventoryType { healthPotion, manaPotion, enumSize };
enum class BufTypes {
  defaultBuf = 0,
  damageRx,
  damageTx,
  healTx,
  healRx,
  damageCritCapped,
  powPhyBuf,
  nextHealAtkIsCrit,
  multiValue,
  applyEffectInit,
  changeByHealValue,
  enumSize
};
enum class amountType {
  damageRx = 0,
  damageTx,
  healRx,
  healTx,
  overHealRx,
  enumSize
};

class Character {
public:
  Character();
  Character(const QString name, const characType type, const Stats &stats);

  void InitTables();
  void ProcessCost(const QString &atkName);
  void AddAtq(const AttaqueType &atq);
  void LoadAtkJson();
  void LoadStuffJson();
  void ApplyEquipOnStats(const std::vector<GameAtkEffects> &allGae);
  // std::pair<bool, QString>, 1: canBeLaunched, 2: reason in case it is false
  std::pair<bool, std::optional<QString>>
  CanBeLaunched(const AttaqueType &atk) const;

  // Effect
  std::pair<QString, std::vector<effectParam>>
  ApplyOneEffect(Character *target, effectParam &effect, const bool fromLaunch,
                 const AttaqueType &atk, const bool reload = false,
                 const bool isCrit = false);
  std::tuple<bool, QStringList, std::vector<effectParam>>
  ApplyAtkEffect(const bool targetedOnMainAtk, const AttaqueType &atk,
                 Character *target,
                 const bool isCrit); // value1: conditions fulfilled ?, value2 :
                                     // logs after applying effects
  void RemoveMalusEffect(const effectParam &ep);

  QString RegenIntoDamage(const int atkValue, const QString &statsName) const;

  static void
  SetStatsOnEffect(StatsType &stat, const int value, const bool isPercent,
                   const bool updateEffect); // TODO à sortir dans un common
                                             // pour gerer les stats?
  std::pair<bool, QString> IsDodging(const AttaqueType &atk) const;
  void UsePotion(const QString &statsName);
  void AddExp(const int newXp);
  void SetEquipment(const std::unordered_map<QString, QString> &);
  void UpdateEquipmentOnJson() const;
  void ApplyEffeftOnStats(const bool updateEffect,
                          const std::vector<GameAtkEffects> &allGae);
  std::pair<bool, int>
  ProcessCriticalStrike(const AttaqueType &atk); // return isCrit, random number
  void ResetBuf(const BufTypes &bufType);
  void SetValuesForThalia(const bool isBear);
  std::optional<int> GetRandomAtkNumber();
  std::optional<QString> FormatStringRandAtk(const int rand);
  void SortAtkByLevel();
  std::pair<int, int> ProcessCurrentValueOnEffect(
      effectParam &ep, const int nbOfApplies, const Stats &launcherStats,
      const bool launch, Character *target,
      const bool isCrit) const; // value 1 isCrit, value 2 total amount value 3
      // maxamount, value 4: crit randNb
  bool IsDead() const;
  void ProcessDeath();
  int GetMaxNbOfApplies(const AttaqueType &atk) const;
  std::optional<EffectsTypeNb> GetBufDebufNumbers() const;

  // Temporary
  std::vector<effectParam> LoadThaliaTalent() const;
  std::vector<effectParam> LoadAzrakTalent() const;
  std::vector<effectParam> LoadThrainTalent() const;
  std::vector<effectParam> LoadElaraTalent() const;

  QString m_Name = "default";
  characType m_type = characType::Hero;
  Stats m_Stats;
  std::unordered_map<QString, Stuff>
      m_WearingEquipment; // key: body, value: equipmentName
  std::unordered_map<QString, AttaqueType>
      m_AttakList; // key: attak name, value: AttakType struct
  // That vector contains all the atks from m_AttakList and is sorted by level.
  std::vector<AttaqueType> m_AtksByLevel;
  int m_Level = 1;
  int m_Exp = 0;
  int m_NextLevel = 100;
  std::deque<int> m_LastAggros; // keep the last five aggros and sum them
  std::vector<QString> m_Forms = std::vector<QString>{STANDARD_FORM};
  QString m_SelectedForm = STANDARD_FORM;
  QString m_ColorStr = "dark";

  QColor color = QColor("dark");
  // Buf
  std::vector<Buffers *> m_AllBufs;
  /// Explain if the last attak has been critical or not
  bool m_isLastAtkCritical = false;
  std::vector<std::unordered_map<uint64_t, uint64_t>> m_LastTxRx;
  Powers m_Power;
  ExtendedCharacter *m_ExtCharacter = nullptr;
  BossClass m_BossClass;

private:
  static void ProcessAddEquip(StatsType &charStat, const StatsType &equipStat);
  static void ProcessRemoveEquip(StatsType &charStat,
                                 const StatsType &equipStat);
  QString ProcessOutputLogOnEffect(const effectParam &ep, const int amount,
                                   const bool fromLaunch, const int nbOfApplies,
                                   const QString &atkName,
                                   const int maxAmount) const;
  int ProcessDecreaseOnTurn(const effectParam &ep) const;
  QString ProcessDecreaseByTurn(const effectParam &ep) const;
  static int DamageByAtk(const Stats &launcherStats, const Stats &targetStats,
                         const bool isMagicAtk, const int atkValue,
                         const int nbTurns);
  int ProcessBerseckOnRxAtk(const int nbOfApplies);
  std::pair<QString, int> ProcessEffectType(
      effectParam &effect, Character *target,
      const AttaqueType &atk); // pair1 output log, pair2 nbOfApplies
  QString ProcessAggro(const int atkValue, const int aggroValue);
  void UpdateStatsToNextLevel();
  void UpdateBuf(const BufTypes &bufType, const int value, const bool isPercent,
                 const QString &stat);
  static int UpdateDamageByBuf(const Buffers *bufDmg, const int value);
};

#endif // CHARACTER_H
