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

int Character::DamageByAtk(Character *target, const AttaqueType &atk) {
  const auto &launcherPowMag =
      std::get<StatsType<double>>(m_Stats.m_AllStatsTable[STATS_POW_MAG]);
  const auto &launcherPowPhy =
      std::get<StatsType<int>>(m_Stats.m_AllStatsTable[STATS_POW_PHY]);
  const auto &targetArmPhy =
      std::get<StatsType<int>>(target->m_Stats.m_AllStatsTable[STATS_ARM_PHY]);
  const auto &targetArmMag =
      std::get<StatsType<int>>(target->m_Stats.m_AllStatsTable[STATS_ARM_MAG]);

  int finalDamage = 0;
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
    finalDamage = static_cast<int>(std::round(damage * protection));
  }
  return finalDamage;
}

QString Character::RegenIntoDamage(const int atkValue,
                                   const QString &statsName) {
  if (atkValue > 0) {
    return "atk value is not >0";
  }

  if (statsName != STATS_HP && statsName != STATS_MANA &&
      statsName != STATS_VIGOR && statsName != STATS_BERSECK) {
    return "Bad stats name";
  }

  QString channelLog;
  const auto pm = Application::GetInstance().m_GameManager->m_PlayersManager;
  const auto &effectList = pm->m_AllEffectsOnGame;
  std::vector<Character *> playerList;
  if (m_type == characType::Hero) {
    playerList = pm->m_HeroesList;
  } else if (m_type == characType::Boss) {
    playerList = pm->m_BossesList;
  }

  for (const auto &e : effectList.at(m_Name)) {
    if (e.allAtkEffects.effect == EFFECT_INTO_DAMAGE) {
      AttaqueType amountIntoDamageAtk;
      amountIntoDamageAtk.damage =
          atkValue * e.allAtkEffects.subValueEffect / 100;
      for (auto *pl : playerList) {
        const auto finalDamage = DamageByAtk(pl, amountIntoDamageAtk);
        auto &hp =
            std::get<StatsType<int>>(pl->m_Stats.m_AllStatsTable[STATS_HP]);
        hp.m_CurrentValue = max(0, hp.m_CurrentValue - finalDamage);
        channelLog = PlayersManager::FormatAtkOnEnnemy(finalDamage);
      }
    }
  }
  return channelLog;
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

  auto &tarCurHp = targetHp.m_CurrentValue;
  if (atk.damage > 0) {
    const auto finalDamage = DamageByAtk(target, atk);
    tarCurHp = max(0, tarCurHp - finalDamage);
    channelLog += PlayersManager::FormatAtkOnEnnemy(finalDamage);
  }

  if (atk.heal > 0) {
    tarCurHp = min(targetHp.m_MaxValue, static_cast<int>(tarCurHp + atk.heal));
    channelLog += PlayersManager::FormatAtkOnAlly(atk.heal);
    // Apply effect transform heal into damage on all bosses
    channelLog += RegenIntoDamage(atk.heal, STATS_HP);
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

  // Cost
  // mana cost in %
  launcherMana.m_CurrentValue =
      max(0, static_cast<int>(launcherMana.m_CurrentValue -
                              atk.manaCost * launcherMana.m_MaxValue / 100));
  launcherVigor.m_CurrentValue =
      max(0, static_cast<int>(launcherVigor.m_CurrentValue -
                              atk.vigorCost * launcherVigor.m_MaxValue / 100));
  launcherBerseck.m_CurrentValue = max(
      0, static_cast<int>(launcherBerseck.m_CurrentValue -
                          atk.berseckCost * launcherBerseck.m_MaxValue / 100));
  // Gain or regen
  // TODO in % ?
  launcherAggro.m_CurrentValue += atk.aggroRate;
  launcherBerseck.m_CurrentValue += atk.berseckRate;
  launcherMana.m_CurrentValue += atk.regenMana;
  launcherVigor.m_CurrentValue += atk.regenVigor;
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

      if (atk.name == "Eveil de la forêt") {
        std::vector<effectParam> epTable = CreateEveilDeLaForet();
        for (const auto &ep : epTable) {
          atk.m_AllEffects.push_back(ep);
        }
      }
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
                                const StatsType<T> &equipStat) const {
  if (equipStat.m_CurrentValue == 0) {
    return;
  }
  const double ratio = static_cast<double>(charStat.m_CurrentValue) /
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
  const double ratio = static_cast<double>(charStat.m_CurrentValue) /
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
  // Cooldown ?
  for (const auto &e : atk.m_AllEffects) {
    // TODO rewrite the cooldown thing for an atk
    if (e.effect == EFFECT_NB_COOL_DOWN) {
      const auto &pm =
          Application::GetInstance().m_GameManager->m_PlayersManager;
      for (const auto &gae : pm->m_AllEffectsOnGame[m_Name]) {
        if (gae.allAtkEffects.effect == e.effect &&
            gae.allAtkEffects.subValueEffect > 0) {
          return false;
        }
      }
    }
  }
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

QString Character::ApplyOneEffect(Character *target, effectParam &effect,
                                  const bool fromLaunch) {
  if (target == nullptr) {
    return "No  target character";
  }
  auto &pm = Application::GetInstance().m_GameManager->m_PlayersManager;
  const auto &powMag =
      std::get<StatsType<double>>(m_Stats.m_AllStatsTable[STATS_POW_MAG]);
  QString result;

  // increment counter turn, effect is used
  effect.counterTurn++;
  if (effect.effect == EFFECT_NB_DECREASE_BY_TURN) {
    const int intMin = 0;
    const int intMax = 100;
    const int stepLimit = (intMax / effect.nbTurns); // get percentual
    const auto maxLimit = stepLimit * (effect.nbTurns - effect.counterTurn);
    if (const auto randNb = Utils::GetRandomNb(intMin, intMax);
        !(randNb >= 0 && randNb < maxLimit)) {
      return QString("%1 n'a pas d'effet").arg(EFFECT_NB_DECREASE_BY_TURN);
    }
  }
  int nbOfApplies = 1; // default value 1 for the nominal case

  if (fromLaunch) {
    if (effect.effect == EFFECT_NB_DECREASE_ON_TURN) {
      int counter = effect.subValueEffect;
      while (counter > 0) {
        const int intMin = 0;
        const int intMax = 100;
        const int stepLimit =
            (intMax / effect.subValueEffect); // get percentual
        const auto maxLimit = stepLimit * counter;
        if (const auto randNb = Utils::GetRandomNb(intMin, intMax);
            randNb >= 0 && randNb < maxLimit) {
          nbOfApplies++;
        } else {
          break;
        }
        counter--;
      }
    }
    if (effect.effect == EFFECT_REINIT) {
      pm->ResetCounterOnOneStatsEffect(this, effect.statsName);
      nbOfApplies = 0;
    }
    if (effect.effect == EFFECT_DELETE_BAD) {
      if (effect.subValueEffect <= 1) {
        pm->DeleteOneBadEffect(this);
      } else {
        pm->DeleteAllBadEffect(this);
      }
    }
    if (effect.effect == EFFECT_IMPROVE_HOTS) {
      pm->ImproveHotsOnPlayers(effect.subValueEffect, target->m_type);
    }
    if (effect.effect == EFFECT_BOOSTED_BY_HOTS) {
      const auto nbHots = pm->GetNbOfStatsInEffectList(target, STATS_HP);
      effect.value = effect.value * (effect.subValueEffect / 100) * nbHots;
    }
  }
  // apply the effect
  // the nomical case is 1 for any atk. effect.subValueEffect is on top of the nominal atk
  const int potentialAttempts = max(1, effect.subValueEffect + 1);
  auto finalValue = effect.value;
  // for stats hp, the value depends of the mag power in case of heal
  // value = effect_value + mag_pow/nb_of_turns
  if (effect.statsName == STATS_HP) {
    auto &localStat =
        std::get<StatsType<int>>(target->m_Stats.m_AllStatsTable[STATS_HP]);

    // for the QString result
    const int delta = localStat.m_MaxValue - localStat.m_CurrentValue;
    const auto amount =
        nbOfApplies *
        (effect.value +
         static_cast<int>(powMag.m_CurrentValue) /
                          effect.nbTurns);
    // new value of stat
    localStat.m_CurrentValue += min(delta, amount);
    QString effectName;
    if (effect.effect.isEmpty() || !fromLaunch) {
      effectName = effect.statsName;
    } else {
        // for example EFFECT_NB_DECREASE_ON_TURN is only from launch
      effectName = effect.statsName + "-" + effect.effect;
    }
    if(fromLaunch){
    result =
        QString("Sur %1: Récupère %5 PV grâce à l'effet %2 (appliqué %3/%4 "
                "possible(s)).")
            .arg(target->m_Name)
            .arg(effectName)
            .arg(nbOfApplies)
            .arg(QString::number(potentialAttempts))
                     .arg(QString::number(amount));
    } else{
        result =
            QString("Sur %1: Récupère %3 PV grâce à l'effet %2.")
                .arg(target->m_Name)
                .arg(effectName)
                .arg(QString::number(amount));
    }
  } else if (effect.statsName != STATS_HP) {
    if (effect.statsName == STATS_DODGE) { // value in %
      auto &localStat = std::get<StatsType<double>>(
          target->m_Stats.m_AllStatsTable[effect.statsName]);
      localStat.m_CurrentValue = static_cast<int>(
          std::round(localStat.m_CurrentValue +
                     nbOfApplies * localStat.m_MaxValue * effect.value / 100));
    } else if (effect.statsName == STATS_POW_MAG) {
      // for 'double' stats
      auto &localStat = std::get<StatsType<double>>(
          target->m_Stats.m_AllStatsTable[effect.statsName]);
      localStat.m_CurrentValue =
          min(localStat.m_MaxValue,
              nbOfApplies * localStat.m_CurrentValue + effect.value);
    } else {
      // for 'int' stats
      auto &localStat = std::get<StatsType<int>>(
          target->m_Stats.m_AllStatsTable[effect.statsName]);
      localStat.m_CurrentValue =
          min(localStat.m_MaxValue,
              nbOfApplies * localStat.m_CurrentValue + effect.value);
    }
    result = QString("Sur %1: l'effet %2-%3 s'applique %4/%5 "
                     "possible(s) avec un max de %6.")
                 .arg(target->m_Name)
                 .arg(effect.statsName)
                 .arg(effect.effect)
                 .arg(nbOfApplies)
                 .arg(QString::number(potentialAttempts))
                 .arg(QString::number(nbOfApplies * finalValue));
  }

  // Apply regen effect turning into damage for all bosses
  if (effect.statsName == STATS_HP || effect.statsName == STATS_MANA ||
      effect.statsName == STATS_VIGOR || effect.statsName == STATS_BERSECK) {
    for (const auto &[playerName, allGae] : pm->m_AllEffectsOnGame) {
      for (const auto &e : allGae) {
        if (e.allAtkEffects.effect == EFFECT_INTO_DAMAGE) {
          // TODO handle return QString
          RegenIntoDamage(effect.value, effect.statsName);
          break;
        }
      }
    }
  }

  return result;
}

// Apply effect after launch of atk
// increment counter turn of effect
// Test targets
// Return tuple
// - 1 : bool: is attak applied ?
// - 2 : QStringList : log on each applied effect to display on channel
// - 3 : std::vector<effectParam> : list of all applied effectParam
std::tuple<bool, QStringList, std::vector<effectParam>>
Character::ApplyAtkEffect(const bool targetedOnMainAtk, const QString &atkName,
                          Character *target) {
  if (target == nullptr) {
    return std::make_tuple(false, QStringList("No target"),
                           std::vector<effectParam>());
  }
  bool applyAtk = true;
  // effect can be modified -> counter nb when applied
  const auto &allEffects = m_AttakList.at(atkName).m_AllEffects;
  std::vector<effectParam> allAppliedEffects;
  QStringList resultEffects;
  const bool isAlly = target->m_type == m_type;

  for (const auto &effect : allEffects) {

    if (!isAlly &&
        (effect.target == TARGET_ALLY || effect.target == TARGET_ALL_HEROES ||
         effect.target == TARGET_HIMSELF)) {
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

    // test if applicable effect
    const auto &pm = Application::GetInstance().m_GameManager->m_PlayersManager;
    if (effect.effect == EFFECT_REINIT &&
        !(pm->GetNbOfStatsInEffectList(this, effect.statsName) >=
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
    effectParam appliedEffect = effect;
    // appliedEffect is modified in ApplyOneEffect
    resultEffects.append(ApplyOneEffect(target, appliedEffect, true));
    allAppliedEffects.push_back(appliedEffect);
  }

  return std::make_tuple(applyAtk, resultEffects, allAppliedEffects);
}

void Character::RemoveMalusEffect(const QString &statsName) {

  for (const auto &stats : ALL_STATS) {
    if (stats == STATS_POW_MAG || stats == STATS_DODGE) {
      auto &localStat =
          std::get<StatsType<double>>(m_Stats.m_AllStatsTable.at(stats));
      localStat.m_CurrentValue = localStat.m_MaxValue;
      break;
    } else if(m_Stats.m_AllStatsTable.find(statsName) != m_Stats.m_AllStatsTable.end()) {
      auto &localStat =
          std::get<StatsType<int>>(m_Stats.m_AllStatsTable.at(stats));
      localStat.m_CurrentValue = localStat.m_MaxValue;
      break;
    }
  }
}

std::vector<effectParam> Character::CreateEveilDeLaForet() {
  std::vector<effectParam> epTable;

  effectParam param;
  param.effect = EFFECT_DELETE_BAD;
  param.value = 0;
  param.nbTurns = 1;
  param.reach = REACH_ZONE;
  param.statsName = "";
  param.target = TARGET_ALLY;
  param.subValueEffect = 1000;
  epTable.push_back(param);

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
