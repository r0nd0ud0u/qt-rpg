#include "playersmanager.h"

#include "Application.h"

// decode json
#include <QDir>
#include <QFile>
#include <QJsonDocument>

void PlayersManager::InitHeroes() {

  Stats stats;
  stats.m_HP.SetValues(605, 605, 605);
  stats.m_Mana.SetValues(369, 369, 369);
  stats.m_Vigor.SetValues(126, 126, 126);
  stats.m_Berseck.SetValues(0, 0, 0);
  stats.m_BerseckRate.SetValues(0, 0, 0);
  stats.m_ArmPhy.SetValues(108, 108, 108);
  stats.m_ArmMag.SetValues(35, 35, 35);
  stats.m_PowPhy.SetValues(10, 10, 10);
  stats.m_PowMag.SetValues(137.5, 137.5, 137.5);
  stats.m_Aggro.SetValues(0, 0, 0);
  stats.m_Speed.SetValues(25, 25, 25);
  stats.m_CriticalStrike.SetValues(20, 20, 20);
  stats.m_Dogde.SetValues(10, 10, 10);
  stats.m_RegenHP.SetValues(14, 14, 14);
  stats.m_RegenMana.SetValues(40.2, 40.2, 40.2);
  stats.m_RegenVigor.SetValues(5, 5, 5);
  stats.m_AggroRate.SetValues(1, 1, 1);
  const auto hero1 = new Character("Thalia", characType::Hero, stats);

  stats.m_HP.SetValues(600, 600, 600);
  stats.m_Mana.SetValues(141, 141, 141);
  stats.m_Vigor.SetValues(445, 445, 445);
  stats.m_Berseck.SetValues(0, 0, 0);
  stats.m_BerseckRate.SetValues(0, 0, 0);
  stats.m_ArmPhy.SetValues(60, 60, 60);
  stats.m_ArmMag.SetValues(47, 47, 47);
  stats.m_PowPhy.SetValues(115, 115, 115);
  stats.m_PowMag.SetValues(15, 15, 15);
  stats.m_Aggro.SetValues(0, 0, 0);
  stats.m_Speed.SetValues(20, 20, 20);
  stats.m_CriticalStrike.SetValues(20, 20, 20);
  stats.m_Dogde.SetValues(9.5, 9.5, 9.5);
  stats.m_RegenHP.SetValues(10, 10, 10);
  stats.m_RegenMana.SetValues(12, 12, 12);
  stats.m_RegenVigor.SetValues(10, 10, 10);
  stats.m_AggroRate.SetValues(1, 1, 1);
  const auto hero2 = new Character("Azrak Ombresang", characType::Hero, stats);

  stats.m_HP.SetValues(828, 828, 828);
  stats.m_Mana.SetValues(0, 0, 0);
  stats.m_Vigor.SetValues(0, 0, 0);
  stats.m_Berseck.SetValues(20, 20, 100);
  stats.m_BerseckRate.SetValues(5, 5, 5);
  stats.m_ArmPhy.SetValues(85, 85, 85);
  stats.m_ArmMag.SetValues(45, 45, 45);
  stats.m_PowPhy.SetValues(30, 30, 30);
  stats.m_PowMag.SetValues(0, 0, 0);
  stats.m_Aggro.SetValues(0, 0, 0);
  stats.m_Speed.SetValues(14, 14, 14);
  stats.m_CriticalStrike.SetValues(15, 15, 15);
  stats.m_Dogde.SetValues(5, 5, 5);
  stats.m_RegenHP.SetValues(0, 0, 0);
  stats.m_RegenMana.SetValues(0, 0, 0);
  stats.m_RegenVigor.SetValues(0, 0, 0);
  stats.m_AggroRate.SetValues(3, 3, 3);
  const auto hero3 = new Character("Thraïn", characType::Hero, stats);

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
  stats.m_HP.SetValues(9999, 9999, 9999);
  stats.m_Mana.SetValues(0, 0, 0);
  stats.m_Vigor.SetValues(126, 126, 126);
  stats.m_Berseck.SetValues(0, 0, 0);
  stats.m_BerseckRate.SetValues(0, 0, 0);
  stats.m_ArmPhy.SetValues(2280, 2280, 2280);
  stats.m_ArmMag.SetValues(35, 35, 35);
  stats.m_PowPhy.SetValues(10, 10, 10);
  stats.m_PowMag.SetValues(138, 138, 138);
  stats.m_Aggro.SetValues(0, 0, 0);
  stats.m_Speed.SetValues(25, 25, 25);
  stats.m_CriticalStrike.SetValues(20, 20, 20);
  stats.m_Dogde.SetValues(10, 10, 10);
  stats.m_RegenHP.SetValues(4, 4, 4);
  stats.m_RegenMana.SetValues(25, 25, 25);
  stats.m_RegenVigor.SetValues(5, 5, 5);
  stats.m_AggroRate.SetValues(1, 1, 1);
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
    QDir subdir(directoryPath + subdirPath);
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
        stuff.m_Stats.m_HP.m_CurrentValue = jsonDoc[STATS_HP].toInt();
        stuff.m_Stats.m_Mana.m_CurrentValue = jsonDoc[STATS_MANA].toInt();
        stuff.m_Stats.m_Vigor.m_CurrentValue = jsonDoc[STATS_VIGOR].toInt();
        stuff.m_Stats.m_Berseck.m_CurrentValue = jsonDoc[STATS_BERSECK].toInt();
        stuff.m_Stats.m_ArmPhy.m_CurrentValue = jsonDoc[STATS_ARM_PHY].toInt();
        stuff.m_Stats.m_ArmMag.m_CurrentValue = jsonDoc[STATS_ARM_MAG].toInt();
        stuff.m_Stats.m_PowPhy.m_CurrentValue = jsonDoc[STATS_POW_PHY].toInt();
        stuff.m_Stats.m_PowMag.m_CurrentValue = jsonDoc[STATS_POW_MAG].toInt();
        stuff.m_Stats.m_Aggro.m_CurrentValue = jsonDoc[STATS_AGGRO].toInt();
        stuff.m_Stats.m_Speed.m_CurrentValue = jsonDoc[STATS_SPEED].toInt();
        stuff.m_Stats.m_CriticalStrike.m_CurrentValue =
            jsonDoc[STATS_CRIT].toInt();
        stuff.m_Stats.m_Dogde.m_CurrentValue = jsonDoc[STATS_DODGE].toInt();
        stuff.m_Stats.m_RegenHP.m_CurrentValue =
            jsonDoc[STATS_REGEN_HP].toInt();
        stuff.m_Stats.m_RegenMana.m_CurrentValue =
            jsonDoc[STATS_REGEN_MANA].toInt();
        stuff.m_Stats.m_RegenVigor.m_CurrentValue =
            jsonDoc[STATS_REGEN_VIGOR].toInt();
        // Add atk to hero atk list

        m_Equipments[stuff.m_Name] = stuff;
      }
    }
  }
}

