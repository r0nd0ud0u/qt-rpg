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

Character::Character(const QString name, const characType type,
                     const Stats &stats)
    : m_Name(name), m_type(type), m_Stats(stats) {
  m_Inventory.resize(static_cast<int>(InventoryType::enumSize));
  // init equip
  for (const auto &e : ALL_EQUIP) {
    m_WearingEquipment[e].m_Name = "";
  }
}

int Character::DamageByAtk(const Stats &launcherStats, const Stats &targetStats,
                           const bool isMagicAtk, const int atkValue,
                           const int nbTurns) {
  const auto &launcherPowMag =
      std::get<StatsType<int>>(launcherStats.m_AllStatsTable.at(STATS_POW_MAG));
  const auto &launcherPowPhy =
      std::get<StatsType<int>>(launcherStats.m_AllStatsTable.at(STATS_POW_PHY));
  const auto &targetArmPhy =
      std::get<StatsType<int>>(targetStats.m_AllStatsTable.at(STATS_ARM_PHY));
  const auto &targetArmMag =
      std::get<StatsType<int>>(targetStats.m_AllStatsTable.at(STATS_ARM_MAG));

  int finalDamage = 0;
  int arm = 0;
  int damage = atkValue;
  if (isMagicAtk) {
    damage += launcherPowMag.m_CurrentValue / nbTurns;
    arm = targetArmMag.m_CurrentValue;
  } else {
    damage += launcherPowPhy.m_CurrentValue / nbTurns;
    arm = targetArmPhy.m_CurrentValue;
  }
  const double protection = 1000.0 / (1000.0 + static_cast<double>(arm));
  finalDamage = static_cast<int>(std::round(damage * protection));
  return finalDamage;
}

// TODO maybe return a log for the channel, and one for the debug
QString Character::RegenIntoDamage(const int atkValue,
                                   const QString &statsName) const {
  if (atkValue <= 0) {
    return "";
  }

  if (statsName != STATS_HP && statsName != STATS_MANA &&
      statsName != STATS_VIGOR && statsName != STATS_BERSECK) {
    return "";
  }

  const auto pm = Application::GetInstance().m_GameManager->m_PlayersManager;
  if (pm->m_AllEffectsOnGame.find(m_Name) == pm->m_AllEffectsOnGame.end()) {
    return "";
  }

  QString channelLog;
  std::vector<Character *> playerList;
  if (m_type == characType::Hero) {
    playerList = pm->m_HeroesList;
  } else if (m_type == characType::Boss) {
    playerList = pm->m_BossesList;
  }

  for (const auto &e : pm->m_AllEffectsOnGame.at(m_Name)) {
    if (e.allAtkEffects.effect == EFFECT_INTO_DAMAGE &&
        statsName == e.allAtkEffects.statsName) {
      for (auto *pl : playerList) {
        const auto finalDamage =
            DamageByAtk(m_Stats, pl->m_Stats, e.allAtkEffects.isMagicAtk,
                        atkValue * e.allAtkEffects.subValueEffect / 100,
                        e.allAtkEffects.nbTurns);
        auto &localstat =
            std::get<StatsType<int>>(pl->m_Stats.m_AllStatsTable[statsName]);
        localstat.m_CurrentValue =
            max(0, localstat.m_CurrentValue - finalDamage);
        channelLog = PlayersManager::FormatAtkOnEnnemy(finalDamage);
      }
    }
  }
  return channelLog;
}

void Character::ProcessCost(const QString &atkName) {
  if (atkName.isEmpty()) {
    return;
  }
  if (m_AttakList.count(atkName) == 0) {
    return;
  }
  const auto &atk = m_AttakList.at(atkName);

  // Stats change on target
  auto &launcherMana =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_MANA]);
  auto &launcherBerseck =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_BERSECK]);
  auto &launcherVigor =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_VIGOR]);

  // Cost
  // mana cost in %
  launcherMana.m_CurrentValue =
      max(0, static_cast<int>(launcherMana.m_CurrentValue -
                              atk.manaCost * launcherMana.m_MaxValue / 100));
  launcherVigor.m_CurrentValue =
      max(0, static_cast<int>(launcherVigor.m_CurrentValue -
                              atk.vigorCost * launcherVigor.m_MaxValue / 100));
  launcherBerseck.m_CurrentValue = max(
      0, static_cast<int>(launcherBerseck.m_CurrentValue - atk.berseckCost));
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
        if (stat.isEmpty() && effect[EFFECT_TYPE].toString().isEmpty()) {
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
        // processed
        param.isMagicAtk = atk.manaCost > 0;

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
        // processed
        param.isMagicAtk = atk.manaCost > 0;
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
#if QT_VERSION_MAJOR == 6
    out.setEncoding(QStringConverter::Encoding::Utf8);
#else
    out.setCodec("UTF-8");
#endif
    QString msg = out.readAll();
    json.close();

    const auto jsonDoc = QJsonDocument::fromJson(msg.toUtf8());
    // decode json
    for (const auto &e : ALL_EQUIP) {
      m_WearingEquipment[e].m_Name = jsonDoc[e].toString();
    }
  }
}

