#ifndef CHARACTER_H
#define CHARACTER_H

#include <QColor>
#include <QString>

#include <tuple>
#include <unordered_map>
#include <vector>

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
};

enum class InventoryType { healthPotion, manaPotion, enumSize };

struct Buf {
  int m_Value = 0;
  bool m_IsPercent = false;
  void SetBuf(const int value, const bool isPercent) {
    m_Value = value;
    m_IsPercent = isPercent;
  }
};

class Character {
public:
  Character() = default;
  Character(const QString name, const characType type, const Stats &stats);

  void ProcessCost(const QString &atkName);
  void AddAtq(const AttaqueType &atq);
  void AddStuff(const Stuff &stuff);
  void LoadAtkJson();
  void LoadStuffJson();
  void ApplyEquipOnStats(const std::unordered_map<QString, Stuff> &allEquipMap);
  bool CanBeLaunched(const AttaqueType &atk) const;

  // Effect
  QString ApplyOneEffect(Character *target, effectParam &effect,
                         const bool fromLaunch, const AttaqueType &atk);
  std::tuple<bool, QStringList, std::vector<effectParam>>
  ApplyAtkEffect(const bool targetedOnMainAtk, const AttaqueType &atk,
                 Character *target); // value1: conditions fulfilled ?, value2 :
                                     // logs after applying effects
  void RemoveMalusEffect(const effectParam &ep);

  QString RegenIntoDamage(const int atkValue, const QString &statsName) const;
  std::vector<effectParam> CreateEveilDeLaForet(); // template
  void SetBuf(const int value, const bool isPercent);

  static void SetStatsByPercent(
      StatsType<int> &stat, const int value,
      const bool isUp); // TODO à sortir dans un common pour gerer les stats?
  static QString GetInventoryString(const InventoryType &type);
  bool IsDodging() const;
  void UsePotion(const QString& statsName);

  QString m_Name = "default";
  characType m_type = characType::Hero;
  Stats m_Stats;
  std::unordered_map<QString, QString>
      m_WearingEquipment; // key: body, value: equipmentName
  std::unordered_map<QString, AttaqueType>
      m_AttakList; // key: attak name, value: AttakType struct
  std::vector<uint8_t> m_Inventory;
  int m_Level = 1;
  int m_Exp = 0;
  QColor color = QColor("dark");
  // Buf
  Buf m_BufDamage;

private:
  template <class T>
  void ProcessAddEquip(StatsType<T> &charStat,
                       const StatsType<T> &equipStat) const;
  template <class T>
  void ProcessRemoveEquip(StatsType<T> &charStat,
                          const StatsType<T> &equipStat);
  int ProcessCurrentValueOnEffect(const effectParam &ep, const int nbOfApplies,
                                  const Stats &launcherStats,
                                  Stats &targetStats) const;
  QString ProcessOutputLogOnEffect(const effectParam &ep, const int amount,
                                   const bool fromLaunch, const int nbOfApplies,
                                   const QString &atkName) const;
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
      const AttaqueType &atk) const; // pair1 output log, pair2 nbOfApplies
  QString ProcessAggro(const int atkValue, const QString &statsName);
  int ProcessCriticalStrike(const int atkValue) const;
};

#endif // CHARACTER_H
