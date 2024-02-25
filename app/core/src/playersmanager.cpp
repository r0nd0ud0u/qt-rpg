#include "playersmanager.h"

#include "Application.h"

// decode json
#include <QDir>
#include <QFile>
#include <QJsonDocument>

void PlayersManager::InitHeroes() {

  Stats stats;
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_HP])
      .SetValues(605, 605, 605, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_MANA])
      .SetValues(369, 369, 369, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_VIGOR])
      .SetValues(126, 126, 126, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_BERSECK])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_BERSECK])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_PHY])
      .SetValues(108, 108, 108, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_MAG])
      .SetValues(35, 35, 35, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_PHY])
      .SetValues(10, 10, 10, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_MAG])
      .SetValues(137, 137, 137, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_AGGRO])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_SPEED])
      .SetValues(25, 25, 1000, 25);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_CRIT])
      .SetValues(20, 20, 20, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_DODGE])
      .SetValues(10, 10, 10, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_HP])
      .SetValues(14, 14, 14, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_MANA])
      .SetValues(40, 40, 40, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_VIGOR])
      .SetValues(5, 5, 5, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_AGGRO])
      .SetValues(1, 1, 1, 0);
  const auto hero1 = new Character("Thalia", characType::Hero, stats);

  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_HP])
      .SetValues(200, 200, 600, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_MANA])
      .SetValues(141, 141, 141, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_VIGOR])
      .SetValues(445, 445, 445, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_BERSECK])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_BERSECK])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_PHY])
      .SetValues(60, 60, 60, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_MAG])
      .SetValues(47, 47, 47, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_PHY])
      .SetValues(115, 115, 115, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_MAG])
      .SetValues(15, 15, 15, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_AGGRO])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_SPEED])
      .SetValues(20, 20, 1000, 20);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_CRIT])
      .SetValues(20, 20, 20, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_DODGE])
      .SetValues(9, 9, 9, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_HP])
      .SetValues(10, 10, 10, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_MANA])
      .SetValues(12, 12, 12, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_VIGOR])
      .SetValues(10, 10, 10, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_AGGRO])
      .SetValues(1, 1, 1, 0);
  const auto hero2 = new Character("Azrak Ombresang", characType::Hero, stats);

  stats.m_HP.SetValues(828, 828, 828, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_MANA])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_MANA])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_BERSECK])
      .SetValues(20, 20, 100, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_BERSECK])
      .SetValues(5, 5, 5, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_PHY])
      .SetValues(85, 85, 85, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_MAG])
      .SetValues(45, 45, 45, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_PHY])
      .SetValues(30, 30, 30, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_MAG])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_AGGRO])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_SPEED])
      .SetValues(14, 14, 1000, 14);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_CRIT])
      .SetValues(15, 15, 15, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_DODGE])
      .SetValues(5, 5, 5, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_HP])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_MANA])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_VIGOR])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_AGGRO])
      .SetValues(3, 3, 3, 0);
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
      .SetValues(2750, 2750, 2750, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_MANA])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_BERSECK])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_BERSECK])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_VIGOR])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_PHY])
      .SetValues(105, 105, 105, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_MAG])
      .SetValues(120, 120, 120, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_PHY])
      .SetValues(80, 80, 80, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_MAG])
      .SetValues(138, 138, 138, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_AGGRO])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_SPEED])
      .SetValues(0, 0, 1000, 5);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_CRIT])
      .SetValues(15, 15, 15, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_DODGE])
      .SetValues(10, 10, 10, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_HP])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_MANA])
      .SetValues(25, 25, 25, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_VIGOR])
      .SetValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_AGGRO])
      .SetValues(0, 0, 0, 0);
  const auto boss1 = new Character("Pignouf", characType::Boss, stats);
  boss1->color = QColor("red");
  m_BossesList.push_back(boss1);

  for (const auto &boss : m_BossesList) {
      boss->LoadAtkJson();
      boss->LoadStuffJson();
      boss->ApplyEquipOnStats(m_Equipments);
  }
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

void PlayersManager::AddGameEffectOnAtk(const QString &launcherName,
                                        const AttaqueType &atk,
                                        const QString &targetName,
                                        const std::vector<effectParam> &effects,
                                        const int currentTurn) {
  for (const auto &e : effects) {
    GameAtkEffects gae;
    gae.launcher = launcherName;
    gae.atk = atk;
    gae.allAtkEffects = e;
    gae.launchingTurn = currentTurn;
    m_AllEffectsOnGame[targetName].push_back(gae);
  }
}

