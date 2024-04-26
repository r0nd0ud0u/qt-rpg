#include "character.h"

// decode json
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QtDebug>

#include "Application.h"
#include "rust-rpg-bridge/utils.h"

#include "playersmanager.h"

#include <cmath>

using namespace std;

Character::Character() { InitTables(); }

Character::Character(const QString name, const characType type,
                     const Stats &stats)
    : m_Name(name), m_type(type), m_Stats(stats) {
  InitTables();
  m_ExtCharacter = try_new_ext_character().into_raw();
  if (m_type == characType::Boss) {
    m_Level = INT_MAX;
  }
}

void Character::InitTables() {
  for (const auto &e : ALL_EQUIP) {
    m_WearingEquipment[e].m_Name = "";
  }
  // init buf
  for (int i = 0; i < static_cast<int>(BufTypes::enumSize); i++) {
    auto buf = buffers_new().into_raw();
    m_AllBufs.push_back(buf);
  }
  m_LastTxRx.resize(static_cast<int>(amountType::enumSize));

  m_Power.is_crit_heal_after_crit = false;
  m_Power.is_damage_tx_heal_needy_ally = false;
}

int Character::DamageByAtk(const Stats &launcherStats, const Stats &targetStats,
                           const bool isMagicAtk, const int atkValue,
                           const int nbTurns) {
  const auto &launcherPowMag = launcherStats.m_AllStatsTable.at(STATS_POW_MAG);
  const auto &launcherPowPhy = launcherStats.m_AllStatsTable.at(STATS_POW_PHY);
  const auto &targetArmPhy = targetStats.m_AllStatsTable.at(STATS_ARM_PHY);
  const auto &targetArmMag = targetStats.m_AllStatsTable.at(STATS_ARM_MAG);

  int finalDamage = 0;
  int arm = 0;
  int damage = atkValue;
  if (isMagicAtk) {
    damage -= launcherPowMag.m_CurrentValue / nbTurns;
    arm = targetArmMag.m_CurrentValue;
  } else {
    damage -= launcherPowPhy.m_CurrentValue / nbTurns;
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
  if (m_type == characType::Boss) {
    playerList = pm->m_HeroesList;
  } else if (m_type == characType::Hero) {
    playerList = pm->m_BossesList;
  }

  // TODO could be change by a buf
  for (const auto &e : pm->m_AllEffectsOnGame.at(m_Name)) {
    if (e.allAtkEffects.effect == EFFECT_INTO_DAMAGE &&
        statsName == e.allAtkEffects.statsName) {
      for (auto *pl : playerList) {
        const auto finalDamage =
            atkValue * e.allAtkEffects.subValueEffect / 100;
        auto &localstat = pl->m_Stats.m_AllStatsTable[statsName];
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
  auto &launcherMana = m_Stats.m_AllStatsTable[STATS_MANA];
  auto &launcherBerseck = m_Stats.m_AllStatsTable[STATS_BERSECK];
  auto &launcherVigor = m_Stats.m_AllStatsTable[STATS_VIGOR];

  // Cost
  // mana cost in %
  launcherMana.m_CurrentValue =
      max(0, static_cast<int>(launcherMana.m_CurrentValue -
                              atk.manaCost * launcherMana.m_RawMaxValue / 100));
  launcherVigor.m_CurrentValue = max(
      0, static_cast<int>(launcherVigor.m_CurrentValue -
                          atk.vigorCost * launcherVigor.m_RawMaxValue / 100));
  launcherBerseck.m_CurrentValue = max(
      0, static_cast<int>(launcherBerseck.m_CurrentValue - atk.berseckCost));
}

void Character::AddAtq(const AttaqueType &atq) { m_AttakList[atq.name] = atq; }

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
      atk.form = json[ATK_FORM].toString();
      atk.level = static_cast<uint8_t>(json[ATK_LEVEL].toInt());
      atk.manaCost = static_cast<uint32_t>(json[ATK_MANA_COST].toInt());
      atk.vigorCost = static_cast<uint32_t>(json[ATK_VIGOR_COST].toInt());
      atk.berseckCost = static_cast<uint32_t>(json[ATK_BERSECK_COST].toInt());
      atk.reach = json[ATK_REACH].toString();
      atk.target = json[ATK_TARGET].toString();
      QJsonArray effectArray = json[EFFECT_ARRAY].toArray();
#if QT_VERSION_MAJOR == 6
      for (const auto &effect : effectArray) {
        if (effect[EFFECT_STAT].toString().isEmpty() &&
            effect[EFFECT_TYPE].toString().isEmpty()) {
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
        if (atk.nature.is_heal) {
          atk.nature.is_heal = is_heal_effect(param.statsName.toStdString(),
                                              param.target.toStdString());
        }

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

void Character::ApplyEquipOnStats(const std::vector<GameAtkEffects> &allGae) {

  for (const auto &[body, stuff] : m_WearingEquipment) {
    if (stuff.m_Name.isEmpty()) {
      continue;
    }
    for (const auto &stats : ALL_STATS) {
      if (m_Stats.m_AllStatsTable.count(stats) == 1 &&
          stuff.m_Stats.m_AllStatsTable.count(stats) > 0) {
        ProcessAddEquip(m_Stats.m_AllStatsTable[stats],
                        stuff.m_Stats.m_AllStatsTable.at(stats));
      }
    }
  }
  ApplyEffeftOnStats(false, allGae);
}

void Character::ApplyEffeftOnStats(const bool updateEffect,
                                   const std::vector<GameAtkEffects> &allGae) {
  for (const auto &gae : allGae) {
    if (gae.allAtkEffects.effect == EFFECT_IMPROVE_BY_PERCENT_CHANGE) {
      // common init
      auto &localStat = m_Stats.m_AllStatsTable[gae.allAtkEffects.statsName];
      SetStatsOnEffect(localStat, gae.allAtkEffects.value, true, updateEffect);
    } else if (gae.allAtkEffects.effect == EFFECT_IMPROVEMENT_STAT_BY_VALUE) {
      // common init
      auto &localStat = m_Stats.m_AllStatsTable[gae.allAtkEffects.statsName];
      SetStatsOnEffect(localStat, gae.allAtkEffects.value, false, updateEffect);
    }
  }
}

void Character::ProcessAddEquip(StatsType &charStat,
                                const StatsType &equipStat) {
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
      static_cast<int>(std::round(charStat.m_MaxValue * ratio));
}

void Character::ProcessRemoveEquip(StatsType &charStat,
                                   const StatsType &equipStat) {
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
      static_cast<int>(std::round(charStat.m_MaxValue * ratio));
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
  const auto &mana = m_Stats.m_AllStatsTable.at(STATS_MANA);
  const auto &berseck = m_Stats.m_AllStatsTable.at(STATS_BERSECK);
  const auto &vigor = m_Stats.m_AllStatsTable.at(STATS_VIGOR);

  // check the impact of an effect on an atk here
  // Cooldown
  for (const auto &e : atk.m_AllEffects) {
    if (e.effect == EFFECT_NB_COOL_DOWN) {
      const auto &pm =
          Application::GetInstance().m_GameManager->m_PlayersManager;
      for (const auto &gae : pm->m_AllEffectsOnGame[m_Name]) {
        if (atk.name == gae.atk.name && gae.allAtkEffects.effect == e.effect) {
          return false;
        }
      }
    }
    if (e.statsName == STATS_HP && ALLIES_TARGETS.count(e.target) > 0 &&
        m_ExtCharacter != nullptr &&
        m_ExtCharacter->get_is_heal_atk_blocked()) {
      return false;
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

std::pair<QString, std::vector<effectParam>>
Character::ApplyOneEffect(Character *target, effectParam &effect,
                          const bool fromLaunch, const AttaqueType &atk,
                          const bool reload, const bool isCrit) {
  std::vector<effectParam> newEffects = {};
  if (target == nullptr) {
    return std::make_pair("No  target character", newEffects);
  }
  QString result;

  // increment counter turn, effect is used
  // it means update only the dot and hot, not the changes on max values of
  // stats can be improved
  if (fromLaunch && effect.effect != EFFECT_IMPROVE_BY_PERCENT_CHANGE &&
      effect.effect != EFFECT_IMPROVEMENT_STAT_BY_VALUE) {
    effect.counterTurn++;
  }

  // conditions
  if (effect.effect == CONDITION_ENNEMIES_DIED) {
    const auto gs = Application::GetInstance().m_GameManager->m_GameState;
    effect.value +=
        static_cast<int>(gs->m_DiedEnnemies.count(gs->m_CurrentTurnNb - 1)) *
        effect.subValueEffect;
    effect.effect = EFFECT_IMPROVE_BY_PERCENT_CHANGE;
  }

  // Apply some effects only at launch
  if (!fromLaunch && ACTIVE_EFFECTS_ON_LAUNCH.count(effect.effect) > 0) {
    return std::make_pair("", newEffects);
  }
  // up/down % stats must be effective only at launch
  if ((effect.statsName == STATS_DODGE || effect.statsName == STATS_CRIT) &&
      (!fromLaunch && !reload)) {
    return std::make_pair("", newEffects);
  }
  // Stats other than HOT or DOT should not be updated each turn
  if (effect.statsName != STATS_HP && effect.effect == EFFECT_VALUE_CHANGE &&
      (!fromLaunch && !reload)) {
    return std::make_pair("", newEffects);
  }

  // Apply crit on effects
  const std::set<QString> BOOSTED_EFFECTS_BY_CRIT{
      EFFECT_IMPROVE_BY_PERCENT_CHANGE, EFFECT_IMPROVEMENT_STAT_BY_VALUE};
  if (isCrit && BOOSTED_EFFECTS_BY_CRIT.count(effect.effect) > 0) {
    effect.subValueEffect =
        std::lround(AttaqueType::COEFF_CRIT_STATS *
                    static_cast<double>(effect.subValueEffect));
    effect.value = std::lround(AttaqueType::COEFF_CRIT_STATS *
                               static_cast<double>(effect.value));
  }
  const auto &[effectLog, nbOfApplies] = ProcessEffectType(effect, target, atk);
  result += effectLog;

  // apply amount on berseck character if target is ennemy
  if (const bool isOnEnnemy = effect.target == TARGET_ENNEMY;
      effect.statsName == STATS_HP && isOnEnnemy) {
    const auto berseckAmount = target->ProcessBerseckOnRxAtk(nbOfApplies);
    result += (berseckAmount > 0) ? QString("recupère +%1 de râge.\n") : "";
  }
  // apply the effect
  const auto [maxAmountSent, realAmountSent] = ProcessCurrentValueOnEffect(
      effect, nbOfApplies, m_Stats, fromLaunch, target, isCrit);

  // TODO should be static and not on target ? pass target by argument
  result += target->ProcessOutputLogOnEffect(
      effect, maxAmountSent, fromLaunch, nbOfApplies, atk.name, realAmountSent);
  // Apply regen effect turning into damage for all bosses
  // can be processed only after calcul of amount of atk
  if (!reload) {
    result += RegenIntoDamage(realAmountSent, effect.statsName);
    // apply buf
    const auto *buf =
        target->m_AllBufs[static_cast<int>(BufTypes::changeByHealValue)];
    if (realAmountSent > 0 && buf != nullptr && buf->get_is_passive_enabled()) {
      const auto &stats = buf->get_all_stat_name();
      // update of the value of the buffer by trueAmount
      // This way, when effect is removed, the effect can be removed
      std::for_each(stats.begin(), stats.end(), [&](const ::rust::String &str) {
        effectParam ep;
        ep.effect = EFFECT_IMPROVEMENT_STAT_BY_VALUE;
        ep.value = realAmountSent;
        ep.isMagicAtk = true;
        ep.statsName = str.data();
        ep.nbTurns = static_cast<int>(buf->get_value());
        const auto &[effectLog, nbOfApplies] =
            ProcessEffectType(ep, target, atk);

        // TODO should be static and not on target ? pass target by argument
        result += target->ProcessOutputLogOnEffect(ep, ep.value, fromLaunch, 1,
                                                   atk.name, ep.value);
        newEffects.push_back(ep);
      });
    }
  }

  // Process aggro
  if (effect.effect != EFFECT_IMPROVEMENT_STAT_BY_VALUE &&
      effect.effect != EFFECT_IMPROVE_BY_PERCENT_CHANGE) {
    if (effect.statsName == STATS_HP) {
      // process aggro for the launcher
      result += ProcessAggro(realAmountSent, 0);
    } else if (effect.statsName == STATS_AGGRO) {
      // Add aggro to a target
      result += target->ProcessAggro(0, effect.value);
    }
  }

  // update effect value
  // keep the calcultated value for the HOT or DOT
  const auto gs = Application::GetInstance().m_GameManager->m_GameState;
  if (effect.statsName == STATS_HP &&
      EFFECTS_HOT_OR_DOT.count(effect.effect) > 0) {
    effect.value = maxAmountSent;
    // case 0 is not saved in m_LastTxRx
    if (realAmountSent > 0) {
      target->m_LastTxRx[static_cast<int>(amountType::healRx)]
                        [gs->m_CurrentTurnNb] += realAmountSent;
      m_LastTxRx[static_cast<int>(amountType::healTx)][gs->m_CurrentTurnNb] +=
          realAmountSent;
    } else if (realAmountSent < 0) {
      m_LastTxRx[static_cast<int>(amountType::damageTx)][gs->m_CurrentTurnNb] +=
          std::abs(realAmountSent);
    }
    target->m_LastTxRx[static_cast<int>(amountType::overHealRx)]
                      [gs->m_CurrentTurnNb] += maxAmountSent - realAmountSent;
  }

  return std::make_pair(result, newEffects);
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
                          Character *target, const bool isCrit) {
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
    if (effect.reach == REACH_RAND_INDIVIDUAL &&
        target->m_ExtCharacter != nullptr &&
        !target->m_ExtCharacter->get_is_random_target()) {
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
                  "stats %4 requis.\n")
              .arg(effect.statsName)
              .arg(effect.effect)
              .arg(effect.subValueEffect)
              .arg(effect.statsName));
      break;
    }
    const auto gs = Application::GetInstance().m_GameManager->m_GameState;
    // Condition number of died ennemies
    if (effect.effect == CONDITION_ENNEMIES_DIED) {
      {
        conditionsAreOk = false;
        allResultEffects.append(
            QString(
                "Pas d'effect %1 activé. Aucun ennemi mort au tour précédent\n")
                .arg(effect.effect));
        break;
      }
    }
    // Atk launched if the character did some damages on the previous turn
    if (effect.effect == CONDITION_DMG_PREV_TURN) {
      if (!(gs->m_CurrentTurnNb > 1 &&
            m_LastTxRx[static_cast<uint64_t>(amountType::damageTx)].count(
                gs->m_CurrentTurnNb - 1) > 0)) {
        conditionsAreOk = false;
        allResultEffects.append(QString("Pas d'effet activé. Pas de dégâts "
                                        "infligés au tour précédent.\n")
                                    .arg(effect.effect));
        break;
      }
    }

    effectParam appliedEffect = effect;
    // appliedEffect is modified in ApplyOneEffect
    const auto [resultEffect, newEffects] =
        ApplyOneEffect(target, appliedEffect, true, atk, false, isCrit);
    // an one-occurence(one turn) or more effect available is displayed
    if (!resultEffect.isEmpty()) {
      allResultEffects.append(resultEffect);
    }
    // from two-occurences(more than one turn) an effect is stored, result
    // effect must be not empty it means the effect has an effect -> TODO can be
    // replaced by a boolean applied effect to process in ApplyOneEffect
    if (appliedEffect.nbTurns - appliedEffect.counterTurn > 0 &&
        !resultEffect.isEmpty()) {
      allAppliedEffects.push_back(appliedEffect);
    }
    // update allAppliedEffects by newEffects created
    std::for_each(
        newEffects.begin(), newEffects.end(),
        [&](const effectParam &e) { allAppliedEffects.push_back(e); });
  }

  return std::make_tuple(conditionsAreOk, allResultEffects, allAppliedEffects);
}

void Character::RemoveMalusEffect(const effectParam &ep) {
  // remove malus applied on stats
  if (!ep.statsName.isEmpty() &&
      m_Stats.m_AllStatsTable.count(ep.statsName) > 0) {
    auto &localStat = m_Stats.m_AllStatsTable.at(ep.statsName);

    if (ep.effect == EFFECT_IMPROVE_BY_PERCENT_CHANGE) {
      SetStatsOnEffect(localStat, -ep.value, true, true);
    }
    if (ep.effect == EFFECT_IMPROVEMENT_STAT_BY_VALUE) {
      SetStatsOnEffect(localStat, -ep.value, false, true);
    }
    if (ep.effect == EFFECT_UPDATE_TURN_RATE) {
      localStat.m_RegenOnTurn -= ep.value;
    }
  }
  if (ep.effect == EFFECT_BLOCK_HEAL_ATK && m_ExtCharacter != nullptr) {
    m_ExtCharacter->set_is_heal_atk_blocked(false);
  }

  // remove debuf/ buf
  // on damages
  if (ep.effect == EFFECT_CHANGE_MAX_DAMAGES_BY_PERCENT) {
    UpdateBuf(BufTypes::damageTx, -ep.value, true, {});
  }
}

std::pair<int, int> Character::ProcessCurrentValueOnEffect(
    effectParam &ep, const int nbOfApplies, const Stats &launcherStats,
    const bool launch, Character *target, const bool isCrit) const {

  if (ep.statsName.isEmpty() || target == nullptr) {
    return std::make_pair(0, 0);
  }
  if (ep.value == 0) {
    return std::make_pair(0, 0);
  }
  // TODO maybe the only effects should be effect_value change and percent ?
  if (ep.effect == EFFECT_IMPROVE_BY_PERCENT_CHANGE ||
      ep.effect == EFFECT_IMPROVEMENT_STAT_BY_VALUE ||
      ep.effect == EFFECT_BUF_VALUE_AS_MUCH_AS_HEAL ||
      ep.effect == EFFECT_UPDATE_TURN_RATE) {
    return std::make_pair(0, 0);
  }
  int output = 0;

  // common init
  auto &localStat = target->m_Stats.m_AllStatsTable[ep.statsName];
  const int delta = localStat.m_MaxValue - localStat.m_CurrentValue;
  int amount = 0;

  // HP
  if (ep.statsName == STATS_HP && ep.effect == EFFECT_NB_DECREASE_ON_TURN) {
    if (launch) {
      // HOT
      const auto &launcherPowMag =
          launcherStats.m_AllStatsTable.at(STATS_POW_MAG);
      amount =
          nbOfApplies * (ep.value + launcherPowMag.m_CurrentValue / ep.nbTurns);
      ep.value = amount;
    } else {
      amount = ep.value;
    }
  } else if (launch && ep.statsName == STATS_HP &&
             EFFECTS_HOT_OR_DOT.count(ep.effect) > 0) {
    if (ep.value < 0) {
      amount = nbOfApplies * DamageByAtk(launcherStats, target->m_Stats,
                                         ep.isMagicAtk, ep.value, ep.nbTurns);
    } else if (ep.isMagicAtk) {
      // HOT or DOT
      const auto &launcherPowMag =
          launcherStats.m_AllStatsTable.at(STATS_POW_MAG);
      amount =
          nbOfApplies * (ep.value + launcherPowMag.m_CurrentValue / ep.nbTurns);
    } else {
      amount = nbOfApplies * ep.value;
    }
  }
  // value in percent
  else if (ep.effect == EFFECT_PERCENT_CHANGE) {
    amount = nbOfApplies * localStat.m_MaxValue * ep.value / 100;
  } else {
    amount = nbOfApplies * ep.value;
  }

  if (amount == 0) {
    return std::make_pair(0, 0);
  }

  if (ep.statsName == STATS_HP) {
    // return the true applied amount
    // add buf
    if (target != nullptr && ep.target == TARGET_ENNEMY && launch) {
      // launcher buf
      if (const auto *bufTx = m_AllBufs[static_cast<int>(BufTypes::damageTx)];
          bufTx != nullptr) {
        amount = static_cast<int>(update_damage_by_buf(
            bufTx->get_value(), bufTx->get_is_percent(), amount));
      }
      if (const auto *bufCrit =
              m_AllBufs[static_cast<int>(BufTypes::damageCritCapped)];
          bufCrit != nullptr) {
        amount = static_cast<int>(update_damage_by_buf(
            bufCrit->get_value(), bufCrit->get_is_percent(), amount));
      }
      // target buf
      if (const auto *bufRx =
              target->m_AllBufs[static_cast<int>(BufTypes::damageRx)];
          bufRx != nullptr) {
        amount = static_cast<int>(update_damage_by_buf(
            bufRx->get_value(), bufRx->get_is_percent(), amount));
      }
    }
    if (target != nullptr && ALLIES_TARGETS.count(ep.target) > 0 && launch) {
      // launcher buf
      if (const auto *bufMulti =
              m_AllBufs[static_cast<int>(BufTypes::multiValue)];
          bufMulti != nullptr && bufMulti->get_value() > 0) {
        amount = static_cast<int>(
            update_heal_by_multi(amount, bufMulti->get_value()));
      }
    }
  }

  // is it a critical strike
  if (isCrit && ep.statsName == STATS_HP && launch) {
    output = 2 * amount;
  } else if (isCrit && launch) {
    output = std::lround(AttaqueType::COEFF_CRIT_STATS *
                         static_cast<double>(amount));
    // value of effect is updated by crit
    ep.value = output;
  } else {
    // new value of stat
    output = amount;
  }

  if (ep.statsName != STATS_HP && ep.statsName != STATS_MANA &&
      ep.statsName != STATS_VIGOR && ep.statsName != STATS_BERSECK) {
    SetStatsOnEffect(localStat, output, (ep.effect == EFFECT_PERCENT_CHANGE),
                     true);
    return std::make_pair(output, output);
  }
  int maxAmount = 0;
  if (output > 0) {
    // heal
    localStat.m_CurrentValue =
        min(output + localStat.m_CurrentValue, localStat.m_MaxValue);
    maxAmount = min(delta, output);
  } else {
    // damage
    int tmp = localStat.m_CurrentValue;
    localStat.m_CurrentValue = max(0, localStat.m_CurrentValue + output);
    // TODO change it
    maxAmount = min(tmp, output);
  }

  return std::make_pair(output, maxAmount);
}

QString Character::ProcessOutputLogOnEffect(
    const effectParam &ep, const int amount, const bool fromLaunch,
    const int nbOfApplies, const QString &atkName, const int maxAmount) const {
  // show logs in case of heal of damage atk
  if (ep.effect == EFFECT_IMPROVE_BY_PERCENT_CHANGE ||
      ep.effect == EFFECT_IMPROVEMENT_STAT_BY_VALUE) {
    return "";
  }

  QString output;
  QString healOrDamageLog;
  int displayedValue = amount;
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
  } else if (amount == 0 && maxAmount > 0) {
    return QString("Effet %3: %1/%2\n")
        .arg(amount)
        .arg(maxAmount)
        .arg(effectName);
  } else {
    return "";
  }
  // nominal atk
  int potentialAttempts = nbOfApplies;
  if (ep.effect == EFFECT_NB_DECREASE_ON_TURN) {
    potentialAttempts = ep.subValueEffect;
  }

  if (ep.statsName == STATS_HP) {
    if (fromLaunch) {
      output = QString("%1 %5/%6 PV avec l'effet %2 (appliqué %3/%4).\n")
                   .arg(healOrDamageLog)
                   .arg(effectName)
                   .arg(nbOfApplies)
                   .arg(QString::number(potentialAttempts))
                   .arg(maxAmount)
                   .arg(QString::number(displayedValue));
    } else {
      output = QString("%1 %3/%4 PV avec l'effet %2-(%5).\n")
                   .arg(healOrDamageLog)
                   .arg(effectName)
                   .arg(maxAmount)
                   .arg(QString::number(displayedValue))
                   .arg(atkName);
    }
  } else if (ep.statsName != STATS_HP) {
    output = QString("l'effet %1: %4/%5 (appliqué %2/%3).\n")
                 .arg(effectName)
                 .arg(nbOfApplies)
                 .arg(QString::number(potentialAttempts))
                 .arg(maxAmount)
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
    if (const auto randNb = get_random_nb(intMin, intMax);
        randNb >= 0 && randNb <= maxLimit) {
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
  if (const auto randNb = get_random_nb(intMin, intMax);
      !(randNb >= 0 && randNb <= maxLimit)) {
    output = QString("%1 n'a pas d'effet").arg(EFFECT_NB_DECREASE_BY_TURN);
  }

  return output;
}

void Character::UpdateBuf(const BufTypes &bufType, const int value,
                          const bool isPercent, const QString &stat) {
  auto *buf = m_AllBufs[static_cast<int>(bufType)];
  if (buf != nullptr) {
    buf->set_buffers(buf->get_value() + value, isPercent);
    buf->add_stat_name(stat.toStdString());
  }
}

void Character::ResetBuf(const BufTypes &bufType) {
  auto *buf = m_AllBufs[static_cast<int>(bufType)];
  // Reset values {percent, decimal}
  if (buf != nullptr) {
    buf->set_buffers(0, false);
    buf->set_buffers(0, true);
  }
}

void Character::SetStatsOnEffect(StatsType &stat, const int value,
                                 const bool isPercent,
                                 const bool updateEffect) {
  const double ratio = (stat.m_MaxValue > 0)
                           ? static_cast<double>(stat.m_CurrentValue) /
                                 static_cast<double>(stat.m_MaxValue)
                           : 1;
  const auto baseValue = stat.m_RawMaxValue + stat.m_BufEquipValue +
                         stat.m_BufEquipPercent * stat.m_RawMaxValue / 100;
  stat.m_MaxValue = baseValue;
  if (updateEffect) {
    if (isPercent) {
      stat.m_BufEffectPercent += value;
    } else {
      stat.m_BufEffectValue += value;
    }
  }
  // update maxvalue with all effects
  stat.m_MaxValue +=
      stat.m_BufEffectValue + stat.m_BufEffectPercent * baseValue / 100;
  // max value is positive
  stat.m_MaxValue = std::max(0, stat.m_MaxValue);
  // calcul of current-value by max-value
  stat.m_CurrentValue = static_cast<int>(std::round(stat.m_MaxValue * ratio));
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

  const auto &localStat = m_Stats.m_AllStatsTable.at(statName);

  return maxNb = localStat.m_CurrentValue / cost;
}

// apply amount on berseck character
// In case of character working with Berseck, an atk from an ennemy up his
// berseck gauge.
int Character::ProcessBerseckOnRxAtk(const int nbOfApplies) {
  auto &berseckStat = m_Stats.m_AllStatsTable[STATS_BERSECK];
  const auto &berseckRate = m_Stats.m_AllStatsTable[STATS_RATE_BERSECK];
  const int delta = berseckStat.m_MaxValue - berseckStat.m_CurrentValue;
  int amount = 0;
  if (berseckStat.m_MaxValue > 0) {
    amount = std::min(delta, berseckRate.m_MaxValue * nbOfApplies);
    berseckStat.m_CurrentValue += amount;
  }

  return amount;
}

std::pair<QString, int> Character::ProcessEffectType(effectParam &effect,
                                                     Character *target,
                                                     const AttaqueType &atk) {
  if (target == nullptr) {
    return std::make_pair("", 0);
  }

  auto &pm = Application::GetInstance().m_GameManager->m_PlayersManager;
  const auto *gs = Application::GetInstance().m_GameManager->m_GameState;

  QString output;
  int nbOfApplies = 1; // default value 1 for the nominal case;
  int a = m_AllBufs[static_cast<int>(BufTypes::applyEffectInit)]->get_value();
  if (m_AllBufs[static_cast<int>(BufTypes::applyEffectInit)]->get_value() > 0) {
    nbOfApplies = static_cast<int>(
        m_AllBufs[static_cast<int>(BufTypes::applyEffectInit)]->get_value());
  }

  if (effect.effect == EFFECT_NB_DECREASE_BY_TURN) {
    // TODO not ready to be used yet
    output = ProcessDecreaseByTurn(effect);
  }
  if (effect.effect == EFFECT_NB_COOL_DOWN) {
    // Must be filled before changing value of nbTurns
    output = (m_Name == target->m_Name)
                 ? QString("Cooldown actif sur %1 de %2 tours.\n")
                       .arg(atk.name)
                       .arg(effect.nbTurns)
                 : "";
    // example cooldown for 2 turns
    // T1  nb turn -1
    // T2 cooldown, nb turn -1
    // T3 cooldown, nb turn -1
    // T4 nb turn -1 => effect finished => can be launched
    // => for a cooldown of n=2 turns, the init value of nbTurns = n + 2;
    effect.nbTurns = effect.nbTurns + 2;
  }
  if (effect.effect == EFFECT_NB_DECREASE_ON_TURN) {
    nbOfApplies = ProcessDecreaseOnTurn(effect);
    // At the moment. condition on "value == 0" to apply 'nbOfApplies' for all
    // the effects of the current atk
    if (effect.value == 0) {
      UpdateBuf(BufTypes::applyEffectInit, nbOfApplies, false, "");
      output = QString("L'attaque sera effectuée %1 fois\n").arg(nbOfApplies);
    }
  }
  if (effect.effect == EFFECT_REINIT) {
    pm->ResetCounterOnOneStatsEffect(target, effect.statsName);
    nbOfApplies = 0;
    if (effect.value == 0) {
      if (effect.statsName != STATS_HP) {
        output = QString("Stats %1 est reinit.\n").arg(effect.statsName);
      } else {
        output = "Chaque HOT est reinit.\n";
      }
    }
    if (effect.statsName == STATS_AGGRO) {
      target->m_LastAggros.clear();
      auto &localStat = target->m_Stats.m_AllStatsTable[effect.statsName];
      localStat.m_CurrentValue = 0;
      output = QString("Effect %1 %2 est activé.\n")
                   .arg(effect.effect)
                   .arg(STATS_AGGRO);
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
    pm->ImproveHotsOnPlayers(effect.value, target->m_type);
    output =
        QString("Les HOTs sont boostés de %1%.\n").arg(effect.subValueEffect);
  }
  if (effect.effect == EFFECT_BOOSTED_BY_HOTS) {
    const auto nbHots = pm->GetNbOfStatsInEffectList(target, STATS_HP);
    effect.value = effect.value * (effect.subValueEffect / 100) * nbHots;
  }
  if (effect.effect == EFFECT_CHANGE_MAX_DAMAGES_BY_PERCENT) {
    if (effect.target == TARGET_ENNEMY) {
      target->UpdateBuf(BufTypes::damageRx, effect.value, true, "");
      output =
          QString("Les dégâts reçus sont modifiés de %1% pendant %2 tours.\n")
              .arg(effect.value)
              .arg(effect.nbTurns);
    } else {
      target->UpdateBuf(BufTypes::damageTx, effect.value, true, "");
      output =
          QString(
              "Les dégâts transmis sont modifiés de %1% pendant %2 tours.\n")
              .arg(effect.value)
              .arg(effect.nbTurns);
    }
  }
  if (effect.effect == EFFECT_IMPROVE_BY_PERCENT_CHANGE) {
    // common init
    auto &localStat = target->m_Stats.m_AllStatsTable[effect.statsName];
    SetStatsOnEffect(localStat, nbOfApplies * effect.value, true, true);
    output = QString("La stat %1 est modifiée de %2%.\n")
                 .arg(effect.statsName)
                 .arg(nbOfApplies * effect.value);
  }
  if (effect.effect == EFFECT_IMPROVEMENT_STAT_BY_VALUE) {
    // common init
    auto &localStat = target->m_Stats.m_AllStatsTable[effect.statsName];
    SetStatsOnEffect(localStat, effect.value, false, true);
    output = QString("La stat %1 est modifiée de %2.\n")
                 .arg(effect.statsName)
                 .arg(effect.value);
  }
  if (effect.effect == EFFECT_REPEAT_AS_MANY_AS) {
    nbOfApplies += GetMaxNbOfApplies(atk);
    // one cost has already been processed at the start of launchingattak->
    // nbOfApplies-1
    for (int i = 0; i < nbOfApplies - 1; i++) {
      ProcessCost(atk.name);
    }
  }
  if (effect.effect == EFFECT_INTO_DAMAGE) {
    output = QString("%1% des sorts %2 -> en dégâts pendant %3 tours.\n")
                 .arg(effect.subValueEffect)
                 .arg(effect.statsName)
                 .arg(effect.nbTurns);
  }
  if (effect.effect == EFFECT_NEXT_HEAL_IS_CRIT) {
    auto *buf = m_AllBufs[static_cast<int>(BufTypes::nextHealAtkIsCrit)];
    if (buf != nullptr) {
      buf->set_is_passive_enabled(true);
      output = QString("Effect %1 est activé.\n").arg(effect.effect);
    }
  }

  if (effect.effect == EFFECT_BUF_MULTI) {
    // TODO at the moment condition is in that effect which is used only for
    // "fleur de vie sangunaire"
    // TODO conditions on at, conditions on effect to take buf-multi and dmg
    // condtion apart here
    if (gs->m_CurrentTurnNb > 1 &&
        m_LastTxRx[static_cast<uint64_t>(amountType::damageTx)].count(
            gs->m_CurrentTurnNb - 1) > 0) {
      UpdateBuf(BufTypes::multiValue, effect.value, true, "");
      output = QString("Le buf multi est activé.\n");
    } else {
      output = QString(
          "Le buf multi n'est pas activé. Pas de dégâts au tour précédent\n");
    }
  }

  if (effect.effect == EFFECT_BLOCK_HEAL_ATK && m_ExtCharacter != nullptr) {
    m_ExtCharacter->set_is_heal_atk_blocked(true);
    // output must be written before modifying nbTurns
    output = QString("Les attaques de soins sont bloqués pendant %1.\n")
                 .arg(effect.nbTurns);
    // Same calculation as cooldown effect
    effect.nbTurns += 2;
  }

  if (effect.effect == EFFECT_REPEAT_IF_HEAL) {
    const auto &healTxTable =
        m_LastTxRx[static_cast<uint64_t>(amountType::healTx)];
    if (healTxTable.find(gs->m_CurrentTurnNb - 1) == healTxTable.end()) {
      output = QString("Attaque non répétée. Pas de heal au précédent tour\n");
    } else {
      const auto randNb = get_random_nb(0, effect.value);
      if (randNb <= effect.value) {
        nbOfApplies += effect.subValueEffect;
        UpdateBuf(BufTypes::applyEffectInit, nbOfApplies, false, "");
      }
      output = QString("L'attaque est appliquée %1 fois. (rand nb:%2)\n")
                   .arg(nbOfApplies)
                   .arg(randNb);
    }
  }

  if (effect.effect == EFFECT_BUF_VALUE_AS_MUCH_AS_HEAL) {
    // 2 steps
    // enable the buf
    // update the stats by ther excedent of heal
    if (auto *buf =
            target->m_AllBufs[static_cast<int>(BufTypes::changeByHealValue)];
        buf != nullptr) {
      buf->set_is_passive_enabled(true);
      buf->add_stat_name(effect.statsName.toStdString());
      buf->set_buffers(effect.nbTurns, false);
    }
    output += QString("Effet soignant avec excédent de soin, activé!\n");
  }
  if (effect.effect == EFFECT_UPDATE_TURN_RATE) {
    auto &localStat = target->m_Stats.m_AllStatsTable[effect.statsName];
    localStat.m_RegenOnTurn += effect.value;
    output += QString("Update regen par valeur sur %1: %2\n")
                  .arg(effect.statsName)
                  .arg(effect.value);
  }

  if (!output.isEmpty()) {
    output += QString("Effet appliqué %1 fois\n").arg(nbOfApplies);
  }
  return std::make_pair(output, nbOfApplies);
}

/**
 * @brief Character::ProcessAggro
 * Keep only the last 5 aggro generated after a heal or a damage
 * Sum them to process the current value of stats aggro
 * the current value of aggro must be positive
 * the negative values are still kept in the 5 last value through
 * @param atkValue
 * @return a QString to output in channel log the result
 */
QString Character::ProcessAggro(const int atkValue, const int aggroValue) {
  const double aggroNorm = 20.0; // random value at the moment
  int localAggro = aggroValue;

  // Aggro filled by atkValue or input aggro value ?
  if (atkValue != 0) {
    localAggro = std::lround(static_cast<double>(abs(atkValue)) / aggroNorm);
  }
  // case null aggro
  if (localAggro == 0) {
    return "Pas d'aggro.\n";
  }
  // keep the last 5
  m_LastAggros.push_back(localAggro);
  if (m_LastAggros.size() == 6) {
    m_LastAggros.pop_front();
  }
  // update current aggro stat with the sum of the last 5, the output must be
  // positive
  auto &aggroStat = m_Stats.m_AllStatsTable[STATS_AGGRO];
  const int oldAggro = aggroStat.m_CurrentValue;
  aggroStat.m_CurrentValue =
      std::max(0, accumulate(m_LastAggros.begin(), m_LastAggros.end(), 0));

  QStringList aggroItems;
  std::for_each(
      m_LastAggros.begin(), m_LastAggros.end(),
      [&aggroItems](const int &n) { aggroItems.append(QString::number(n)); });

  return QString("Gen Aggro:%1 pour %2, old: %3, new: %4, items=(%5)\n")
      .arg(localAggro)
      .arg(m_Name)
      .arg(oldAggro)
      .arg(aggroStat.m_CurrentValue)
      .arg(aggroItems.join(","));
}

/**
 * @brief Character::ProcessCriticalStrike
 * a critical strike can be enabled if the buf nextHealAtkIsCrit has been
 * enabled or if a random number has been drawn between [0,100]
 *
 * Update m_isLastAtkCritical attribute
 * @param atk
 * @return
 */
std::pair<bool, int> Character::ProcessCriticalStrike(const AttaqueType &atk) {
  const auto &critStat = m_Stats.m_AllStatsTable.at(STATS_CRIT);
  int64_t randNb = -1;
  const int critCapped = 60;
  const int maxCritUsed = std::min(critCapped, critStat.m_CurrentValue);

  const auto &isCritByBuf =
      m_AllBufs[static_cast<int>(BufTypes::nextHealAtkIsCrit)]
          ->get_is_passive_enabled();

  bool isCrit = false;
  if (randNb = get_random_nb(0, 100); (isCritByBuf && atk.nature.is_heal) ||
                                      (randNb >= 0 && randNb <= maxCritUsed)) {
    // update buf dmg by crit capped
    UpdateBuf(BufTypes::damageCritCapped,
              min(0, critStat.m_CurrentValue - critCapped), false, "");
    // Reset isCritByBuf if it has been used
    if (isCritByBuf) {
      m_AllBufs[static_cast<int>(BufTypes::nextHealAtkIsCrit)]
          ->set_is_passive_enabled(true);
    } else {
      m_AllBufs[static_cast<int>(BufTypes::nextHealAtkIsCrit)]
          ->set_is_passive_enabled(false);
    }
    isCrit = true;
  }
  // update critical attribute
  m_isLastAtkCritical = isCrit;
  return std::make_pair(isCrit, randNb);
}

/**
 * @brief Character::IsDodging
 * An ultimate atk cannot be dodged.
 * @param atk
 * @return
 */
std::pair<bool, QString> Character::IsDodging(const AttaqueType &atk) const {
  bool isDodging = false;
  if (const int ULTIMATE_LEVEL = 13; atk.level == ULTIMATE_LEVEL) {
    return std::make_pair(isDodging, "ultimate");
  }

  const auto &stat = m_Stats.m_AllStatsTable.at(STATS_DODGE);
  const int DEFAULT_RAND = -1;
  int64_t randNb = DEFAULT_RAND;
  if (randNb = get_random_nb(0, 100);
      randNb >= 0 && randNb <= stat.m_CurrentValue) {
    isDodging = true;
  }
  return std::make_pair(isDodging, QString::number(randNb));
}

void Character::UsePotion(const QString &statsName) {
  if (m_Stats.m_AllStatsTable.count(statsName) == 0) {
    return;
  }
  auto &stat = m_Stats.m_AllStatsTable.at(statsName);
  int boost = 0;
  if (statsName == STATS_HP) {
    boost = 20;
  }
  if (statsName == STATS_BERSECK) {
    boost = 20;
  }
  if (statsName == STATS_VIGOR) {
    boost = 20;
  }
  if (statsName == STATS_MANA) {
    boost = 20;
  }
  stat.m_CurrentValue = std::min(
      stat.m_CurrentValue + boost * stat.m_MaxValue / 100, stat.m_MaxValue);
}

void Character::AddExp(const int newXp) {
  m_Exp += newXp;

  while (m_Exp >= m_NextLevel) {
    m_Level += 1;
    m_NextLevel += m_NextLevel + m_NextLevel * 20 / 100;
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
      ProcessRemoveEquip(m_Stats.m_AllStatsTable[stat],
                         stuff.m_Stats.m_AllStatsTable[stat]);
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
        auto &local = stuff.m_Stats.m_AllStatsTable[stat];
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

/**
 * @brief Character::UpdateStatsToNextLevel
 * Update the selected stats (not all) by 10% for their current raw max value
 * apply on those their equipment buf and their effect
 */
void Character::UpdateStatsToNextLevel() {
  for (const auto &stat : STATS_TO_LEVEL_UP) {
    if (m_Stats.m_AllStatsTable.count(stat) == 0) {
      continue;
    }
    auto &localStat = m_Stats.m_AllStatsTable[stat];

    // store the ratio between max value and current value
    const double ratio = (localStat.m_MaxValue > 0)
                             ? static_cast<double>(localStat.m_CurrentValue) /
                                   static_cast<double>(localStat.m_MaxValue)
                             : 1;
    // update the raw value by 10%
    localStat.m_RawMaxValue += localStat.m_RawMaxValue * 10 / 100;

    // recalcultate with equipment and effect
    localStat.m_MaxValue =
        localStat.m_RawMaxValue + localStat.m_BufEquipValue +
        localStat.m_RawMaxValue * localStat.m_BufEquipPercent / 100 +
        localStat.m_BufEffectValue +
        localStat.m_RawMaxValue * localStat.m_BufEffectPercent / 100;

    // recompute current value by ratio
    localStat.m_CurrentValue =
        static_cast<int>(std::round(localStat.m_MaxValue * ratio));
  }
}

std::vector<effectParam> Character::LoadThaliaTalent() const {
  std::vector<effectParam> epTable;

  // TODO stats improbed already in the stats
  // to change when creating new thalia forms
  //  effectParam param1;
  //  param1.effect = EFFECT_IMPROVE_BY_PERCENT_CHANGE;
  //  param1.value = 50;
  //  param1.nbTurns = 1000;
  //  param1.reach = REACH_INDIVIDUAL;
  //  param1.statsName = STATS_REGEN_MANA;
  //  param1.target = TARGET_HIMSELF;
  //  param1.subValueEffect = 0;
  //  epTable.push_back(param1);

  //  effectParam param2;
  //  param2.effect = EFFECT_IMPROVE_BY_PERCENT_CHANGE;
  //  param2.value = 10;
  //  param2.nbTurns = 1000;
  //  param2.reach = REACH_INDIVIDUAL;
  //  param2.statsName = STATS_HP;
  //  param2.target = TARGET_HIMSELF;
  //  param2.subValueEffect = 0;
  //  epTable.push_back(param2);

  //  effectParam param3;
  //  param3.effect = EFFECT_IMPROVE_BY_PERCENT_CHANGE;
  //  param3.value = 35;
  //  param3.nbTurns = 1000;
  //  param3.reach = REACH_INDIVIDUAL;
  //  param3.statsName = STATS_ARM_PHY;
  //  param3.target = TARGET_HIMSELF;
  //  param3.subValueEffect = 0;
  //  epTable.push_back(param3);

  return epTable;
}

std::vector<effectParam> Character::LoadAzrakTalent() const {
  std::vector<effectParam> epTable;

  effectParam param1;
  param1.effect = EFFECT_IMPROVEMENT_STAT_BY_VALUE;
  param1.value = 3;
  param1.nbTurns = 1000;
  param1.reach = REACH_INDIVIDUAL;
  param1.statsName = STATS_REGEN_MANA;
  param1.target = TARGET_HIMSELF;
  param1.subValueEffect = 0;
  epTable.push_back(param1);

  return epTable;
}

std::vector<effectParam> Character::LoadThrainTalent() const {
  std::vector<effectParam> epTable;

  effectParam param1;
  param1.effect = EFFECT_IMPROVEMENT_STAT_BY_VALUE;
  param1.value = 10;
  param1.nbTurns = 1000;
  param1.reach = REACH_INDIVIDUAL;
  param1.statsName = STATS_DODGE;
  param1.target = TARGET_HIMSELF;
  param1.subValueEffect = 0;
  epTable.push_back(param1);

  effectParam param2;
  param2.effect = EFFECT_IMPROVEMENT_STAT_BY_VALUE;
  param2.value = 15;
  param2.nbTurns = 1000;
  param2.reach = REACH_INDIVIDUAL;
  param2.statsName = STATS_HP;
  param2.target = TARGET_HIMSELF;
  param2.subValueEffect = 0;
  epTable.push_back(param2);

  effectParam param3;
  param3.effect = EFFECT_IMPROVE_BY_PERCENT_CHANGE;
  param3.value = 35;
  param3.nbTurns = 1000;
  param3.reach = REACH_INDIVIDUAL;
  param3.statsName = STATS_ARM_PHY;
  param3.target = TARGET_HIMSELF;
  param3.subValueEffect = 0;
  epTable.push_back(param3);

  effectParam param4;
  param4.effect = EFFECT_IMPROVE_BY_PERCENT_CHANGE;
  param4.value = 35;
  param4.nbTurns = 1000;
  param4.reach = REACH_INDIVIDUAL;
  param4.statsName = STATS_ARM_MAG;
  param4.target = TARGET_HIMSELF;
  param4.subValueEffect = 0;
  epTable.push_back(param4);

  return epTable;
}

std::vector<effectParam> Character::LoadElaraTalent() const {
  std::vector<effectParam> epTable;

  effectParam param1;
  param1.effect = EFFECT_IMPROVE_BY_PERCENT_CHANGE;
  param1.value = 10;
  param1.nbTurns = 1000;
  param1.reach = REACH_ZONE;
  param1.statsName = STATS_ARM_MAG;
  param1.target = TARGET_ALL_HEROES;
  param1.subValueEffect = 0;
  epTable.push_back(param1);

  return epTable;
}

int Character::UpdateDamageByBuf(const Buffers *bufDmg, const int value) {
  if (bufDmg == nullptr) {
    return value;
  }
  int output = value;
  if (bufDmg->get_value() > 0) {
    if (bufDmg->get_is_percent()) {
      output += output * bufDmg->get_value() / 100;
    } else if (output > 0) {
      output += bufDmg->get_value();
    }
  }

  return output;
}

void Character::SetValuesForThalia(const bool isBear) {
  auto &localstat = m_Stats.m_AllStatsTable[STATS_BERSECK];
  if (isBear) {
    localstat.InitValues(20, 20, 100, 0);
  } else {
    localstat.InitValues(0, 0, 0, 0);
  }
}
