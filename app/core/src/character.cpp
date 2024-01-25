#include "character.h"

// decode json
#include <QDir>
#include <QFile>
#include <QJsonDocument>

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

void Character::Attaque(int degat, Character *target) const {
  if (target == nullptr) {
    return;
  }
  target->m_Stats.m_HP.m_Value -= degat;
}

void Character::AddAtq(const AttaqueType &atq) { attakList.push_back(atq); }

void Character::AddStuff(const Stuff &stuff) {
  m_Stats.m_HP.m_Value += stuff.m_Stats.m_HP.m_Value;
  m_Stats.m_Mana = stuff.m_Stats.m_Mana;
  m_Stats.m_Vigor = stuff.m_Stats.m_Vigor;
  m_Stats.m_Bersecker = stuff.m_Stats.m_Bersecker;
  m_Stats.m_ArmPhy = stuff.m_Stats.m_ArmPhy;
  m_Stats.m_ArmMag = stuff.m_Stats.m_ArmMag;
  m_Stats.m_powPhy = stuff.m_Stats.m_powPhy;
  m_Stats.m_powMag = stuff.m_Stats.m_powMag;
  m_Stats.m_aggro = stuff.m_Stats.m_aggro;
  m_Stats.m_speed = stuff.m_Stats.m_speed;
  m_Stats.m_CriticalStrike = stuff.m_Stats.m_CriticalStrike;
  m_Stats.m_dogde = stuff.m_Stats.m_dogde;
  m_Stats.m_regenHP = stuff.m_Stats.m_regenHP;
  m_Stats.m_regenMana = stuff.m_Stats.m_regenMana;
  m_Stats.m_regenVigueur = stuff.m_Stats.m_regenVigueur;
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
      QString msg = out.readAll();
      atkJson.close();

      auto json = QJsonDocument::fromJson(msg.toUtf8());
      // decode json
      AttaqueType atk;
      atk.name = json[ATK_NAME].toString();
      atk.namePhoto = json[ATK_PHOTO].toString();
      atk.aggroCum = json[ATK_BERSECK_AGGRO].toInt();
      atk.damage = json[ATK_DAMAGE].toInt();
      atk.heal = json[ATK_HEAL].toInt();
      atk.regenMana = json[ATK_REGEN_MANA].toInt();
      atk.effect = json[ATK_EFFECT].toString();
      atk.manaCost = json[ATK_MANA_COST].toInt();
      atk.vigorCost = json[ATK_VIGOR_COST].toInt();
      atk.reach = json[ATK_REACH].toString();
      atk.target = json[ATK_TARGET].toString();
      atk.turnsDuration = static_cast<uint16_t>(json[ATK_DURATION].toInt());
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

void Character::ApplyAllEquipment(
    const std::unordered_map<QString, Stuff> &allEquipMap) {
  for (const auto &[body, equipName] : m_WearingEquipment) {
    if (equipName.isEmpty()) {
      continue;
    }
    const auto &equip = allEquipMap.at(equipName);
    m_Stats.m_HP.m_Value += equip.m_Stats.m_HP.m_Value;
    m_Stats.m_Mana.m_Value += equip.m_Stats.m_Mana.m_Value;
    m_Stats.m_Vigor.m_Value += equip.m_Stats.m_Vigor.m_Value;
    m_Stats.m_Bersecker.m_Value += equip.m_Stats.m_Bersecker.m_Value;
    m_Stats.m_ArmPhy.m_Value += equip.m_Stats.m_ArmPhy.m_Value;
    m_Stats.m_ArmMag.m_Value += equip.m_Stats.m_ArmMag.m_Value;
    m_Stats.m_powPhy.m_Value += equip.m_Stats.m_powPhy.m_Value;
    m_Stats.m_powMag.m_Value += equip.m_Stats.m_powMag.m_Value;
    m_Stats.m_aggro.m_Value += equip.m_Stats.m_aggro.m_Value;
    m_Stats.m_speed.m_Value += equip.m_Stats.m_speed.m_Value;
    m_Stats.m_CriticalStrike.m_Value += equip.m_Stats.m_CriticalStrike.m_Value;
    m_Stats.m_dogde.m_Value += equip.m_Stats.m_dogde.m_Value;
    m_Stats.m_regenHP.m_Value += equip.m_Stats.m_regenHP.m_Value;
    m_Stats.m_regenMana.m_Value += equip.m_Stats.m_regenMana.m_Value;
    m_Stats.m_regenVigueur.m_Value += equip.m_Stats.m_regenVigueur.m_Value;
  }
}
