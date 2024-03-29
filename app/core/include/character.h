#ifndef CHARACTER_H
#define CHARACTER_H

#include <QColor>
#include <QString>

#include <tuple>
#include <unordered_map>
#include <vector>
#include <deque>

#include "common.h"
#include "effect.h"
#include "stuff.h"


enum class characType { Hero, Boss };

class AttaqueType {
public:
  QString name = "Atq";
  uint8_t level = 1;
  uint32_t damage = 0;
  uint32_t heal = 0;
  uint32_t regenMana = 0;
  uint32_t regenVigor = 0;
  uint32_t regenBerseck = 0;
  uint32_t aggro = 0;
  uint32_t aggroRate = 0;
  uint32_t berseckRate = 0;
  QString effect;
  uint32_t manaCost = 0;
  uint32_t vigorCost = 0;
  uint32_t berseckCost = 0;
  QString target = TARGET_ENNEMY;
  QString reach = REACH_INDIVIDUAL;
  uint16_t turnsDuration = 1;
  QString namePhoto = "default.png";
  static std::vector<QString> TARGET_TYPES;
  static std::vector<QString> REACH_TYPES;
  std::vector<effectParam> m_AllEffects = {};
  QString form = STANDARD_FORM;
};

enum class InventoryType { healthPotion, manaPotion, enumSize };
enum class BufTypes { defaultBuf, damageRx, damageTx, damageCritCapped, powPhyBuf, enumSize };

class Character {
public:
  Character();
  Character(const QString name, const characType type, const Stats &stats);

  void InitTables();
  void ProcessCost(const QString &atkName);
  void AddAtq(const AttaqueType &atq);
  void LoadAtkJson();
  void LoadStuffJson();
  void ApplyEquipOnStats();
  bool CanBeLaunched(const AttaqueType &atk) const;

  // Effect
  QString ApplyOneEffect(Character *target, effectParam &effect,
                         const bool fromLaunch, const AttaqueType &atk,
                         const bool reload = false, const bool isCrit = false);
  std::tuple<bool, QStringList, std::vector<effectParam>>
  ApplyAtkEffect(const bool targetedOnMainAtk, const AttaqueType &atk,
                 Character *target,
                 const bool isCrit); // value1: conditions fulfilled ?, value2 :
                                     // logs after applying effects
  void RemoveMalusEffect(const effectParam &ep);

  QString RegenIntoDamage(const int atkValue, const QString &statsName) const;
  std::vector<effectParam> CreateEveilDeLaForet(); // template

  static void
  SetStatsOnEffect(StatsType<int> &stat, const int value, const bool isUp,
                   const bool isPercent,
                   const bool updateEffect); // TODO à sortir dans un common
                                             // pour gerer les stats?
  static QString GetInventoryString(const InventoryType &type);
  std::pair<bool, QString> IsDodging() const;
  void UsePotion(const QString &statsName);
  void AddExp(const int newXp);
  void SetEquipment(const std::unordered_map<QString, QString> &);
  void UpdateEquipmentOnJson() const;
  void ApplyEffeftOnStats(const bool updateEffect);
  std::pair<bool, int> ProcessCriticalStrike(); // return isCrit, random number
  void ResetBuf(const BufTypes &bufType);
  void SetValuesForThalia(const bool isBear);

  // Temporary
  std::vector<effectParam> LoadThaliaTalent() const;
  std::vector<effectParam> LoadAzrakTalent() const;
  std::vector<effectParam> LoadThrainTalent() const;

  QString m_Name = "default";
  characType m_type = characType::Hero;
  Stats m_Stats;
  std::unordered_map<QString, Stuff>
      m_WearingEquipment; // key: body, value: equipmentName
  std::unordered_map<QString, AttaqueType>
      m_AttakList; // key: attak name, value: AttakType struct
  int m_Level = 1;
  int m_Exp = 0;
  int m_NextLevel = 100;
  std::deque<int> m_LastAggros; // keep the last five aggros and sum them
  std::vector<QString> m_Forms;
  QString m_SelectedForm = STANDARD_FORM;

  QColor color = QColor("dark");
  // Buf
  std::vector<Buf> m_AllBufs;
  int m_HealRxOnTurn = 0;

private:
  template <class T>
  void ProcessAddEquip(StatsType<T> &charStat,
                       const StatsType<T> &equipStat) const;
  template <class T>
  void ProcessRemoveEquip(StatsType<T> &charStat,
                          const StatsType<T> &equipStat);
  std::pair<int, int> ProcessCurrentValueOnEffect(
      effectParam &ep, const int nbOfApplies, const Stats &launcherStats,
      const bool launch, Character *target,
      const bool isCrit) const; // value 1 isCrit, value 2 total amount value 3
                                // maxamount, value 4: crit randNb
  QString ProcessOutputLogOnEffect(const effectParam &ep, const int amount,
                                   const bool fromLaunch, const int nbOfApplies,
                                   const QString &atkName,
                                   const int maxAmount) const;
  int ProcessDecreaseOnTurn(const effectParam &ep) const;
  QString ProcessDecreaseByTurn(const effectParam &ep) const;
  static int DamageByAtk(const Stats &launcherStats, const Stats &targetStats,
                         const bool isMagicAtk, const int atkValue,
                         const int nbTurns);
  int GetSignEffectValue(const QString &target) const;
  QChar GetCharEffectValue(const QString &target) const;
  int GetMaxNbOfApplies(const AttaqueType &atk) const;
  int ProcessBerseckOnRxAtk(const int nbOfApplies);
  std::pair<QString, int> ProcessEffectType(
      effectParam &effect, Character *target,
      const AttaqueType &atk); // pair1 output log, pair2 nbOfApplies
  QString ProcessAggro(const int atkValue);
  void UpdateStatsToNextLevel();
  void UpdateBuf(const BufTypes &bufType, const int value,
                 const bool isPercent);
  static int UpdateDamageByBuf(const Buf &bufDmg, const int value);
};

#endif // CHARACTER_H
