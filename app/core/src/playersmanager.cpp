#include "playersmanager.h"

#include "Application.h"

// decode json
#include <QDir>
#include <QFile>
#include <QJsonDocument>

void PlayersManager::InitHeroes() {

  Stats stats;
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_HP])
      .SetValues(605, 605, 605);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_MANA])
      .SetValues(369, 369, 369);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_VIGOR])
      .SetValues(126, 126, 126);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_BERSECK])
      .SetValues(0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_BERSECK])
      .SetValues(0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_PHY])
      .SetValues(108, 108, 108);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_MAG])
      .SetValues(35, 35, 35);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_PHY])
      .SetValues(10, 10, 10);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_MAG])
      .SetValues(137, 137, 137);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_AGGRO])
      .SetValues(0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_SPEED])
      .SetValues(25, 25, 25);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_CRIT])
      .SetValues(20, 20, 20);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_DODGE])
      .SetValues(10, 10, 10);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_HP])
      .SetValues(14, 14, 14);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_MANA])
      .SetValues(40, 40, 40);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_VIGOR])
      .SetValues(5, 5, 5);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_AGGRO])
      .SetValues(1, 1, 1);
  const auto hero1 = new Character("Thalia", characType::Hero, stats);

  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_HP])
      .SetValues(200, 200, 600);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_MANA])
      .SetValues(141, 141, 141);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_VIGOR])
      .SetValues(445, 445, 445);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_BERSECK])
      .SetValues(0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_BERSECK])
      .SetValues(0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_PHY])
      .SetValues(60, 60, 60);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_MAG])
      .SetValues(47, 47, 47);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_PHY])
      .SetValues(115, 115, 115);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_MAG])
      .SetValues(15, 15, 15);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_AGGRO])
      .SetValues(0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_SPEED])
      .SetValues(20, 20, 20);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_CRIT])
      .SetValues(20, 20, 20);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_DODGE])
      .SetValues(9, 9, 9);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_HP])
      .SetValues(10, 10, 10);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_MANA])
      .SetValues(12, 12, 12);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_VIGOR])
      .SetValues(10, 10, 10);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_AGGRO])
      .SetValues(1, 1, 1);
  const auto hero2 = new Character("Azrak Ombresang", characType::Hero, stats);

  stats.m_HP.SetValues(828, 828, 828);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_MANA])
      .SetValues(0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_MANA])
      .SetValues(0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_BERSECK])
      .SetValues(20, 20, 100);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_BERSECK])
      .SetValues(5, 5, 5);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_PHY])
      .SetValues(85, 85, 85);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_MAG])
      .SetValues(45, 45, 45);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_PHY])
      .SetValues(30, 30, 30);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_MAG])
      .SetValues(0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_AGGRO])
      .SetValues(0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_SPEED])
      .SetValues(14, 14, 14);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_CRIT])
      .SetValues(15, 15, 15);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_DODGE])
      .SetValues(5, 5, 5);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_HP])
      .SetValues(0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_MANA])
      .SetValues(0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_VIGOR])
      .SetValues(0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_AGGRO])
      .SetValues(3, 3, 3);
  const auto hero3 = new Character("Thraïn", characType::Hero, stats);

  // color
  hero1->color = QColor("green");
  hero2->color = QColor("orange");
  hero3->color = QColor("blue");

  m_HeroesList.push_back(hero1);
  m_HeroesList.push_back(hero2);
  m_HeroesList.push_back(hero3);

  for (const auto &hero : m_HeroesList) {
    hero->LoadAtkJson();
    hero->LoadStuffJson();
    hero->ApplyEquipOnStats(m_Equipments);
  }
}

void PlayersManager::InitBosses() {
  Stats stats;
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_HP])
      .SetValues(9999, 9999, 9999);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_MANA])
      .SetValues(0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_MANA])
      .SetValues(126, 126, 126);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_BERSECK])
      .SetValues(0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_BERSECK])
      .SetValues(0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_PHY])
      .SetValues(2280, 2280, 2280);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_MAG])
      .SetValues(35, 35, 35);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_PHY])
      .SetValues(10, 10, 10);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_MAG])
      .SetValues(138, 138, 138);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_AGGRO])
      .SetValues(0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_SPEED])
      .SetValues(25, 25, 25);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_CRIT])
      .SetValues(20, 20, 20);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_DODGE])
      .SetValues(10, 10, 10);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_HP])
      .SetValues(4, 4, 4);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_MANA])
      .SetValues(25, 25, 25);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_VIGOR])
      .SetValues(5, 5, 5);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_AGGRO])
      .SetValues(1, 1, 1);
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

