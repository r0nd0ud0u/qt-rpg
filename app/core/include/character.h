#ifndef CHARACTER_H
#define CHARACTER_H

#include <QColor>
#include <QString>

#include <tuple>
#include <unordered_map>
#include <vector>

#include "bossclass.h"
#include "common.h"
#include "effect.h"
#include "stuff.h"
#include "utils.h" // for EffectsTypeNb struct
#include "statsingame.h"
#include "attack.h"

#include "rust-rpg-bridge/attaque.h"
#include "rust-rpg-bridge/character.h"
#include "rust-rpg-bridge/powers.h"

// forward declaration
class GameAtkEffects;

enum class characType { Hero, Boss };

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
  boostedByHots,
  enumSize
};
enum class amountType {
  damageRx = 0,
  damageTx,
  healRx,
  healTx,
  overHealRx,
  aggro,
  enumSize
};

const std::vector<QString> amountTypeStr{"damageRx", "damageTx", "healRx", "healTx", "overHealRx", "aggro"};

enum class CharacterClass { Standard = 0, Tank, enumSize };

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
  EffectOutcome ApplyOneEffect(Character *target, effectParam &effect, const bool fromLaunch,
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
  void UpdateEquipmentOnJson(const QString& dirPath) const;
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
  std::optional<AllEffectsType> GetBufDebuf() const;
  void InitAggroOnTurn(const int turnNb);
  void ProcessBlock(const bool isDodging);
  QString GetPhotoName() const;
  void UpdateByProcessedEffect(const EffectOutcome& outcome);

  // Temporary
  std::vector<effectParam> LoadThaliaTalent() const;
  std::vector<effectParam> LoadAzrakTalent() const;
  std::vector<effectParam> LoadThrainTalent() const;
  std::vector<effectParam> LoadElaraTalent() const;

  QString m_Name = "default";
  QString m_PhotoName;
  characType m_Type = characType::Hero;
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
  CharacterClass m_Class = CharacterClass::Standard;
  bool m_IsBlockingAtk = false;
  StatsInGame m_StatsInGame;
  int m_ActionsDoneInRound = 0;
  int m_MaxNbActionsInRound = 1;

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
  QString ProcessAggro(const int atkValue, const int aggroValue,
                       const int nbTurn);
  void UpdateStatsToNextLevel();
  void UpdateBuf(const BufTypes &bufType, const int value, const bool isPercent,
                 const QString &stat);
  static int UpdateDamageByBuf(const Buffers *bufDmg, const int value);
};

#endif // CHARACTER_H
