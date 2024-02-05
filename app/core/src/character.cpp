#include "character.h"

// decode json
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QtDebug>

#include "Application.h"

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
  auto &tarCurHp = target->m_Stats.m_HP.m_CurrentValue;
  if (atk.damage > 0) {
    int arm = 0;
    int damage = atk.damage;
    if (atk.manaCost > 0) {
      damage += static_cast<int>(std::round(m_Stats.m_PowMag.m_CurrentValue));
      arm = target->m_Stats.m_ArmMag.m_CurrentValue;
    } else if (atk.vigorCost > 0 || atk.berseckCost > 0) {
      damage += m_Stats.m_PowPhy.m_CurrentValue;
      arm = target->m_Stats.m_ArmPhy.m_CurrentValue;
    }
    const double protection = 1000.0 / (1000.0 + static_cast<double>(arm));
    const auto finalDamage = static_cast<int>(std::round(damage * protection));

    tarCurHp = max(0, tarCurHp - finalDamage);
    channelLog = PlayersManager::FormatAtkOnEnnemy(m_Name, target->m_Name,atkName, finalDamage);
  }
  if (atk.heal > 0) {
    tarCurHp = min(target->m_Stats.m_HP.m_CurrentValue,
                   static_cast<int>(tarCurHp + atk.heal));
      channelLog = PlayersManager::FormatAtkOnAlly(m_Name, target->m_Name,atkName, atk.heal);
  }
  return channelLog;
}

void Character::UpdateStatsOnAtk(const QString &atkName) {
  if (atkName.isEmpty()) {
    return;
  }
  const auto &atk = m_AttakList.at(atkName);

  // Stats change on target
  // Cost
  m_Stats.m_Mana.m_CurrentValue =
      max(0, static_cast<int>(m_Stats.m_Mana.m_CurrentValue - atk.manaCost));
  m_Stats.m_Vigor.m_CurrentValue =
      max(0, static_cast<int>(m_Stats.m_Vigor.m_CurrentValue - atk.vigorCost));
  m_Stats.m_Berseck.m_CurrentValue = max(
      0, static_cast<int>(m_Stats.m_Berseck.m_CurrentValue - atk.berseckCost));
  // Gain
  m_Stats.m_Aggro.m_CurrentValue += m_Stats.m_AggroRate.m_CurrentValue;
  m_Stats.m_Berseck.m_CurrentValue += m_Stats.m_BerseckRate.m_CurrentValue;
  // Regen
  m_Stats.m_Mana.m_CurrentValue += static_cast<int>(std::round(m_Stats.m_RegenMana.m_CurrentValue));
}

void Character::AddAtq(const AttaqueType &atq) { m_AttakList[atq.name] = atq; }