QStringList
PlayersManager::RemoveTerminatedEffectsOnPlayer(const QString &curPlayerName) {
  QStringList sl;
  if (m_AllEffectsOnGame.count(curPlayerName) == 0) {
    return sl;
  }
  auto &gaeTable = m_AllEffectsOnGame[curPlayerName];
  for (auto it = gaeTable.begin(); it != gaeTable.end(); it++) {
    if (it->allAtkEffects.counterTurn == it->allAtkEffects.nbTurns) {
      QString terminated("L'effet %1 sur %2 est terminé.");
      terminated =
          terminated.arg(it->allAtkEffects.statsName).arg(curPlayerName);
      sl.push_back(terminated);
      // remove malus effect from player
      auto *player = GetCharacterByName(curPlayerName);
      if (player != nullptr) {
        player->RemoveMalusEffect(it->allAtkEffects);
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
  return sl;
}

QStringList PlayersManager::ApplyEffectsOnPlayer(const QString &curPlayerName,
                                                 const int currentTurn) {
  QStringList logs;
  if (m_AllEffectsOnGame.count(curPlayerName) == 0) {
    return logs;
  }
  auto &gaeTable = m_AllEffectsOnGame[curPlayerName];
  auto *targetPl = GetCharacterByName(curPlayerName);

  QStringList localLog;
  localLog.append(QString("Sur %1: ").arg(curPlayerName));

  if (targetPl != nullptr) {
    for (auto &gae : gaeTable) {
      if (gae.launchingTurn == currentTurn) {
        continue;
      }
      auto *launcherPl = GetCharacterByName(gae.launcher);
      if (launcherPl != nullptr) {
        localLog.append(launcherPl->ApplyOneEffect(targetPl, gae.allAtkEffects,
                                                   false, gae.atk));
      }
    }
  }
  if (localLog.size() > 1) {
    logs.append(localLog.join("\n"));
  }

  return logs;
}

void PlayersManager::ApplyRegenStats(const characType &type) {
  std::vector<Character *> playerList = m_HeroesList;
  if (type == characType::Hero) {
    playerList = m_HeroesList;
  } else if (type == characType::Boss) {
    playerList = m_BossesList;
  }
  for (const auto &pl : playerList) {

    auto &hp = std::get<StatsType<int>>(pl->m_Stats.m_AllStatsTable[STATS_HP]);
    const auto &regenHp =
        std::get<StatsType<int>>(pl->m_Stats.m_AllStatsTable[STATS_REGEN_HP]);
    auto &mana =
        std::get<StatsType<int>>(pl->m_Stats.m_AllStatsTable[STATS_MANA]);
    const auto &regenMana =
        std::get<StatsType<int>>(pl->m_Stats.m_AllStatsTable[STATS_REGEN_MANA]);
    auto &berseck =
        std::get<StatsType<int>>(pl->m_Stats.m_AllStatsTable[STATS_BERSECK]);
    const auto &regenBerseck = std::get<StatsType<int>>(
        pl->m_Stats.m_AllStatsTable[STATS_RATE_BERSECK]);
    auto &vigor =
        std::get<StatsType<int>>(pl->m_Stats.m_AllStatsTable[STATS_VIGOR]);
    const auto &regenVigor = std::get<StatsType<int>>(
        pl->m_Stats.m_AllStatsTable[STATS_REGEN_VIGOR]);
    auto &speed =
        std::get<StatsType<int>>(pl->m_Stats.m_AllStatsTable[STATS_SPEED]);

    hp.m_CurrentValue =
        std::min(hp.m_MaxValue, hp.m_CurrentValue + regenHp.m_CurrentValue);
    mana.m_CurrentValue = std::min(
        mana.m_MaxValue, mana.m_CurrentValue + regenMana.m_CurrentValue);
    berseck.m_CurrentValue =
        std::min(berseck.m_MaxValue,
                 berseck.m_CurrentValue + regenBerseck.m_CurrentValue);
    vigor.m_CurrentValue = std::min(
        vigor.m_MaxValue, vigor.m_CurrentValue + regenVigor.m_CurrentValue);
    speed.m_CurrentValue =
        std::min(speed.m_MaxValue, speed.m_CurrentValue + speed.m_RegenOnTurn);
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

QString PlayersManager::DeleteOneBadEffect(const Character *chara) {
  if (chara == nullptr) {
    // debug
    return "No target encountered on DeleteOneBadEffect";
  }
  if (m_AllEffectsOnGame.count(chara->m_Name) == 0) {
    // output channel log
    return "Aucun effet actif.";
  }
  for (auto &e : m_AllEffectsOnGame[chara->m_Name])
    // TODO rule about debuf
    // a DOT can be a debuf for example
    if (e.allAtkEffects.target == TARGET_ENNEMY &&
        !e.allAtkEffects.statsName.isEmpty()) {
      e.allAtkEffects.counterTurn = e.allAtkEffects.nbTurns;
      return "supprime un effet néfaste.";
    }

  return "Pas de mauvais effet supprimé.";
}

QString PlayersManager::DeleteAllBadEffect(const Character *chara) {
  if (chara == nullptr) {
    // debug
    return "No target encountered on DeleteAllBadEffect";
  }
  if (m_AllEffectsOnGame.count(chara->m_Name) == 0) {
    // output channel log
    return "Aucun effet actif.";
  }
  for (auto &e : m_AllEffectsOnGame[chara->m_Name]) {
    if (e.allAtkEffects.value < 0) {
      e.allAtkEffects.counterTurn = e.allAtkEffects.nbTurns;
    }
  }
  return "supprime tous les effets néfastes.";
}

void PlayersManager::DecreaseCoolDownEffects(const QString &curPlayerName) {
  if (m_AllEffectsOnGame.count(curPlayerName) == 0) {
    return;
  }
  for (auto &gae : m_AllEffectsOnGame[curPlayerName]) {
    if (gae.allAtkEffects.effect == EFFECT_NB_COOL_DOWN &&
        gae.allAtkEffects.subValueEffect > 0) {
      gae.allAtkEffects.subValueEffect--;
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
      if (e.allAtkEffects.statsName == STATS_HP &&
          ALLIES_TARGETS.count(e.allAtkEffects.target) > 0) {
        e.allAtkEffects.value += e.allAtkEffects.value * valuePercent / 100;
      }
    }
  }
}

void PlayersManager::IncrementCounterEffect() {
  for (auto &[playerName, gaeTable] : m_AllEffectsOnGame) {
    for (auto &gae : gaeTable) {
      gae.allAtkEffects.counterTurn++;
    }
  }
}

/// Boss is deleted from list
/// Hero has only his life staying at 0.
/// That method returns a QStringlist with the different died played.
/// That list will be displayed on channel logs.
QStringList PlayersManager::CheckDiedPlayers(const characType &launcherType) {
  std::vector<Character *> playerList;

  if (launcherType == characType::Hero) {
    playerList = m_HeroesList;
  } else if (launcherType == characType::Boss) {
    playerList = m_BossesList;
  }
  QStringList output;

  auto newEnd = std::remove_if(
      playerList.begin(), playerList.end(), [](const Character *pl) {
        if (pl == nullptr) {
          return false;
        }
        const auto &hp =
            std::get<StatsType<int>>(pl->m_Stats.m_AllStatsTable.at(STATS_HP));
        return hp.m_CurrentValue <= 0; // remove elements where this is true
      });

  std::for_each(newEnd, playerList.end(), [&output](const Character *pl) {
    if (pl != nullptr) {
      output.append(pl->m_Name);
    }
  });

  // Delete only boss player
  if (launcherType == characType::Boss) {
    // TODO check if bosses has another phase
    // if yes init that phase
    // if not erase the boss of the list
    playerList.erase(newEnd, playerList.end());
  }

  return output;
}

void PlayersManager::AddSupAtkTurn(const characType &launcherType,
                                   std::vector<QString> &playerOrderTable) const{
  std::vector<Character *> playerList1;
  std::vector<Character *> playerList2;

  if (launcherType == characType::Hero) {
    playerList1 = m_HeroesList;
    playerList2 = m_BossesList;
  } else if (launcherType == characType::Boss) {
    playerList2 = m_HeroesList;
    playerList1 = m_BossesList;
  }

  const int speedThreshold = 100;
  for (auto *pl1 : playerList1) {
    auto &speedPl1 =
        std::get<StatsType<int>>(pl1->m_Stats.m_AllStatsTable.at(STATS_SPEED))
            .m_CurrentValue;
    for (const auto &pl2 : playerList2) {
      const auto &speedpl2 =
          std::get<StatsType<int>>(pl2->m_Stats.m_AllStatsTable.at(STATS_SPEED))
              .m_CurrentValue;
      if (speedPl1 - speedpl2 >= speedThreshold) {
        speedPl1 -= speedThreshold;
        playerOrderTable.push_back(pl1->m_Name);
        break;
      }
    }
  }
}

std::pair<bool, QString> PlayersManager::IsDodging(const std::vector<TargetInfo>& targetList){
    QString plName;
    const bool isDodging = std::any_of(
        targetList.begin(), targetList.end(), [this, &plName](const TargetInfo &ti) {
            if (ti.m_IsTargeted) {
                const auto *targetChara =
                    this->GetCharacterByName(ti.m_Name);
                plName = ti.m_Name;
                return targetChara->IsDodging();
            }
            return false;
        });

    return std::make_pair(isDodging, plName);
}
