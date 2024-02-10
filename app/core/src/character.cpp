#include "character.h"

// decode json
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QtDebug>

#include "Application.h"
#include "utils.h"

using namespace std;

// STATIC ATTAK TYPE
// init m_TargetTypes
std::vector<QString> AttaqueType::TARGET_TYPES = {TARGET_ENNEMY, TARGET_ALLY};
// init m_ReachTypes
std::vector<QString> AttaqueType::REACH_TYPES = {REACH_ZONE, REACH_INDIVIDUAL};

Character::Character(const QString name, const characType type,
                     const Stats &stats)
    : m_Name(name), m_type(type), m_Stats(stats) {
  m_Inventory.resize(static_cast<int>(InventoryType::enumSize));
}

//////
/// \brief Character::Attaque
/// HOT by n turns : divided pow by n
/// \param atkName
/// \param target
/// Returns a string to append in channel
///
QString Character::Attaque(const QString &atkName, Character *target) {
  if (target == nullptr || atkName.isEmpty()) {
    return "No target on atk or no atk name";
  }
  QString channelLog;
  const auto &atk = m_AttakList.at(atkName);

  // Stats change on target
  auto &targetHp =
      std::get<StatsType<int>>(target->m_Stats.m_AllStatsTable[STATS_HP]);
  auto &launcherPowMag =
      std::get<StatsType<double>>(m_Stats.m_AllStatsTable[STATS_POW_MAG]);
  auto &launcherPowPhy =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_POW_PHY]);
  auto &targetArmPhy =
      std::get<StatsType<int>>(target->m_Stats.m_AllStatsTable[STATS_ARM_PHY]);
  auto &targetArmMag =
      std::get<StatsType<int>>(target->m_Stats.m_AllStatsTable[STATS_ARM_MAG]);

  auto &tarCurHp = targetHp.m_CurrentValue;
  if (atk.damage > 0) {
    int arm = 0;
    int damage = atk.damage;
    if (atk.manaCost > 0) {
      damage += static_cast<int>(std::round(launcherPowMag.m_CurrentValue));
      arm = targetArmMag.m_CurrentValue;
    } else if (atk.vigorCost > 0 || atk.berseckCost > 0) {
      damage += launcherPowPhy.m_CurrentValue;
      arm = targetArmPhy.m_CurrentValue;
    }
    const double protection = 1000.0 / (1000.0 + static_cast<double>(arm));
    const auto finalDamage = static_cast<int>(std::round(damage * protection));

    tarCurHp = max(0, tarCurHp - finalDamage);
    channelLog = PlayersManager::FormatAtkOnEnnemy(m_Name, target->m_Name,
                                                   atkName, finalDamage);
  }
  if (atk.heal > 0) {
    tarCurHp = min(targetHp.m_MaxValue, static_cast<int>(tarCurHp + atk.heal));
    channelLog = PlayersManager::FormatAtkOnAlly(m_Name, target->m_Name,
                                                 atkName, atk.heal);
  }
  if (channelLog.isEmpty()) {
    channelLog = PlayersManager::FormatAtk(m_Name, target->m_Name, atkName);
  }
  return channelLog;
}

