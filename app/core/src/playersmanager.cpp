#include "playersmanager.h"

#include "Application.h"

// decode json
#include <QDir>
#include <QFile>
#include <QJsonDocument>

void PlayersManager::InitHeroes() {

  Stats stats;
  stats.m_HP.m_Value = 605;
  stats.m_Mana.m_Value = 369;
  stats.m_Vigor.m_Value = 126;
  stats.m_Bersecker.m_Value = 0;
  stats.m_ArmPhy.m_Value = 108;
  stats.m_ArmMag.m_Value = 35;
  stats.m_powPhy.m_Value = 10;
  stats.m_powMag.m_Value = 137.5;
  stats.m_aggro.m_Value = 0;
  stats.m_speed.m_Value = 25;
  stats.m_CriticalStrike.m_Value = 20;
  stats.m_dogde.m_Value = 10;
  stats.m_regenHP.m_Value = 14;
  stats.m_regenMana.m_Value = 40.2;
  stats.m_regenVigueur.m_Value = 5;
  const auto hero1 = new Character("Thalia", characType::Hero, stats);

  stats.m_HP.m_Value = 600;
  stats.m_Mana.m_Value = 141;
  stats.m_Vigor.m_Value = 445;
  stats.m_Bersecker.m_Value = 0;
  stats.m_ArmPhy.m_Value = 60;
  stats.m_ArmMag.m_Value = 47;
  stats.m_powPhy.m_Value = 115;
  stats.m_powMag.m_Value = 15;
  stats.m_aggro.m_Value = 0;
  stats.m_speed.m_Value = 20;
  stats.m_CriticalStrike.m_Value = 20;
  stats.m_dogde.m_Value = 9.5;
  stats.m_regenHP.m_Value = 10;
  stats.m_regenMana.m_Value = 12;
  stats.m_regenVigueur.m_Value = 10;
  const auto hero2 = new Character("Azrak Ombresang", characType::Hero, stats);

  stats.m_HP.m_Value = 828;
  stats.m_Mana.m_Value = 0;
  stats.m_Vigor.m_Value = 0;
  stats.m_Bersecker.m_Value = 100;
  stats.m_ArmPhy.m_Value = 85;
  stats.m_ArmMag.m_Value = 45;
  stats.m_powPhy.m_Value = 30;
  stats.m_powMag.m_Value = 0;
  stats.m_aggro.m_Value = 0;
  stats.m_speed.m_Value = 14;
  stats.m_CriticalStrike.m_Value = 15;
  stats.m_dogde.m_Value = 5;
  stats.m_regenHP.m_Value = 0;
  stats.m_regenMana.m_Value = 0;
  stats.m_regenVigueur.m_Value = 0;
  const auto hero3 = new Character("Thraïn", characType::Hero, stats);
  hero3->m_CurrentStats.m_Bersecker.m_Value = 20;

  m_HeroesList.push_back(hero1);
  m_HeroesList.push_back(hero2);
  m_HeroesList.push_back(hero3);

  for (const auto &hero : m_HeroesList) {
    hero->LoadAtkJson();
    hero->LoadStuffJson();
    hero->ApplyAllEquipment(m_Equipments);
  }
}

void PlayersManager::InitBosses() {
  Stats stats;
  stats.m_HP.m_Value = 9999;
  stats.m_Mana.m_Value = 0;
  stats.m_Vigor.m_Value = 126;
  stats.m_Bersecker.m_Value = 0;
  stats.m_ArmPhy.m_Value = 2280;
  stats.m_ArmMag.m_Value = 35;
  stats.m_powPhy.m_Value = 10;
  stats.m_powMag.m_Value = 138;
  stats.m_aggro.m_Value = 0;
  stats.m_speed.m_Value = 25;
  stats.m_CriticalStrike.m_Value = 20;
  stats.m_dogde.m_Value = 10;
  stats.m_regenHP.m_Value = 4;
  stats.m_regenMana.m_Value = 25;
  stats.m_regenVigueur.m_Value = 5;
  const auto boss = new Character("Pignouf hehe", characType::Boss, stats);

  m_BossesList.push_back(boss);
}

void PlayersManager::LoadAllEquipmentsJson() {
  // List all equipment
  QString directoryPath =
      OFFLINE_ROOT_EQUIPMENT; // Replace with the actual path
  QDir directory(directoryPath);
  if (!directory.exists()) {
    qDebug() << "Directory does not exist: " << directoryPath;
  }
  // Detect all directories, each directory is a part of the body
  QStringList subdirectories =
      directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

  for (const auto &subdirPath : subdirectories) {
    QDir subdir(directoryPath +subdirPath);
    if (!subdir.exists()) {
      qDebug() << "Directory does not exist: " << subdirPath;
    }
    QStringList fileList = subdir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    for (const QString &file : fileList) {
      QFile json(directoryPath + subdirPath + "/" + file);
      if (!json.open(QFile::ReadOnly | QFile::Text)) {
        Application::GetInstance().log(" Could not open the file for reading " +
                                       directoryPath + "/" + file);
        return;
      } else {
        // Convert json file to QString
        QTextStream out(&json);
        QString msg = out.readAll();
        json.close();
        const auto jsonDoc = QJsonDocument::fromJson(msg.toUtf8());
        // decode json
        Stuff stuff;
        stuff.m_Name = jsonDoc[EQUIP_NAME].toString();
        // Fill stuff stats
        stuff.m_Stats.m_HP.m_Value = jsonDoc[STATS_HP].toInt();
        stuff.m_Stats.m_Mana.m_Value = jsonDoc[STATS_MANA].toInt();
        stuff.m_Stats.m_Vigor.m_Value = jsonDoc[STATS_VIGOR].toInt();
        stuff.m_Stats.m_Bersecker.m_Value = jsonDoc[STATS_BERSECK].toInt();
        stuff.m_Stats.m_ArmPhy.m_Value = jsonDoc[STATS_ARM_PHY].toInt();
        stuff.m_Stats.m_ArmMag.m_Value = jsonDoc[STATS_ARM_MAG].toInt();
        stuff.m_Stats.m_powPhy.m_Value = jsonDoc[STATS_POW_PHY].toInt();
        stuff.m_Stats.m_powMag.m_Value = jsonDoc[STATS_POW_MAG].toInt();
        stuff.m_Stats.m_aggro.m_Value = jsonDoc[STATS_AGGRO].toInt();
        stuff.m_Stats.m_speed.m_Value = jsonDoc[STATS_SPEED].toInt();
        stuff.m_Stats.m_CriticalStrike.m_Value = jsonDoc[STATS_CRIT].toInt();
        stuff.m_Stats.m_dogde.m_Value = jsonDoc[STATS_DODGE].toInt();
        stuff.m_Stats.m_regenHP.m_Value = jsonDoc[STATS_REGEN_HP].toInt();
        stuff.m_Stats.m_regenMana.m_Value = jsonDoc[STATS_REGEN_MANA].toInt();
        stuff.m_Stats.m_regenVigueur.m_Value = jsonDoc[STATS_REGEN_VIGOR].toInt();
        // Add atk to hero atk list

        m_Equipments[stuff.m_Name] = stuff;
      }
    }
  }
}

Character* PlayersManager::GetCharacterByName(const QString& name){
    for(auto* boss : m_BossesList){
        if(name == boss->m_Name){
            return boss;
        }
    }
    for(auto* hero : m_HeroesList){
        if(name == hero->m_Name){
            return hero;
        }
    }

    return nullptr;
}