Character *PlayersManager::GetCharacterByName(const QString &name) {
  for (auto *boss : m_BossesList) {
    if (name == boss->m_Name) {
      return boss;
    }
  }
  for (auto *hero : m_HeroesList) {
    if (name == hero->m_Name) {
      return hero;
    }
  }

  return nullptr;
}

void PlayersManager::UpdatePartnersOnAtk(const Character* curPlayer, const QString &atkName) const{
    std::vector<Character*> playerList;

    if (curPlayer->m_type == characType::Hero){
        playerList = m_HeroesList;
    } else     if (curPlayer->m_type == characType::Boss){
        playerList = m_BossesList;
    }
    const auto& atk = curPlayer->m_AttakList.at(atkName);
    for (const auto &other : playerList) {
        if (other->m_Name == curPlayer->m_Name) {
            continue;
        }
        if (atk.target == TARGET_ALLY && atk.reach == REACH_ZONE) {
            // Regen
            other->m_Stats.m_Mana.m_CurrentValue +=  static_cast<int>(std::round(other->m_Stats.m_RegenMana.m_CurrentValue));
            // apply effect
            other->ApplyAtkEffect(atk.m_AllEffects);
        }
    }
}

QString PlayersManager::FormatAtkOnEnnemy(const QString player1, const QString player2,  const QString &atkName, const int damage){
    return QString("%1 utilise %2 sur %3 et fait %4 de dégâts!").arg(player1).arg(atkName).arg(player2).arg(damage);
}

QString PlayersManager::FormatAtkOnAlly(const QString player1, const QString player2,  const QString &atkName, const int damage){
    return QString("%1 utilise %2 sur %3 et soigne de %4 PV!").arg(player1).arg(atkName).arg(player2).arg(damage);
}