void Character::UpdateStatsOnAtk(const QString &atkName) {
  if (atkName.isEmpty()) {
    return;
  }
  const auto &atk = m_AttakList.at(atkName);

  // Stats change on target
  auto &launcherMana =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_MANA]);
  auto &launcherAggro =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_AGGRO]);
  auto &launcherBerseck =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_BERSECK]);
  auto &launcherVigor =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_VIGOR]);

  auto &launcherAggroRate =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_RATE_AGGRO]);
  auto &launcherRegenMana =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_REGEN_MANA]);
  auto &launcherRegenVigor =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_REGEN_VIGOR]);
  auto &launcherBerseckRate =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_RATE_BERSECK]);

  // Cost
  launcherMana.m_CurrentValue =
      max(0, static_cast<int>(launcherMana.m_CurrentValue - atk.manaCost));
  launcherVigor.m_CurrentValue =
      max(0, static_cast<int>(launcherVigor.m_CurrentValue - atk.vigorCost));
  launcherBerseck.m_CurrentValue = max(
      0, static_cast<int>(launcherBerseck.m_CurrentValue - atk.berseckCost));
  // Gain or regen
  launcherAggro.m_CurrentValue += launcherAggroRate.m_CurrentValue;
  launcherBerseck.m_CurrentValue += launcherBerseckRate.m_CurrentValue;
  launcherMana.m_CurrentValue +=
      static_cast<int>(std::round(launcherRegenMana.m_CurrentValue));
  launcherVigor.m_CurrentValue +=
      static_cast<int>(std::round(launcherRegenVigor.m_CurrentValue));
}

void Character::AddAtq(const AttaqueType &atq) { m_AttakList[atq.name] = atq; }

void Character::AddStuff(const Stuff &stuff) {

  std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_HP]).m_CurrentValue +=
      stuff.m_Stats.m_HP.m_CurrentValue;

  // m_Stats.m_Mana = stuff.m_Stats.m_Mana;
  // m_Stats.m_Vigor = stuff.m_Stats.m_Vigor;
  // m_Stats.m_Berseck = stuff.m_Stats.m_Berseck;
  // m_Stats.m_ArmPhy = stuff.m_Stats.m_ArmPhy;
  // m_Stats.m_ArmMag = stuff.m_Stats.m_ArmMag;
  // m_Stats.m_PowPhy = stuff.m_Stats.m_PowPhy;
  // m_Stats.m_PowMag = stuff.m_Stats.m_PowMag;
  // m_Stats.m_Aggro = stuff.m_Stats.m_Aggro;
  // m_Stats.m_Speed = stuff.m_Stats.m_Speed;
  // m_Stats.m_CriticalStrike = stuff.m_Stats.m_CriticalStrike;
  // m_Stats.m_Dogde = stuff.m_Stats.m_Dogde;
  // m_Stats.m_RegenHP = stuff.m_Stats.m_RegenHP;
  // m_Stats.m_RegenMana = stuff.m_Stats.m_RegenMana;
  // m_Stats.m_RegenVigor = stuff.m_Stats.m_RegenVigor;
}

QString Character::GetInventoryString(const InventoryType &type) {
  switch (type) {
  case InventoryType::healthPotion:
    return "Potion de vie";
  case InventoryType::manaPotion:
    return "Potion de mana";
  default:
    break;
  }
  return "";
}