void Character::ApplyEquipOnStats() {

  for (const auto &[body, stuff] : m_WearingEquipment) {
    if (stuff.m_Name.isEmpty()) {
      continue;
    }
    for (const auto &stats : ALL_STATS) {
      if (m_Stats.m_AllStatsTable.count(stats) == 1) {
        ProcessAddEquip(
            std::get<StatsType<int>>(m_Stats.m_AllStatsTable[stats]),
            std::get<StatsType<int>>(stuff.m_Stats.m_AllStatsTable.at(stats)));
      }
    }
  }
  ApplyEffeftOnStats();
}

void Character::ApplyEffeftOnStats() {
  auto &allGae =
      Application::GetInstance()
          .m_GameManager->m_PlayersManager->m_AllEffectsOnGame[m_Name];

  for (auto &gae : allGae) {
    ApplyOneEffect(this, gae.allAtkEffects, false, gae.atk);
  }
}

template <class T>
void Character::ProcessAddEquip(StatsType<T> &charStat,
                                const StatsType<T> &equipStat) const {
  if (equipStat.m_BufEquipPercent == 0 && equipStat.m_BufEquipValue == 0) {
    return;
  }

  charStat.m_BufEquipValue += equipStat.m_BufEquipValue;
  charStat.m_BufEquipPercent += equipStat.m_BufEquipPercent;

  const double ratio = (charStat.m_MaxValue > 0)
                           ? static_cast<double>(charStat.m_CurrentValue) /
                                 static_cast<double>(charStat.m_MaxValue)
                           : 1;
  charStat.m_MaxValue =
      charStat.m_RawMaxValue + charStat.m_BufEquipValue +
      charStat.m_RawMaxValue * charStat.m_BufEquipPercent / 100;

  charStat.m_CurrentValue =
      static_cast<T>(std::round(charStat.m_MaxValue * ratio));
}

template <class T>
void Character::ProcessRemoveEquip(StatsType<T> &charStat,
                                   const StatsType<T> &equipStat) {
  if (equipStat.m_BufEquipPercent == 0 && equipStat.m_BufEquipValue == 0) {
    return;
  }
  charStat.m_BufEquipValue -= equipStat.m_BufEquipValue;
  charStat.m_BufEquipPercent -= equipStat.m_BufEquipPercent;

  const double ratio = (charStat.m_MaxValue > 0)
                           ? static_cast<double>(charStat.m_CurrentValue) /
                                 static_cast<double>(charStat.m_MaxValue)
                           : 1;
  charStat.m_MaxValue =
      charStat.m_RawMaxValue + charStat.m_BufEquipValue +
      charStat.m_RawMaxValue * charStat.m_BufEquipPercent / 100;

  charStat.m_CurrentValue =
      static_cast<T>(std::round(charStat.m_MaxValue * ratio));
}