void Character::AddStuff(const Stuff &stuff) {
  m_Stats.m_HP.m_CurrentValue += stuff.m_Stats.m_HP.m_CurrentValue;
  m_Stats.m_Mana = stuff.m_Stats.m_Mana;
  m_Stats.m_Vigor = stuff.m_Stats.m_Vigor;
  m_Stats.m_Berseck = stuff.m_Stats.m_Berseck;
  m_Stats.m_ArmPhy = stuff.m_Stats.m_ArmPhy;
  m_Stats.m_ArmMag = stuff.m_Stats.m_ArmMag;
  m_Stats.m_PowPhy = stuff.m_Stats.m_PowPhy;
  m_Stats.m_PowMag = stuff.m_Stats.m_PowMag;
  m_Stats.m_Aggro = stuff.m_Stats.m_Aggro;
  m_Stats.m_Speed = stuff.m_Stats.m_Speed;
  m_Stats.m_CriticalStrike = stuff.m_Stats.m_CriticalStrike;
  m_Stats.m_Dogde = stuff.m_Stats.m_Dogde;
  m_Stats.m_RegenHP = stuff.m_Stats.m_RegenHP;
  m_Stats.m_RegenMana = stuff.m_Stats.m_RegenMana;
  m_Stats.m_RegenVigor = stuff.m_Stats.m_RegenVigor;
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
      for(const auto& effect : effectArray) {
          const auto& stat = effect[EFFECT_STAT].toString();
          if(stat.isEmpty()){
              break;
          }
          effectParam param;
          param.effect = effect[EFFECT_TYPE].toString();
          param.value = effect[EFFECT_VALUE].toInt();
          param.nbTurns = effect[EFFECT_ACTIVE_TURNS].toInt();
          param.reach = effect[EFFECT_REACH].toString();
          param.statsName = effect[EFFECT_STAT].toString();
          param.target = effect[EFFECT_TARGET].toString();
          atk.m_AllEffects.push_back(param);
      }
#else
      for(const auto& effect : effectArray) {
          effectParam param;
          if(effect.isObject()){
              const QJsonObject item = effect.toObject();
              for(const auto& key : item.keys()){
                  const auto& val = item[key];
                  if (val.isString()){
                      param.effect = val.toString();
                  }
                  else if (val.isDouble())
                      param.value = static_cast<int>(val.toDouble());
              }
          }

          if(param.effect.isEmpty()){
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
    ProcessAddEquip(m_Stats.m_HP, equip.m_Stats.m_HP);
    ProcessAddEquip(m_Stats.m_Mana, equip.m_Stats.m_Mana);
    ProcessAddEquip(m_Stats.m_Vigor, equip.m_Stats.m_Vigor);
    ProcessAddEquip(m_Stats.m_Berseck, equip.m_Stats.m_Berseck);
    ProcessAddEquip(m_Stats.m_ArmPhy, equip.m_Stats.m_ArmPhy);
    ProcessAddEquip(m_Stats.m_ArmMag, equip.m_Stats.m_ArmMag);
    ProcessAddEquip(m_Stats.m_PowPhy, equip.m_Stats.m_PowPhy);
    ProcessAddEquip(m_Stats.m_PowMag, equip.m_Stats.m_PowMag);
    ProcessAddEquip(m_Stats.m_Aggro, equip.m_Stats.m_Aggro);
    ProcessAddEquip(m_Stats.m_Speed, equip.m_Stats.m_Speed);
    ProcessAddEquip(m_Stats.m_CriticalStrike, equip.m_Stats.m_CriticalStrike);
    ProcessAddEquip(m_Stats.m_Dogde, equip.m_Stats.m_Dogde);
    ProcessAddEquip(m_Stats.m_RegenHP, equip.m_Stats.m_RegenHP);
    ProcessAddEquip(m_Stats.m_RegenMana, equip.m_Stats.m_RegenMana);
    ProcessAddEquip(m_Stats.m_RegenVigor, equip.m_Stats.m_RegenVigor);
  }
}

template<class T>
void Character::ProcessAddEquip(StatsType<T>& charStat, const StatsType<T>& equipStat){
    if(equipStat.m_CurrentValue == 0){
        return;
    }
    double ratio = static_cast<double>(charStat.m_CurrentValue)/static_cast<double>(charStat.m_MaxValue);
    charStat.m_MaxValue += equipStat.m_CurrentValue; // Currently only current value is filled on equip stat

    charStat.m_CurrentValue = static_cast<T>(std::round(charStat.m_MaxValue*ratio));
}

template<class T>
void Character::ProcessRemoveEquip(StatsType<T>& charStat, const StatsType<T>& equipStat){
    if(equipStat.m_CurrentValue == 0){
        return;
    }
    double ratio = static_cast<double>(charStat.m_CurrentValue)/static_cast<double>(charStat.m_MaxValue);
    charStat.m_MaxValue -= equipStat.m_CurrentValue; // Currently only current value is filled on equip stat

    charStat.m_CurrentValue = static_cast<T>(std::round(charStat.m_MaxValue*ratio));
}

/////////////////////////////////////////
/// \brief Character::CanBeLaunched
/// The attak can be launched if the character has enough mana, vigor and
/// berseck.
///
bool Character::CanBeLaunched(const AttaqueType &atk) const {
  const auto remainingMana =
      static_cast<uint32_t>(m_Stats.m_Mana.m_CurrentValue);
  const auto remainingBerseck =
      static_cast<uint32_t>(m_Stats.m_Berseck.m_CurrentValue);
  const auto remainingVigor =
      static_cast<uint32_t>(m_Stats.m_Vigor.m_CurrentValue);
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

void Character::ApplyOneEffect(Character *&target, const effectParam &effect) {
    if (effect.statsName == STATS_HP) {
        target->m_Stats.m_HP.m_CurrentValue =
            min(target->m_Stats.m_HP.m_MaxValue,
                target->m_Stats.m_HP.m_CurrentValue + effect.value);
    }
    if (effect.statsName == STATS_MANA) {
        // Regen
        target->m_Stats.m_Mana.m_CurrentValue =
            min(target->m_Stats.m_Mana.m_MaxValue,
                target->m_Stats.m_Mana.m_CurrentValue + effect.value);
    }
}
void Character::ApplyAtkEffect(const bool targetedOnMainAtk,
                               const QString &atkName, Character *target) {
    if (target == nullptr) {
        return;
    }

    const auto &allEffects = m_AttakList.at(atkName).m_AllEffects;

    for (const auto &effect : allEffects) {

        // is targeted ?
        if (effect.target == TARGET_ALLY && effect.reach == REACH_INDIVIDUAL &&
            !targetedOnMainAtk) {
            continue;
        }
        if (effect.target == TARGET_ENNEMY && effect.reach == REACH_INDIVIDUAL &&
            !targetedOnMainAtk) {
            continue;
        }

        ApplyOneEffect(target, effect);
    }
}