void Character::LoadAtkJson() {
  // List all attak for a character
  QString directoryPath = OFFLINE_ATK + m_Name; // Replace with the actual path
  QDir directory(directoryPath);
  if (!directory.exists()) {
    qDebug() << "Directory does not exist: " << directoryPath;
  }
  QStringList fileList =
      directory.entryList(QDir::Files | QDir::NoDotAndDotDot);
  qDebug() << "List of files in the directory:";
  for (const QString &file : fileList) {
    qDebug() << file;
    QFile atkJson(directoryPath + "/" + file);
    if (!atkJson.open(QFile::ReadOnly | QFile::Text)) {
      Application::GetInstance().log(" Could not open the file for reading " +
                                     directoryPath + "/" + file);
      return;
    } else {
      // Convert json file to QString
      QTextStream out(&atkJson);
#if QT_VERSION_MAJOR == 6
      out.setEncoding(QStringConverter::Encoding::Utf8);
#else
      out.setCodec("UTF-8");
#endif

      QString msg = out.readAll();
      atkJson.close();

      auto json = QJsonDocument::fromJson(msg.toUtf8());
      // decode json
      AttaqueType atk;
      atk.name = json[ATK_NAME].toString();
      atk.namePhoto = json[ATK_PHOTO].toString();
      atk.aggro = json[ATK_AGGRO].toInt();
      atk.damage = static_cast<uint32_t>(json[ATK_DAMAGE].toInt());
      atk.heal = json[ATK_HEAL].toInt();
      atk.regenMana = json[ATK_REGEN_MANA].toInt();
      atk.effect = json[ATK_EFFECT].toString();
      atk.level = static_cast<uint8_t>(json[ATK_LEVEL].toInt());
      atk.manaCost = static_cast<uint32_t>(json[ATK_MANA_COST].toInt());
      atk.vigorCost = static_cast<uint32_t>(json[ATK_VIGOR_COST].toInt());
      atk.regenBerseck = static_cast<uint32_t>(json[ATK_REGEN_BERSECK].toInt());
      atk.regenVigor = static_cast<uint32_t>(json[ATK_REGEN_VIGOR].toInt());
      atk.berseckCost = static_cast<uint32_t>(json[ATK_BERSECK_COST].toInt());
      atk.reach = json[ATK_REACH].toString();
      atk.target = json[ATK_TARGET].toString();
      atk.turnsDuration = static_cast<uint16_t>(json[ATK_DURATION].toInt());
      QJsonArray effectArray = json[EFFECT_ARRAY].toArray();
#if QT_VERSION_MAJOR == 6
      for (const auto &effect : effectArray) {
        const auto &stat = effect[EFFECT_STAT].toString();
        if (stat.isEmpty()) {
          break;
        }
        effectParam param;
        param.effect = effect[EFFECT_TYPE].toString();
        param.value = effect[EFFECT_VALUE].toInt();
        param.nbTurns = effect[EFFECT_ACTIVE_TURNS].toInt();
        param.reach = effect[EFFECT_REACH].toString();
        param.statsName = effect[EFFECT_STAT].toString();
        param.target = effect[EFFECT_TARGET].toString();
        param.subValueEffect = effect[EFFECT_SUB_VALUE].toInt();
        atk.m_AllEffects.push_back(param);
      }
#else
      for (const auto &effect : effectArray) {
        effectParam param;
        if (effect.isObject()) {
          const QJsonObject item = effect.toObject();
          for (const auto &key : item.keys()) {
            const auto &val = item[key];
            if (val.isString()) {
              if (key == EFFECT_TYPE) {
                param.effect = val.toString();
              }
              if (key == EFFECT_REACH) {
                param.reach = val.toString();
              }
              if (key == EFFECT_STAT) {
                param.statsName = val.toString();
              }
              if (key == EFFECT_TARGET) {
                param.target = val.toString();
              }
            } else if (val.isDouble()) {
              if (key == EFFECT_VALUE) {
                param.value = static_cast<int>(val.toDouble());
              }
              if (key == EFFECT_ACTIVE_TURNS) {
                param.nbTurns = static_cast<int>(val.toDouble());
              }
              if (key == EFFECT_SUB_VALUE) {
                param.subValueEffect = static_cast<int>(val.toDouble());
              }
            }
          }
        }

        if (param.statsName.isEmpty() && param.effect.isEmpty()) {
          break;
        }
        atk.m_AllEffects.push_back(param);
      }
#endif

      // Add atk to hero atk list
      AddAtq(atk);
    }
  }
}