/////////////////////////////////////////
/// \brief Character::CanBeLaunched
/// The attak can be launched if the character has enough mana, vigor and
/// berseck.
///
bool Character::CanBeLaunched(const AttaqueType &atk) const {
  if (atk.level > m_Level) {
    return false;
  }
  const auto &mana =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable.at(STATS_MANA));
  const auto &berseck =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable.at(STATS_BERSECK));
  const auto &vigor =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable.at(STATS_VIGOR));

  // check the impact of an effect on an atk here
  // Cooldown
  for (const auto &e : atk.m_AllEffects) {
    // TODO rewrite the cooldown thing for an atk
    if (e.effect == EFFECT_NB_COOL_DOWN) {
      const auto &pm =
          Application::GetInstance().m_GameManager->m_PlayersManager;
      for (const auto &gae : pm->m_AllEffectsOnGame[m_Name]) {
        if (atk.name == gae.atk.name && gae.allAtkEffects.effect == e.effect &&
            gae.allAtkEffects.nbTurns > 0) {
          return false;
        }
      }
    }
  }

  const int manaCost = atk.manaCost * mana.m_MaxValue / 100;
  const int vigorCost = atk.vigorCost * mana.m_MaxValue / 100;
  const int berseckCost = atk.berseckCost;

  if (manaCost <= mana.m_CurrentValue && vigorCost <= vigor.m_CurrentValue &&
      berseckCost <= berseck.m_CurrentValue) {
    return true;
  }

  return false;
}

int Character::GetSignEffectValue(const QString &target) const {
  int sign = 1;
  if (target == TARGET_ENNEMY) {
    sign = -1;
  }

  return sign;
}

QChar Character::GetCharEffectValue(const QString &target) const {
  QChar sign = '+';
  if (target == TARGET_ENNEMY) {
    sign = '-';
  }

  return sign;
}

QString Character::ApplyOneEffect(Character *target, effectParam &effect,
                                  const bool fromLaunch, const AttaqueType &atk,
                                  const bool reload) {
  if (target == nullptr) {
    return "No  target character";
  }
  QString result;

  // increment counter turn, effect is used
  if (fromLaunch) {
    effect.counterTurn++;
  }

  // conditions
  if (effect.effect == CONDITION_ENNEMIES_DIED) {
    const auto gs = Application::GetInstance().m_GameManager->m_GameState;
    effect.value *=
        static_cast<int>(gs->m_DiedEnnemies.count(gs->m_CurrentTurnNb - 1));
    effect.effect = EFFECT_IMPROVE_BY_PERCENT_CHANGE;
  }

  // Apply some effects only at launch
  if (!fromLaunch && ACTIVE_EFFECTS_ON_LAUNCH.count(effect.effect) > 0) {
    return "";
  }
  // up/down % stats must be effective only at launch
  if ((effect.statsName == STATS_DODGE || effect.statsName == STATS_CRIT) &&
      (!fromLaunch && !reload)) {
    return "";
  }

  const auto &[effectLog, nbOfApplies] = ProcessEffectType(effect, target, atk);
  result += effectLog;

  // apply amount on berseck character if target is ennemy
  if (const bool isOnEnnemy = effect.target == TARGET_ENNEMY;
      effect.statsName == STATS_HP && isOnEnnemy) {
    const auto berseckAmount = target->ProcessBerseckOnRxAtk(nbOfApplies);
    result += (berseckAmount > 0) ? QString("recupère +%1 de râge.") : "";
  }
  // apply the effect
  const auto amount = ProcessCurrentValueOnEffect(effect, nbOfApplies, m_Stats,
                                                  target->m_Stats);
  // TODO should be static and not on target ? pass target by argument
  result += target->ProcessOutputLogOnEffect(effect, amount, fromLaunch,
                                             nbOfApplies, atk.name);
  // Apply regen effect turning into damage for all bosses
  // can be processed only after calcul of amount of atk
  if (!reload) {
    result += RegenIntoDamage(amount, effect.statsName);
  }
  // Process aggro
  if (fromLaunch) {
    result += ProcessAggro(amount, effect.statsName);
  }

  return result;
}

