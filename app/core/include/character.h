#ifndef CHARACTER_H
#define CHARACTER_H

#include <QString>
#include <QColor>

#include <unordered_map>
#include <vector>
#include <tuple>

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

class Character {
public:
  Character(const QString name, const characType type, const Stats &stats);

  QString Attaque(const QString &atkName, Character *target);
  void UpdateStatsOnAtk(const QString &atkName);
  void AddAtq(const AttaqueType &atq);
  void AddStuff(const Stuff &stuff);
  void LoadAtkJson();
  void LoadStuffJson();
  void ApplyEquipOnStats(const std::unordered_map<QString, Stuff> &allEquipMap);
  bool CanBeLaunched(const AttaqueType &atk) const;

  // Effect
  QString ApplyOneEffect(Character *target, effectParam &effect,
                         const bool fromLaunch);
  std::tuple<bool, QStringList, std::vector<effectParam>>
  ApplyAtkEffect(const bool targetedOnMainAtk, const QString &atkName,
                 Character *target); // value1: apply the atk ?, value2 : logs
                                     // after applying effects
  void RemoveMalusEffect(const QString& statsName);
  int DamageByAtk(Character *target, const AttaqueType &atk);
  QString RegenIntoDamage(const int atkValue, const QString& statsName);
  std::vector<effectParam> CreateEveilDeLaForet();

  static QString GetInventoryString(const InventoryType &type);

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

  private:
  template <class T>
  void ProcessAddEquip(StatsType<T> &charStat, const StatsType<T> &equipStat) const;
  template <class T>
  void ProcessRemoveEquip(StatsType<T> &charStat,
                          const StatsType<T> &equipStat);
  int ProcessCurrentValueOnEffect(const effectParam& ep, const int launcherPowMag, const int nbOfApplies, const bool percent);
  QString ProcessOutputLogOnEffect(const effectParam& ep, const int amount, const bool fromLaunch, const int nbOfApplies);
  int ProcessDecreaseOnTurn(const effectParam& ep);
  QString ProcessDecreaseByTurn(const effectParam& ep);
};

#endif // CHARACTER_H