void Character::LoadStuffJson() {
  QString directoryPath =
      OFFLINE_WEARING_EQUIPMENT; // Replace with the actual path
  if (QDir directory(directoryPath); !directory.exists()) {
    qDebug() << "Directory does not exist: " << directoryPath;
  }
  QFile json(directoryPath + "/" + m_Name + ".json");
  if (!json.open(QFile::ReadOnly | QFile::Text)) {
    Application::GetInstance().log(" Could not open the file for reading " +
                                   directoryPath + m_Name + ".json");
    return;
  } else {
    // Convert json file to QString
    QTextStream out(&json);
    QString msg = out.readAll();
    json.close();

    const auto jsonDoc = QJsonDocument::fromJson(msg.toUtf8());
    // decode json
    m_WearingEquipment[EQUIP_HEAD] = jsonDoc[EQUIP_HEAD].toString();
    m_WearingEquipment[EQUIP_NECKLACE] = jsonDoc[EQUIP_NECKLACE].toString();
    m_WearingEquipment[EQUIP_CHEST] = jsonDoc[EQUIP_CHEST].toString();
    m_WearingEquipment[EQUIP_PANTS] = jsonDoc[EQUIP_PANTS].toString();
    m_WearingEquipment[EQUIP_SHOES] = jsonDoc[EQUIP_SHOES].toString();
    m_WearingEquipment[EQUIP_LEFT_ARM] = jsonDoc[EQUIP_LEFT_ARM].toString();
    m_WearingEquipment[EQUIP_RIGHT_ARM] = jsonDoc[EQUIP_RIGHT_ARM].toString();
    m_WearingEquipment[EQUIP_LEFT_LEG] = jsonDoc[EQUIP_LEFT_LEG].toString();
    m_WearingEquipment[EQUIP_RIGHT_LEG] = jsonDoc[EQUIP_RIGHT_LEG].toString();
    m_WearingEquipment[EQUIP_RING] = jsonDoc[EQUIP_RING].toString();
    m_WearingEquipment[EQUIP_RIGHT_WEAPON] =
        jsonDoc[EQUIP_RIGHT_WEAPON].toString();
    m_WearingEquipment[EQUIP_LEFT_WEAPON] =
        jsonDoc[EQUIP_LEFT_WEAPON].toString();
  }
}

void Character::ApplyEquipOnStats(
    const std::unordered_map<QString, Stuff> &allEquipMap) {

  for (const auto &[body, equipName] : m_WearingEquipment) {
    if (equipName.isEmpty()) {
      continue;
    }
    const auto &equip = allEquipMap.at(equipName);
    ProcessAddEquip(std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_HP]),
                    equip.m_Stats.m_HP);
    ProcessAddEquip(
        std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_MANA]),
        equip.m_Stats.m_Mana);
    ProcessAddEquip(
        std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_VIGOR]),
        equip.m_Stats.m_Vigor);
    ProcessAddEquip(
        std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_BERSECK]),
        equip.m_Stats.m_Berseck);
    ProcessAddEquip(
        std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_ARM_PHY]),
        equip.m_Stats.m_ArmPhy);
    ProcessAddEquip(
        std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_ARM_MAG]),
        equip.m_Stats.m_ArmMag);
    ProcessAddEquip(
        std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_POW_PHY]),
        equip.m_Stats.m_PowPhy);
    ProcessAddEquip(
        std::get<StatsType<double>>(m_Stats.m_AllStatsTable[STATS_POW_MAG]),
        equip.m_Stats.m_PowMag);
    ProcessAddEquip(
        std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_AGGRO]),
        equip.m_Stats.m_Aggro);
    ProcessAddEquip(
        std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_SPEED]),
        equip.m_Stats.m_Speed);
    ProcessAddEquip(
        std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_CRIT]),
        equip.m_Stats.m_CriticalStrike);
    ProcessAddEquip(
        std::get<StatsType<double>>(m_Stats.m_AllStatsTable[STATS_DODGE]),
        equip.m_Stats.m_Dogde);
    ProcessAddEquip(
        std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_REGEN_HP]),
        equip.m_Stats.m_RegenHP);
    ProcessAddEquip(
        std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_REGEN_MANA]),
        equip.m_Stats.m_RegenMana);
    ProcessAddEquip(
        std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_REGEN_VIGOR]),
        equip.m_Stats.m_RegenVigor);
  }
}

template <class T>
void Character::ProcessAddEquip(StatsType<T> &charStat,
                                const StatsType<T> &equipStat) {
  if (equipStat.m_CurrentValue == 0) {
    return;
  }
  double ratio = static_cast<double>(charStat.m_CurrentValue) /
                 static_cast<double>(charStat.m_MaxValue);
  charStat.m_MaxValue +=
      equipStat.m_CurrentValue; // Currently only current value is filled on
                                // equip stat

  charStat.m_CurrentValue =
      static_cast<T>(std::round(charStat.m_MaxValue * ratio));
}