// Apply effect after launch of atk
// increment counter turn of effect
// Test targets
// Return tuple
// - 1 : bool: conditions of atk are fulfilled ?
// - 2 : QStringList : log on each applied effect to display on channel
// - 3 : std::vector<effectParam> : list of all applied effectParam
std::tuple<bool, QStringList, std::vector<effectParam>>
Character::ApplyAtkEffect(const bool targetedOnMainAtk, const AttaqueType &atk,
                          Character *target) {
  if (target == nullptr) {
    return std::make_tuple(false, QStringList("No target"),
                           std::vector<effectParam>());
  }
  bool conditionsAreOk = true;
  // effect can be modified -> counter nb when applied
  const auto &allEffects = atk.m_AllEffects;
  std::vector<effectParam> allAppliedEffects;
  QStringList allResultEffects;
  const bool isAlly = target->m_type == m_type;

  for (const auto &effect : allEffects) {
    if (effect.target == TARGET_HIMSELF && m_Name != target->m_Name) {
      continue;
    }
    if (effect.target == TARGET_ONLY_ALLY && m_Name == target->m_Name) {
      continue;
    }
    if (!isAlly && ALLIES_TARGETS.count(effect.target) > 0) {
      continue;
    }
    if (isAlly && effect.target == TARGET_ENNEMY) {
      continue;
    }
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

    // Condition if applicable effect
    // TODO extract a method here with conditions
    const auto &pm = Application::GetInstance().m_GameManager->m_PlayersManager;
    // Condition nb of HOTS on player
    if (effect.statsName == STATS_HP && effect.effect == EFFECT_REINIT &&
        pm->GetNbOfStatsInEffectList(target, effect.statsName) <
            effect.subValueEffect) {
      conditionsAreOk = false;
      allResultEffects.append(
          QString("L'effet %1-%2 n'est pas applicable. %3 effet(s) sur "
                  "stats %4 requis.")
              .arg(effect.statsName)
              .arg(effect.effect)
              .arg(effect.subValueEffect)
              .arg(effect.statsName));
      break;
    }
    // Condition number of died ennemies
    if (effect.effect == CONDITION_ENNEMIES_DIED) {
      const auto gs = Application::GetInstance().m_GameManager->m_GameState;
      if (gs->m_CurrentTurnNb > 1 &&
          gs->m_DiedEnnemies.count(gs->m_CurrentTurnNb - 1) > 0) {
      } else {
        conditionsAreOk = false;
        allResultEffects.append(
            QString(
                "Pas d'effect %1 activé. Aucun ennemi mort au tour précédent")
                .arg(effect.effect));
        break;
      }
    }

    effectParam appliedEffect = effect;
    // appliedEffect is modified in ApplyOneEffect
    const QString resultEffect =
        ApplyOneEffect(target, appliedEffect, true, atk);
    // an one-occurence or more effect available is displayed
    if (!resultEffect.isEmpty()) {
      allResultEffects.append(resultEffect);
    }
    // from two-occurences an effect is stored, result effect must be not empty
    // it means the effect has an effect -> TODO can be replaced by a boolean
    // applied effect to process in ApplyOneEffect
    if (appliedEffect.nbTurns - appliedEffect.counterTurn > 0 &&
        !resultEffect.isEmpty()) {
      allAppliedEffects.push_back(appliedEffect);
    }
  }

  return std::make_tuple(conditionsAreOk, allResultEffects, allAppliedEffects);
}

void Character::RemoveMalusEffect(const effectParam &ep) {
  // remove malus applied on stats
  if (!ep.statsName.isEmpty() &&
      m_Stats.m_AllStatsTable.count(ep.statsName) > 0) {
    auto &localStat =
        std::get<StatsType<int>>(m_Stats.m_AllStatsTable.at(ep.statsName));
    if (ep.effect == EFFECT_PERCENT_CHANGE) {
      SetStatsOnEffect(localStat, ep.value, false, true);
    } else {
      SetStatsOnEffect(localStat, ep.value, false, false);
    }
  }

  // remove debuf/ buf
  // on damages
  if (ep.effect == EFFECT_CHANGE_MAX_DAMAGES_BY_PERCENT) {
    SetBuf(0, true);
  }
}

std::vector<effectParam> Character::CreateEveilDeLaForet() {
  std::vector<effectParam> epTable;

  effectParam param2;
  param2.effect = EFFECT_IMPROVE_HOTS;
  param2.value = 0;
  param2.nbTurns = 1;
  param2.reach = REACH_ZONE;
  param2.statsName = "";
  param2.target = TARGET_ALLY;
  param2.subValueEffect = 20;
  epTable.push_back(param2);

  effectParam param3;
  param3.effect = EFFECT_BOOSTED_BY_HOTS;
  param3.value = 80;
  param3.nbTurns = 1;
  param3.reach = REACH_ZONE;
  param3.statsName = STATS_HP;
  param3.target = TARGET_ALLY;
  param3.subValueEffect = 25;
  epTable.push_back(param3);

  return epTable;
}

