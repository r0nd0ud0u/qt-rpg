#include "playersmanager.h"

#include "Application.h"

// decode json
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

void PlayersManager::InitHeroes() {

  Stats stats;
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_HP])
      .InitValues(135, 135, 135, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_MANA])
      .InitValues(160, 160, 160, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_VIGOR])
      .InitValues(40, 40, 40, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_BERSECK])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_BERSECK])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_PHY])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_MAG])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_PHY])
      .InitValues(10, 10, 10, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_MAG])
      .InitValues(20, 20, 20, 0);
  // TODO set max aggro 9999 is a good idea??
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_AGGRO])
      .InitValues(0, 0, 9999, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_SPEED])
      .InitValues(12, 12, 1000, 12);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_CRIT])
      .InitValues(10, 10, 10, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_DODGE])
      .InitValues(5, 5, 5, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_HP])
      .InitValues(7, 7, 7, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_MANA])
      .InitValues(7, 7, 7, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_VIGOR])
      .InitValues(5, 5, 5, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_AGGRO])
      .InitValues(1, 1, 1, 0);
  const auto hero1 = new Character("Thalia", characType::Hero, stats);

  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_HP])
      .InitValues(145, 145, 145, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_MANA])
      .InitValues(70, 70, 70, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_VIGOR])
      .InitValues(130, 130, 130, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_BERSECK])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_BERSECK])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_PHY])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_MAG])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_PHY])
      .InitValues(20, 20, 20, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_MAG])
      .InitValues(10, 10, 10, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_AGGRO])
      .InitValues(0, 0, 9999, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_SPEED])
      .InitValues(10, 10, 1000, 10);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_CRIT])
      .InitValues(12, 12, 12, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_DODGE])
      .InitValues(6, 6, 6, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_HP])
      .InitValues(7,7, 7, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_MANA])
      .InitValues(7, 7, 7, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_VIGOR])
      .InitValues(5, 5, 5, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_AGGRO])
      .InitValues(1, 1, 1, 0);
  const auto hero2 = new Character("Azrak Ombresang", characType::Hero, stats);

  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_HP])
      .InitValues(155, 155, 155, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_MANA])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_BERSECK])
      .InitValues(20, 20, 100, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_VIGOR])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_BERSECK])
      .InitValues(5, 5, 5, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_PHY])
      .InitValues(50, 50, 50, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_MAG])
      .InitValues(50, 50, 50, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_PHY])
      .InitValues(20, 20, 20, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_MAG])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_AGGRO])
      .InitValues(0, 0, 9999, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_SPEED])
      .InitValues(5, 5, 1000, 5);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_CRIT])
      .InitValues(8, 8, 8, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_DODGE])
      .InitValues(5, 5, 5, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_HP])
      .InitValues(7, 7, 7, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_MANA])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_VIGOR])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_AGGRO])
      .InitValues(3, 3, 3, 0);
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
    hero->ApplyEquipOnStats();
  }
  const auto epParamTalent1 = hero1->LoadThaliaTalent();
  const auto epParamTalent2 = hero2->LoadAzrakTalent();
  const auto epParamTalent3 = hero3->LoadThrainTalent();
  AddGameEffectOnAtk(hero1->m_Name, AttaqueType(), hero1->m_Name,
                     epParamTalent1, 0);
  AddGameEffectOnAtk(hero2->m_Name, AttaqueType(), hero2->m_Name,
                     epParamTalent2, 0);
  AddGameEffectOnAtk(hero3->m_Name, AttaqueType(), hero3->m_Name,
                     epParamTalent3, 0);

  ApplyEffectsOnPlayer(
      hero1->m_Name, 1,
      true); // 1 because launching turn must be different than current turn
  ApplyEffectsOnPlayer(hero2->m_Name, 1, true);
  ApplyEffectsOnPlayer(hero3->m_Name, 1, true);
}