template <class T>
void Character::ProcessRemoveEquip(StatsType<T> &charStat,
                                   const StatsType<T> &equipStat) {
  if (equipStat.m_CurrentValue == 0) {
    return;
  }
  double ratio = static_cast<double>(charStat.m_CurrentValue) /
                 static_cast<double>(charStat.m_MaxValue);
  charStat.m_MaxValue -=
      equipStat.m_CurrentValue; // Currently only current value is filled on
                                // equip stat

  charStat.m_CurrentValue =
      static_cast<T>(std::round(charStat.m_MaxValue * ratio));
}

/////////////////////////////////////////
/// \brief Character::CanBeLaunched
/// The attak can be launched if the character has enough mana, vigor and
/// berseck.
///
bool Character::CanBeLaunched(const AttaqueType &atk) const {
  const auto remainingMana = static_cast<uint32_t>(
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable.at(STATS_MANA))
          .m_CurrentValue);
  const auto remainingBerseck = static_cast<uint32_t>(
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable.at(STATS_BERSECK))
          .m_CurrentValue);
  const auto remainingVigor = static_cast<uint32_t>(
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable.at(STATS_VIGOR))
          .m_CurrentValue);
  if (atk.manaCost > 0 && atk.manaCost <= remainingMana) {
    return true;
  }
  if (atk.berseckCost > 0 && atk.berseckCost <= remainingBerseck) {
    return true;
  }
  if (atk.vigorCost > 0 && atk.vigorCost <= remainingVigor) {
    return true;
  }
  return false;
}

QString Character::ApplyOneEffect(Character *target, const effectParam &effect,
                                  const bool fromLaunch) {
  if (effect.effect == EFFECT_NB_DECREASE_BY_TURN) {
    const int intMin = 0;
    const int intMax = 100;
    const int stepLimit = (intMax / effect.nbTurns); // get percentual
    const auto maxLimit = stepLimit * (effect.nbTurns - effect.counterTurn);
    const auto randNb = Utils::GetRandomNb(intMin, intMax);
    if (!(randNb >= 0 && randNb < maxLimit)) {
      return QString("%1 n'a pas d'effet").arg(EFFECT_NB_DECREASE_BY_TURN);
    }
  }
  int nbOfApplies = 1; // default value 1 for the nominal case

  if (fromLaunch) {
    if (effect.effect == EFFECT_NB_DECREASE_ON_TURN) {
      // Set nbOfApplies to 0 as it is considered as a counter here
      nbOfApplies = 0;
      int counter = effect.subValueEffect;
      while (counter > 0) {
        const int intMin = 0;
        const int intMax = 100;
        const int stepLimit =
            (intMax / effect.subValueEffect); // get percentual
        const auto maxLimit = stepLimit * counter;
        const auto randNb = Utils::GetRandomNb(intMin, intMax);
        if (randNb >= 0 && randNb < maxLimit) {
          nbOfApplies++;
        } else {
          break;
        }
        counter--;
      }
    }
    if (effect.effect == EFFECT_REINIT) {
      target->ResetCounterOnOneStatsEffect(effect.statsName);
    }
    if (effect.effect == EFFECT_DELETE_BAD) {
      target->DeleteOneBadEffect();
    }
  }
  for (int i = 0; i < nbOfApplies; i++) {
    if (effect.statsName == STATS_SPEED) { // value in %
      auto &localStat = std::get<StatsType<int>>(
          target->m_Stats.m_AllStatsTable[effect.statsName]);
      localStat.m_CurrentValue = static_cast<int>(
          std::round(localStat.m_CurrentValue +
                     localStat.m_CurrentValue * effect.value / 100));
    } else if (effect.statsName == STATS_POW_MAG ||
               effect.statsName == STATS_DODGE) {
      auto &localStat = std::get<StatsType<double>>(
          target->m_Stats.m_AllStatsTable[effect.statsName]);
      localStat.m_CurrentValue =
          min(localStat.m_MaxValue, localStat.m_CurrentValue + effect.value);
    } else {
      auto &localStat = std::get<StatsType<int>>(
          target->m_Stats.m_AllStatsTable[effect.statsName]);
      localStat.m_CurrentValue =
          min(localStat.m_MaxValue, localStat.m_CurrentValue + effect.value);
    }
  }
  const int potentialAttempts = max(1, effect.subValueEffect);
  return QString("Sur %1. L'effet %2-%3 s'est appliqué %4 fois sur %5 "
                 "potentielle(s) "
                 "tentative(s) avec une valeur max de %6.")
      .arg(target->m_Name)
      .arg(effect.statsName)
      .arg(effect.effect)
      .arg(nbOfApplies)
      .arg(QString::number(potentialAttempts))
      .arg(QString::number(nbOfApplies * effect.value));
}