void PlayersManager::AddGameEffectOnAtk(
    const QString& launcherName, const QString &atkName, const QString& targetName,
    const std::vector<effectParam> &effects) {
    for(const auto& e : effects){
        GameAtkEffects gae;
        gae.launcher = launcherName;
        gae.atkName = atkName;
        gae.allAtkEffects = e;
        m_AllEffectsOnGame[targetName].push_back(gae);
    }
}

QStringList PlayersManager::RemoveTerminatedEffects() {
  QStringList sl;
  for (auto &[playerName, gaeTable] : m_AllEffectsOnGame) {
    for (auto it = gaeTable.begin(); it != gaeTable.end(); it++) {
      if (it->allAtkEffects.counterTurn == it->allAtkEffects.nbTurns) {
        QString terminated("L'effet %1 sur %2 est terminé.");
        terminated =
            terminated.arg(it->allAtkEffects.statsName).arg(playerName);
        sl.push_back(terminated);
        // remove malus effect from player
        auto *player = GetCharacterByName(playerName);
        if (player != nullptr) {
          player->RemoveMalusEffect(it->allAtkEffects.statsName);
        }
      }
    }
    auto newEnd = std::remove_if(
        gaeTable.begin(), gaeTable.end(), [](const GameAtkEffects &element) {
          return element.allAtkEffects.nbTurns ==
                 element.allAtkEffects
                     .counterTurn; // remove elements where this is true
        });
    gaeTable.erase(newEnd, gaeTable.end());
  }
  return sl;
}

QStringList PlayersManager::ApplyEffects() {
  QStringList logs;
  for (auto &[targetName, effectsTable] : m_AllEffectsOnGame) {
    auto *targetPl = GetCharacterByName(targetName);
    if (targetPl != nullptr) {
      for (auto &gae : effectsTable) {
        auto *launcherPl = GetCharacterByName(gae.launcher);
        if (launcherPl != nullptr) {
          logs.append(
              launcherPl->ApplyOneEffect(targetPl, gae.allAtkEffects, false));
        }
      }
    }
  }
  return logs;
}

void PlayersManager::ApplyRegenStats() {
  for (const auto &hero : m_HeroesList) {

    auto &heroHp =
        std::get<StatsType<int>>(hero->m_Stats.m_AllStatsTable[STATS_HP]);
    const auto &heroRegenHp =
        std::get<StatsType<int>>(hero->m_Stats.m_AllStatsTable[STATS_REGEN_HP]);
    auto &heroMana =
        std::get<StatsType<int>>(hero->m_Stats.m_AllStatsTable[STATS_MANA]);
    const auto &heroRegenMana = std::get<StatsType<int>>(
        hero->m_Stats.m_AllStatsTable[STATS_REGEN_MANA]);
    auto &heroBerseck =
        std::get<StatsType<int>>(hero->m_Stats.m_AllStatsTable[STATS_BERSECK]);
    const auto &heroRegenBerseck = std::get<StatsType<int>>(
        hero->m_Stats.m_AllStatsTable[STATS_RATE_BERSECK]);
    auto &heroVigor =
        std::get<StatsType<int>>(hero->m_Stats.m_AllStatsTable[STATS_VIGOR]);
    const auto &heroRegenVigor = std::get<StatsType<int>>(
        hero->m_Stats.m_AllStatsTable[STATS_REGEN_VIGOR]);

    heroHp.m_CurrentValue =
        std::min(heroHp.m_CurrentValue,
                 heroHp.m_CurrentValue + heroRegenHp.m_CurrentValue);
    heroMana.m_CurrentValue =
        std::min(heroMana.m_CurrentValue,
                 heroMana.m_CurrentValue + heroRegenMana.m_CurrentValue);
    heroBerseck.m_CurrentValue =
        std::min(heroBerseck.m_CurrentValue,
                 heroBerseck.m_CurrentValue + heroRegenBerseck.m_CurrentValue);
    heroVigor.m_CurrentValue =
        std::min(heroVigor.m_CurrentValue,
                 heroVigor.m_CurrentValue + heroRegenVigor.m_CurrentValue);
  }
  for (const auto &boss : m_BossesList) {

    auto &hp =
        std::get<StatsType<int>>(boss->m_Stats.m_AllStatsTable[STATS_HP]);
    const auto &regenHp =
        std::get<StatsType<int>>(boss->m_Stats.m_AllStatsTable[STATS_REGEN_HP]);
    auto &mana =
        std::get<StatsType<int>>(boss->m_Stats.m_AllStatsTable[STATS_MANA]);
    const auto &regenMana = std::get<StatsType<int>>(
        boss->m_Stats.m_AllStatsTable[STATS_REGEN_MANA]);
    auto &berseck =
        std::get<StatsType<int>>(boss->m_Stats.m_AllStatsTable[STATS_BERSECK]);
    const auto &regenBerseck = std::get<StatsType<int>>(
        boss->m_Stats.m_AllStatsTable[STATS_RATE_BERSECK]);
    auto &vigor =
        std::get<StatsType<int>>(boss->m_Stats.m_AllStatsTable[STATS_VIGOR]);
    const auto &regenVigor = std::get<StatsType<int>>(
        boss->m_Stats.m_AllStatsTable[STATS_REGEN_VIGOR]);

    hp.m_CurrentValue =
        std::min(hp.m_CurrentValue, hp.m_CurrentValue + regenHp.m_CurrentValue);
    mana.m_CurrentValue = std::min(
        mana.m_CurrentValue, mana.m_CurrentValue + regenMana.m_CurrentValue);
    berseck.m_CurrentValue =
        std::min(berseck.m_CurrentValue,
                 berseck.m_CurrentValue + regenBerseck.m_CurrentValue);
    vigor.m_CurrentValue = std::min(
        vigor.m_CurrentValue, vigor.m_CurrentValue + regenVigor.m_CurrentValue);
  }
}