void PlayersManager::InitBosses() {
  Stats stats;
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_HP])
      .InitValues(2750, 2750, 2750, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_MANA])
      .InitValues(9999, 9999, 9999, 9999);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_BERSECK])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_BERSECK])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_VIGOR])
      .InitValues(9999, 9999, 9999, 9999);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_PHY])
      .InitValues(105, 105, 105, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_ARM_MAG])
      .InitValues(120, 120, 120, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_PHY])
      .InitValues(40, 40, 40, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_POW_MAG])
      .InitValues(30, 40, 40, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_AGGRO])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_SPEED])
      .InitValues(0, 0, 1000, 5);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_CRIT])
      .InitValues(15, 15, 15, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_DODGE])
      .InitValues(10, 10, 10, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_HP])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_MANA])
      .InitValues(25, 25, 25, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_REGEN_VIGOR])
      .InitValues(0, 0, 0, 0);
  std::get<StatsType<int>>(stats.m_AllStatsTable[STATS_RATE_AGGRO])
      .InitValues(0, 0, 0, 0);
  const auto boss1 = new Character("Pignouf", characType::Boss, stats);
  boss1->color = QColor("red");
  m_BossesList.push_back(boss1);

  for (const auto &boss : m_BossesList) {
    boss->LoadAtkJson();
    boss->LoadStuffJson();
    boss->ApplyEquipOnStats();
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
#if QT_VERSION_MAJOR == 6
        out.setEncoding(QStringConverter::Encoding::Utf8);
#else
        out.setCodec("UTF-8");
#endif
        QString msg = out.readAll();
        json.close();
        const auto jsonDoc = QJsonDocument::fromJson(msg.toUtf8());
        // decode json

        Stuff stuff;
        stuff.m_Name = jsonDoc[EQUIP_NAME].toString();

#if QT_VERSION_MAJOR == 6
// do some code here
#else
        for (const auto &stats : ALL_STATS) {
          if (stuff.m_Stats.m_AllStatsTable.count(stats) == 0) {
            continue;
          }
          // init
          auto &stuffStat =
              std::get<StatsType<int>>(stuff.m_Stats.m_AllStatsTable[stats]);
          stuffStat.InitValues(0, 0, 0, 0);
          QJsonArray jsonArray = jsonDoc[stats].toArray();
          for (const auto &elem : jsonArray) {
            if (elem.isObject()) {
              const QJsonObject item = elem.toObject();
              for (const auto &key : item.keys()) {
                const auto &val = item[key];
                if (val.isDouble()) {
                  if (key == "percent") {
                    stuffStat.m_BufEquipPercent =
                        static_cast<int>(val.toDouble());
                  } else if (key == "value") {
                    stuffStat.m_BufEquipValue =
                        static_cast<int>(val.toDouble());
                  }
                }
              }
            }
          }
        }
#endif
        m_Equipments[jsonDoc[EQUIP_CATEGORY].toString()][stuff.m_Name] = stuff;
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
                                                 const int currentTurn,
                                                 const bool fromLaunch) {
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
        // effet is applicable at launch of one character and then at the next
        // turn of the target
        continue;
      }
      auto *launcherPl = GetCharacterByName(gae.launcher);
      if (launcherPl != nullptr) {
        const auto output = launcherPl->ApplyOneEffect(
            targetPl, gae.allAtkEffects, fromLaunch, gae.atk);
        if (!output.isEmpty()) {
          localLog.append(output);
        }
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
    auto &vigor =
        std::get<StatsType<int>>(pl->m_Stats.m_AllStatsTable[STATS_VIGOR]);
    const auto &regenVigor = std::get<StatsType<int>>(
        pl->m_Stats.m_AllStatsTable[STATS_REGEN_VIGOR]);
    auto &speed =
        std::get<StatsType<int>>(pl->m_Stats.m_AllStatsTable[STATS_SPEED]);

    hp.m_CurrentValue =
        std::min(hp.m_MaxValue, hp.m_CurrentValue + hp.m_CurrentValue*regenHp.m_CurrentValue/100);
    mana.m_CurrentValue = std::min(
        mana.m_MaxValue, mana.m_CurrentValue + mana.m_CurrentValue*regenMana.m_CurrentValue/100);
    berseck.m_CurrentValue =
        std::min(berseck.m_MaxValue,
                 berseck.m_CurrentValue + berseck.m_RegenOnTurn);
    vigor.m_CurrentValue = std::min(
        vigor.m_MaxValue, vigor.m_CurrentValue + vigor.m_CurrentValue*regenVigor.m_CurrentValue/100);
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
    return "Aucun effet actif.\n";
  }
  for (auto &e : m_AllEffectsOnGame[chara->m_Name])
    // TODO rule about debuf
    // a DOT can be a debuf for example
    if (e.allAtkEffects.target == TARGET_ENNEMY &&
        !e.allAtkEffects.statsName.isEmpty()) {
      e.allAtkEffects.counterTurn = e.allAtkEffects.nbTurns;
      return "supprime un effet néfaste.\n";
    }

  return "Pas de mauvais effet supprimé.\n";
}

QString PlayersManager::DeleteAllBadEffect(const Character *chara) {
  if (chara == nullptr) {
    // debug
    return "No target encountered on DeleteAllBadEffect";
  }
  if (m_AllEffectsOnGame.count(chara->m_Name) == 0) {
    // output channel log
    return "Aucun effet actif.\n";
  }
  for (auto &e : m_AllEffectsOnGame[chara->m_Name]) {
    if (e.allAtkEffects.value < 0) {
      e.allAtkEffects.counterTurn = e.allAtkEffects.nbTurns;
    }
  }
  return "supprime tous les effets néfastes.\n";
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

  if (launcherType == characType::Hero){
      return QStringList();
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

void PlayersManager::AddSupAtkTurn(
    const characType &launcherType,
    std::vector<QString> &playerOrderTable) const {
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

std::pair<bool, QString>
PlayersManager::IsDodging(const std::vector<TargetInfo> &targetList) {
  QString plName;
  const bool isDodging =
      std::any_of(targetList.begin(), targetList.end(),
                  [this, &plName](const TargetInfo &ti) {
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

void PlayersManager::AddExpForHeroes(const int exp) {
  for (auto &pl : m_HeroesList) {
    pl->AddExp(exp);
  }
}