int Character::ProcessCurrentValueOnEffect(const effectParam &ep,
                                           const int nbOfApplies,
                                           const Stats &launcherStats,
                                           Stats &targetStats) const {
  if (ep.statsName.isEmpty()) {
    return 0;
  }
  if (ep.value == 0) {
    return 0;
  }
  int output = 0;
  // heal or damage is suggested by sign
  const int sign = GetSignEffectValue(ep.target);
  const bool isPercentChange = ON_PERCENT_STATS.count(ep.statsName) > 0;

  // common init
  auto &localStat =
      std::get<StatsType<int>>(targetStats.m_AllStatsTable[ep.statsName]);
  const int delta = localStat.m_MaxValue - localStat.m_CurrentValue;
  int amount = 0;

  // HP
  if (ep.statsName == STATS_HP && ep.effect == EFFECT_VALUE_CHANGE) {
    if (const bool isOnEnnemy = ep.target == TARGET_ENNEMY; isOnEnnemy) {
      amount = nbOfApplies * DamageByAtk(launcherStats, targetStats,
                                         ep.isMagicAtk, ep.value, ep.nbTurns);
      // TODO DOT ?
    } else if (ep.isMagicAtk) {
      // HOT
      const auto &launcherPowMag = std::get<StatsType<int>>(
          launcherStats.m_AllStatsTable.at(STATS_POW_MAG));
      amount =
          nbOfApplies * (ep.value + launcherPowMag.m_CurrentValue / ep.nbTurns);
    }
  }
  // value in percent
  else if (isPercentChange || ep.effect == EFFECT_PERCENT_CHANGE) {
    // TODO duplicate with EFFECT_IMPROVE_BY_PERCENT_CHANGE ?
    amount = nbOfApplies * localStat.m_MaxValue * ep.value / 100;
  }
  // nominal behavior
  else {
    amount = nbOfApplies * ep.value;
  }

  // new value of stat
  amount = sign * amount; // apply the sign after the calcul of amount
  localStat.m_CurrentValue += min(delta, amount);

  // return the true applied amount
  // add buf
  if (m_BufDamage.m_Value > 0) {
    if (m_BufDamage.m_IsPercent) {
      amount += amount * m_BufDamage.m_Value / 100;
    } else {
      amount += m_BufDamage.m_Value;
    }
  }
  // is it a critical strike
  amount = ProcessCriticalStrike(amount);
  if (ep.statsName == STATS_DODGE || ep.statsName == STATS_CRIT) {
    output = amount;
  } else {
    output = min(delta, amount);
  }

  return output;
}

QString Character::ProcessOutputLogOnEffect(const effectParam &ep,
                                            const int amount,
                                            const bool fromLaunch,
                                            const int nbOfApplies,
                                            const QString &atkName) const {
  QString output;
  QString healOrDamageLog;
  uint32_t displayedValue = abs(amount);
  QString effectName;
  if (ep.effect.isEmpty() || !fromLaunch) {
    effectName = ep.statsName;
  } else {
    // for example EFFECT_NB_DECREASE_ON_TURN is only from launch
    effectName = ep.statsName + "-" + ep.effect;
  }
  if (amount < 0) {
    healOrDamageLog = "inflige";
  } else if (amount > 0) {
    healOrDamageLog = "récupère";
  } else if (amount == 0) {
    return QString("%1 n'a pas d'effet.").arg(effectName);
  }
  // nominal atk
  int potentialAttempts = nbOfApplies;
  if (ep.effect == EFFECT_NB_DECREASE_ON_TURN) {
    // the nomical case is 0 for any atk. effect.subValueEffect is on top of the
    potentialAttempts = ep.subValueEffect;
  }

  if (ep.statsName == STATS_HP) {
    if (fromLaunch) {
      output = QString("%1 %5 PV grâce à l'effet %2 (appliqué %3/%4 "
                       "possible(s)).")
                   .arg(healOrDamageLog)
                   .arg(effectName)
                   .arg(nbOfApplies)
                   .arg(QString::number(potentialAttempts))
                   .arg(QString::number(displayedValue));
    } else {
      output = QString("%1 %3 PV grâce à l'effet %2 (%4).")
                   .arg(healOrDamageLog)
                   .arg(effectName)
                   .arg(QString::number(displayedValue))
                   .arg(atkName);
    }
  } else if (ep.statsName != STATS_HP) {
    output = QString("l'effet %1 s'applique %2/%3 "
                     "possible(s) avec une valeur de %4.")
                 .arg(effectName)
                 .arg(nbOfApplies)
                 .arg(QString::number(potentialAttempts))
                 .arg(QString::number(displayedValue));
  }

  return output;
}