// Apply effect after launch of atk
std::pair<bool, QStringList>
Character::ApplyAtkEffect(const bool targetedOnMainAtk, const QString &atkName,
                          Character *target) {
  if (target == nullptr) {
    return std::make_pair(false, QStringList("No target"));
  }
  bool applyAtk = true;
  const auto &allEffects = m_AttakList.at(atkName).m_AllEffects;

  QStringList resultEffects;
  for (const auto &effect : allEffects) {

    // is targeted ?
    if (effect.target == TARGET_ALLY && effect.reach == REACH_INDIVIDUAL &&
        !targetedOnMainAtk) {
      continue;
    }
    if (effect.target == TARGET_ALLY && effect.reach == REACH_ZONE &&
        !targetedOnMainAtk) {
      continue;
    }
    if (effect.target == TARGET_ENNEMY && effect.reach == REACH_INDIVIDUAL &&
        !targetedOnMainAtk) {
      continue;
    }
    if (effect.effect == EFFECT_REINIT &&
        !(target->GetNbOfStatsInEffectList(effect.statsName) >=
          effect.subValueEffect)) {
      applyAtk = false;
      resultEffects.append(
          QString("Sur %1. L'effet %2-%3 n'est pas applicable. %4 effet(s) sur "
                  "stats %5 requis.")
              .arg(target->m_Name)
              .arg(effect.statsName)
              .arg(effect.effect)
              .arg(effect.subValueEffect)
              .arg(effect.statsName));
      break;
    }
    resultEffects.append(ApplyOneEffect(target, effect, true));
  }

  return std::make_pair(applyAtk, resultEffects);
}

int Character::GetNbOfStatsInEffectList(const QString &statsName) {
  int counter = 0;
  for (const auto &e : m_EffectsList) {
    if (e->statsName == statsName) {
      counter++;
    }
  }
  return counter;
}

void Character::ResetCounterOnOneStatsEffect(const QString &statsName) {
  for (auto &e : m_EffectsList) {
    if (e->statsName == statsName) {
      e->counterTurn = 0;
    }
  }
}

void Character::DeleteOneBadEffect() {
  for (auto &e : m_EffectsList) {
    if (e->value < 0) {
      e->counterTurn = e->nbTurns;
    }
  }
}

void Character::RemoveMalusEffect(const QString &statsName) {

  for (const auto &stats : ALL_STATS) {
    if (stats == STATS_POW_MAG || stats == STATS_DODGE) {
      auto &localStat =
          std::get<StatsType<double>>(m_Stats.m_AllStatsTable[stats]);
      localStat.m_CurrentValue = localStat.m_MaxValue;
      break;
    } else {
      auto &localStat =
          std::get<StatsType<int>>(m_Stats.m_AllStatsTable[stats]);
      localStat.m_CurrentValue = localStat.m_MaxValue;
      break;
    }
  }
}