QString PlayersManager::FormatAtkOnEnnemy(const int damage) {
  return QString("fait %1 de dégâts!").arg(damage);
}

QString PlayersManager::FormatAtkOnAlly(const int damage) {
  return QString("soigne de %4 PV!").arg(damage);
}

QString PlayersManager::FormatAtk(const QString player2,
                                  const QString &atkName) {
  return QString("utilise %2 sur %3!").arg(atkName).arg(player2);
}

int PlayersManager::GetNbOfStatsInEffectList(const Character *chara,
                                             const QString &statsName) const {
  if (chara == nullptr || m_AllEffectsOnGame.count(chara->m_Name) == 0) {
    return 0;
  }

  int counter = 0;
  for (const auto &e : m_AllEffectsOnGame.at(chara->m_Name)) {
    if (e.allAtkEffects.statsName == statsName) {
      counter++;
    }
  }
  return counter;
}

void PlayersManager::ResetCounterOnOneStatsEffect(const Character *chara,
                                                  const QString &statsName) {
  if (chara == nullptr || m_AllEffectsOnGame.count(chara->m_Name) == 0) {
    return;
  }
  for (auto &e : m_AllEffectsOnGame[chara->m_Name]) {
    if (e.allAtkEffects.statsName == statsName) {
      e.allAtkEffects.counterTurn = 0;
    }
  }
}


void PlayersManager::DeleteOneBadEffect(const Character *chara) {
  if (chara == nullptr || m_AllEffectsOnGame.count(chara->m_Name) == 0) {
    return;
  }
  for (auto &e : m_AllEffectsOnGame[chara->m_Name])
     // TODO rule about debuf
      // a DOT can be a debuf for example
    if (e.allAtkEffects.value < 0 && !e.allAtkEffects.statsName.isEmpty()) {
      e.allAtkEffects.counterTurn = e.allAtkEffects.nbTurns;
      break;
    }
}

void PlayersManager::DeleteAllBadEffect(const Character *chara) {
  if (chara == nullptr || m_AllEffectsOnGame.count(chara->m_Name) == 0) {
    return;
  }
  for (auto &e : m_AllEffectsOnGame[chara->m_Name]) {
    if (e.allAtkEffects.value < 0) {
      e.allAtkEffects.counterTurn = e.allAtkEffects.nbTurns;
    }
  }
}

void PlayersManager::DecreaseCoolDownEffects() {
  for (auto &[playerName, allGae] : m_AllEffectsOnGame) {
    for (auto &gae : allGae) {
      if (gae.allAtkEffects.effect == EFFECT_NB_COOL_DOWN &&
          gae.allAtkEffects.subValueEffect > 0) {
        gae.allAtkEffects.subValueEffect--;
      }
    }
  }
}

void PlayersManager::ImproveHotsOnPlayers(const int valuePercent,
                                          const characType launcherType) {
  std::vector<Character *> playerList;

  if (launcherType == characType::Hero) {
    playerList = m_HeroesList;
  } else if (launcherType == characType::Boss) {
    playerList = m_BossesList;
  }
  for (const auto &pl : playerList) {
    if (pl == nullptr || m_AllEffectsOnGame.count(pl->m_Name) == 0) {
      continue;
    }
    for (auto &e : m_AllEffectsOnGame[pl->m_Name]) {
      if (e.allAtkEffects.statsName == STATS_HP) {
        e.allAtkEffects.value += e.allAtkEffects.value * valuePercent / 100;
      }
    }
  }
}