int Character::ProcessDecreaseOnTurn(const effectParam &ep) const {
  int nbOfApplies = 0; // default value 0 for the nominal case
  int counter = ep.subValueEffect;
  while (counter > 0) {
    const int intMin = 0;
    const int intMax = 100;
    const int stepLimit = (intMax / ep.subValueEffect); // get percentual
    const auto maxLimit = stepLimit * counter;
    if (const auto randNb = Utils::GetRandomNb(intMin, intMax);
        randNb >= 0 && randNb < maxLimit) {
      nbOfApplies++;
    } else {
      break;
    }
    counter--;
  }
  return nbOfApplies;
}

QString Character::ProcessDecreaseByTurn(const effectParam &ep) const {
  QString output;

  const int intMin = 0;
  const int intMax = 100;
  const int stepLimit = (intMax / ep.nbTurns); // get percentual
  const auto maxLimit = stepLimit * (ep.nbTurns - ep.counterTurn);
  if (const auto randNb = Utils::GetRandomNb(intMin, intMax);
      !(randNb >= 0 && randNb < maxLimit)) {
    output = QString("%1 n'a pas d'effet").arg(EFFECT_NB_DECREASE_BY_TURN);
  }

  return output;
}

void Character::SetBuf(const int value, const bool isPercent) {
  m_BufDamage.SetBuf(value, isPercent);
}

void Character::SetStatsOnEffect(StatsType<int> &stat, const int value,
                                  const bool isUp, const bool isPercent) {
  int div = 1;
  if (isPercent) {
    div = 100;
  }
  int sign = 1;
  if (!isUp) {
    sign = -1;
  }
  const double ratio = (stat.m_MaxValue > 0)
                           ? static_cast<double>(stat.m_CurrentValue) /
                                 static_cast<double>(stat.m_MaxValue)
                           : 1;
  const auto baseValue = stat.m_RawMaxValue + stat.m_BufEquipValue +
                         stat.m_BufEquipPercent * stat.m_RawMaxValue / 100;
  stat.m_MaxValue += sign * baseValue * value / div;
  stat.m_CurrentValue += static_cast<int>(sign * std::round(stat.m_MaxValue * ratio));
}

int Character::GetMaxNbOfApplies(const AttaqueType &atk) const {
  int maxNb = 0;
  int cost = 0;
  QString statName;
  if (atk.manaCost > 0) {
    cost = atk.manaCost;
    statName = STATS_MANA;
  }
  if (atk.vigorCost > 0) {
    cost = atk.vigorCost;
    statName = STATS_VIGOR;
  }
  if (atk.berseckCost > 0) {
    cost = atk.berseckCost;
    statName = STATS_BERSECK;
  }
  if (m_Stats.m_AllStatsTable.count(statName) == 0 || cost == 0) {
    return 0;
  }

  const auto &localStat =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable.at(statName));

  return maxNb = localStat.m_CurrentValue / cost;
}

// apply amount on berseck character
// In case of character working with Berseck, an atk from an ennemy up his
// berseck gauge.
int Character::ProcessBerseckOnRxAtk(const int nbOfApplies) {
  auto &berseckStat =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_BERSECK]);
  const auto &berseckRate =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_RATE_BERSECK]);
  const int delta = berseckStat.m_MaxValue - berseckStat.m_CurrentValue;
  int amount = 0;
  if (berseckStat.m_MaxValue > 0) {
    amount = std::min(delta, berseckRate.m_MaxValue * nbOfApplies);
    berseckStat.m_CurrentValue += amount;
  }

  return amount;
}

std::pair<QString, int>
Character::ProcessEffectType(effectParam &effect, Character *target,
                             const AttaqueType &atk) const {
  if (target == nullptr) {
    return std::make_pair("", 0);
  }

  auto &pm = Application::GetInstance().m_GameManager->m_PlayersManager;

  QString output;
  int nbOfApplies = 1; // default value 1 for the nominal case

  if (effect.effect == EFFECT_NB_DECREASE_BY_TURN) {
    // TODO not ready to be used yet
    output = ProcessDecreaseByTurn(effect);
  }
  if (effect.effect == EFFECT_NB_COOL_DOWN) {
    output = (m_Name == target->m_Name)
                 ? QString("Cooldown actif sur %1 de %2 tours.")
                       .arg(atk.name)
                       .arg(effect.nbTurns)
                 : "";
  }
  if (effect.effect == EFFECT_NB_DECREASE_ON_TURN) {
    nbOfApplies = ProcessDecreaseOnTurn(effect);
    effect.value *= nbOfApplies;
  }
  if (effect.effect == EFFECT_REINIT) {
    pm->ResetCounterOnOneStatsEffect(target, effect.statsName);
    nbOfApplies = 0;
    if (effect.value == 0) {
      output = QString("Les HOTs sont reinitialisés.");
    }
  }
  if (effect.effect == EFFECT_DELETE_BAD) {
    if (effect.subValueEffect <= 1) {
      output = pm->DeleteOneBadEffect(target);
    } else {
      output = pm->DeleteAllBadEffect(target);
    }
  }
  if (effect.effect == EFFECT_IMPROVE_HOTS) {
    pm->ImproveHotsOnPlayers(effect.subValueEffect, target->m_type);
    output =
        QString("Les HOTs sont boostés de %1%.").arg(effect.subValueEffect);
  }
  if (effect.effect == EFFECT_BOOSTED_BY_HOTS) {
    const auto nbHots = pm->GetNbOfStatsInEffectList(target, STATS_HP);
    effect.value = effect.value * (effect.subValueEffect / 100) * nbHots;
  }
  if (effect.effect == EFFECT_CHANGE_MAX_DAMAGES_BY_PERCENT) {
    target->SetBuf(effect.value, true);
    output = QString("Les dégâts sont boostés de %1% pour %2 tours.")
                 .arg(effect.value)
                 .arg(effect.nbTurns);
  }
  if (effect.effect == EFFECT_IMPROVE_BY_PERCENT_CHANGE) {
    const QChar sign = GetCharEffectValue(effect.target);
    const auto signBool = static_cast<bool>(GetSignEffectValue(effect.target));
    // common init
    auto &localStat = std::get<StatsType<int>>(
        target->m_Stats.m_AllStatsTable[effect.statsName]);
    SetStatsOnEffect(localStat, effect.value, signBool, true);
    output = QString("La stat %1 est modifié de %2%3%.")
                 .arg(effect.statsName)
                 .arg(sign)
                 .arg(effect.value);
  }
  if (effect.effect == EFFECT_VALUE_CHANGE) {
    const QChar sign = GetCharEffectValue(effect.target);
    const auto signBool = static_cast<bool>(GetSignEffectValue(effect.target));
    // common init
    auto &localStat = std::get<StatsType<int>>(
        target->m_Stats.m_AllStatsTable[effect.statsName]);
    SetStatsOnEffect(localStat, effect.value, signBool, false);
    output = QString("La stat %1 est modifié de %2%3%.")
                 .arg(effect.statsName)
                 .arg(sign)
                 .arg(effect.value);
  }
  // only for aggro
  if (effect.effect == EFFECT_REINIT && effect.statsName == STATS_AGGRO) {
    auto &localStat = std::get<StatsType<int>>(
        target->m_Stats.m_AllStatsTable[effect.statsName]);
    localStat.m_CurrentValue = 0;
  }
  if (effect.effect == EFFECT_REPEAT_AS_MANY_AS) {
    nbOfApplies += GetMaxNbOfApplies(atk);
  }

  return std::make_pair(output, nbOfApplies);
}

QString Character::ProcessAggro(const int atkValue, const QString &statsName) {
  if (atkValue <= 0) {
    return "";
  }
  if (statsName != STATS_HP) {
    return "";
  }
  const int aggroNorm = 50; // random value at the moment
  auto &aggroStat =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_AGGRO]);
  const auto genAggro = static_cast<int>(std::round(atkValue / aggroNorm));
  aggroStat.m_CurrentValue += genAggro;

  return QString("L'aggro monte de +%1").arg(genAggro);
}

int Character::ProcessCriticalStrike(const int atkValue) const {
  int critAmount = atkValue;
  if (atkValue > 0) {
    // this is a heal not an atk
    return critAmount;
  }
  const auto &critStat =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable.at(STATS_CRIT));
  if (const auto randNb = Utils::GetRandomNb(0, 100);
      randNb >= 0 && randNb < critStat.m_CurrentValue) {
    critAmount = atkValue * 2;
  }
  return critAmount;
}

bool Character::IsDodging() const {
  bool isDodging = false;
  const auto &stat =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable.at(STATS_DODGE));
  if (const auto randNb = Utils::GetRandomNb(0, 100);
      randNb >= 0 && randNb < stat.m_CurrentValue) {
    isDodging = true;
  }
  return isDodging;
}

void Character::UsePotion(const QString &statsName) {
  if (m_Stats.m_AllStatsTable.count(statsName) == 0) {
    return;
  }
  auto &stat = std::get<StatsType<int>>(m_Stats.m_AllStatsTable.at(statsName));
  int boost = 0;
  if (statsName == STATS_HP) {
    boost = 50;
  }
  if (statsName == STATS_BERSECK) {
    boost = 20;
  }
  if (statsName == STATS_VIGOR) {
    boost = 50;
  }
  if (statsName == STATS_MANA) {
    boost = 50;
  }
  stat.m_CurrentValue = std::min(stat.m_CurrentValue + boost, stat.m_MaxValue);
}

void Character::AddExp(const int newXp) {
  m_Exp += newXp;

  while (m_Exp >= m_NextLevel) {
    m_Level += 1;
    m_NextLevel += m_NextLevel * 20 / 100;
    UpdateStatsToNextLevel();
  }
}

void Character::SetEquipment(
    const std::unordered_map<QString, QString> &table) {
  const auto pm = Application::GetInstance().m_GameManager->m_PlayersManager;
  for (auto &[bodyPart, stuff] : m_WearingEquipment) {

    for (auto &stat : ALL_STATS) {
      if (m_Stats.m_AllStatsTable.count(stat) == 0) {
        continue;
      }
      ProcessRemoveEquip(
          std::get<StatsType<int>>(m_Stats.m_AllStatsTable[stat]),
          std::get<StatsType<int>>(stuff.m_Stats.m_AllStatsTable[stat]));
    }

    if (table.count(bodyPart) == 1 && pm->m_Equipments.count(bodyPart) == 1) {
      stuff = pm->m_Equipments[bodyPart][table.at(bodyPart)];
    } else {
      // TODO maybe have an equipment by default
      for (auto &stat : ALL_STATS) {
        if (stuff.m_Stats.m_AllStatsTable.count(stat) == 0) {
          continue;
        }
        stuff.m_Name = "";
        auto &local =
            std::get<StatsType<int>>(stuff.m_Stats.m_AllStatsTable[stat]);
        local.InitValues(0, 0, 0, 0);
      }
    }
  }
}

void Character::UpdateEquipmentOnJson() const {
  // init json doc
  QJsonObject obj;
  for (const auto &[bodyPart, equip] : m_WearingEquipment) {
    if (bodyPart.isEmpty()) {
      continue;
    }
    obj.insert(bodyPart, equip.m_Name);
  }
  // output attak json
  QJsonDocument doc(obj);
  QString directoryPath =
      OFFLINE_WEARING_EQUIPMENT; // Replace with the actual path
  if (QDir directory(directoryPath); !directory.exists()) {
    qDebug() << "Directory does not exist: " << directoryPath;
  }
  QFile json(directoryPath + m_Name + ".json");
  if (!json.open(QFile::WriteOnly | QFile::Text)) {
    Application::GetInstance().log(" Could not open the file for reading " +
                                   directoryPath + m_Name + ".json");
  }
  QTextStream out(&json);
#if QT_VERSION_MAJOR == 6
  out.setEncoding(QStringConverter::Encoding::Utf8);
#else
  out.setCodec("UTF-8");
#endif
  out << doc.toJson() << "\n";
}

void Character::UpdateStatsToNextLevel() {
  for (const auto &stat : STATS_TO_LEVEL_UP) {
    if (m_Stats.m_AllStatsTable.count(stat) == 0) {
      continue;
    }
    auto &localStat = std::get<StatsType<int>>(m_Stats.m_AllStatsTable[stat]);
    localStat.m_RawMaxValue += localStat.m_RawMaxValue * 10 / 100;

    // update current value and max value
    ApplyEquipOnStats();
    // re apply effects
  }
}
