#include "playersmanager.h"

#include "Application.h"

// decode json
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "rust-rpg-bridge/attaque.h"
#include "rust-rpg-bridge/utils.h"

void PlayersManager::InitHeroes() {

  Stats stats;
  stats.m_AllStatsTable[STATS_HP].InitValues(135, 135, 135, 0);
  stats.m_AllStatsTable[STATS_MANA].InitValues(160, 160, 160, 0);
  stats.m_AllStatsTable[STATS_VIGOR].InitValues(40, 40, 40, 0);
  stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_POW_PHY].InitValues(10, 10, 10, 0);
  stats.m_AllStatsTable[STATS_POW_MAG].InitValues(20, 20, 20, 0);
  // TODO set max aggro 9999 is a good idea??
  stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 0, 9999, 0);
  stats.m_AllStatsTable[STATS_SPEED].InitValues(12, 12, 1000, 12);
  stats.m_AllStatsTable[STATS_CRIT].InitValues(10, 10, 10, 0);
  stats.m_AllStatsTable[STATS_DODGE].InitValues(5, 5, 5, 0);
  stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(7, 7, 7, 0);
  stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(7, 7, 7, 0);
  stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(5, 5, 5, 0);
  stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(1, 1, 1, 0);
  const auto hero1 = new Character("Thalia", characType::Hero, stats);
  hero1->m_Forms.push_back(ENT_FORM);
  hero1->m_Forms.push_back(BEAR_FORM);

  stats.m_AllStatsTable[STATS_HP].InitValues(145, 145, 145, 0);
  stats.m_AllStatsTable[STATS_MANA].InitValues(70, 70, 70, 0);
  stats.m_AllStatsTable[STATS_VIGOR].InitValues(130, 130, 130, 0);
  stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_POW_PHY].InitValues(20, 20, 20, 0);
  stats.m_AllStatsTable[STATS_POW_MAG].InitValues(10, 10, 10, 0);
  stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 0, 9999, 0);
  stats.m_AllStatsTable[STATS_SPEED].InitValues(10, 10, 1000, 10);
  stats.m_AllStatsTable[STATS_CRIT].InitValues(12, 12, 12, 0);
  stats.m_AllStatsTable[STATS_DODGE].InitValues(6, 6, 6, 0);
  stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(7, 7, 7, 0);
  stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(7, 7, 7, 0);
  stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(5, 5, 5, 0);
  stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(1, 1, 1, 0);
  const auto hero2 = new Character("Azrak Ombresang", characType::Hero, stats);
  hero2->m_Forms.push_back(STANDARD_FORM);

  stats.m_AllStatsTable[STATS_HP].InitValues(155, 155, 155, 0);
  stats.m_AllStatsTable[STATS_MANA].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_BERSECK].InitValues(20, 20, 100, 0);
  stats.m_AllStatsTable[STATS_VIGOR].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(5, 5, 5, 0);
  stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(50, 50, 50, 0);
  stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(50, 50, 50, 0);
  stats.m_AllStatsTable[STATS_POW_PHY].InitValues(20, 20, 20, 0);
  stats.m_AllStatsTable[STATS_POW_MAG].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 0, 9999, 0);
  stats.m_AllStatsTable[STATS_SPEED].InitValues(5, 5, 1000, 5);
  stats.m_AllStatsTable[STATS_CRIT].InitValues(8, 8, 8, 0);
  stats.m_AllStatsTable[STATS_DODGE].InitValues(5, 5, 5, 0);
  stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(7, 7, 7, 0);
  stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(3, 3, 3, 0);
  const auto hero3 = new Character("Thraïn", characType::Hero, stats);
  hero3->m_Forms.push_back(STANDARD_FORM);

  // character flo
  stats.m_AllStatsTable[STATS_HP].InitValues(130, 130, 130, 0);
  stats.m_AllStatsTable[STATS_MANA].InitValues(180, 180, 180, 0);
  stats.m_AllStatsTable[STATS_VIGOR].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_POW_PHY].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_POW_MAG].InitValues(25, 25, 25, 0);
  // TODO set max aggro 9999 is a good idea??
  stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 0, 9999, 0);
  stats.m_AllStatsTable[STATS_SPEED].InitValues(8, 8, 1000, 8);
  stats.m_AllStatsTable[STATS_CRIT].InitValues(10, 10, 10, 0);
  stats.m_AllStatsTable[STATS_DODGE].InitValues(7, 7, 7, 0);
  stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(5, 5, 5, 0);
  stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(5, 5, 5, 0);
  stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(1, 1, 1, 0);
  const auto hero4 = new Character("Elara la guerisseuse de la Lorien",
                                   characType::Hero, stats);
  hero4->m_Forms.push_back(STANDARD_FORM);

  // color
  hero1->color = QColor("green");
  hero2->color = QColor("orange");
  hero3->color = QColor("blue");
  hero4->color = QColor("pink");

  m_HeroesList.push_back(hero1);
  m_HeroesList.push_back(hero2);
  m_HeroesList.push_back(hero3);
  m_HeroesList.push_back(hero4);

  for (auto *hero : m_HeroesList) {
    hero->LoadAtkJson();
    hero->LoadStuffJson();
    std::unordered_map<QString, QString> table;
    for (const auto &[name, stuff] : hero->m_WearingEquipment) {
      if (!stuff.m_Name.isEmpty()) {
        table[name] = stuff.m_Name;
      }
    }
    hero->SetEquipment(table);
    hero->ApplyEquipOnStats();
  }
  const auto epParamTalent1 = hero1->LoadThaliaTalent();
  const auto epParamTalent2 = hero2->LoadAzrakTalent();
  const auto epParamTalent3 = hero3->LoadThrainTalent();
  const auto epParamTalent4 = hero4->LoadElaraTalent();
  AddGameEffectOnAtk(hero1->m_Name, AttaqueType(), hero1->m_Name,
                     epParamTalent1, 0);
  AddGameEffectOnAtk(hero2->m_Name, AttaqueType(), hero2->m_Name,
                     epParamTalent2, 0);
  AddGameEffectOnAtk(hero3->m_Name, AttaqueType(), hero3->m_Name,
                     epParamTalent3, 0);

  hero1->ApplyEffeftOnStats(true);
  hero2->ApplyEffeftOnStats(true);
  hero3->ApplyEffeftOnStats(true);

  // add passive powers
  hero4->m_Power.is_crit_heal_after_crit = true;
  hero4->m_Power.is_damage_tx_heal_needy_ally = true;
}

void PlayersManager::InitBosses() {
  Stats stats;
  stats.m_AllStatsTable[STATS_HP].InitValues(7000, 7000, 7000, 0);
  stats.m_AllStatsTable[STATS_MANA].InitValues(9999, 9999, 9999, 9999);
  stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_VIGOR].InitValues(9999, 9999, 9999, 9999);
  stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(580, 580, 580, 0);
  stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(603, 603, 603, 0);
  stats.m_AllStatsTable[STATS_POW_PHY].InitValues(100, 100, 100, 0);
  stats.m_AllStatsTable[STATS_POW_MAG].InitValues(200, 200, 200, 0);
  stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_SPEED].InitValues(0, 0, 1000, 25);
  stats.m_AllStatsTable[STATS_CRIT].InitValues(21, 21, 21, 0);
  stats.m_AllStatsTable[STATS_DODGE].InitValues(18, 18, 18, 0);
  stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(25, 25, 25, 0);
  stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(0, 0, 0, 0);
  const auto boss1 =
      new Character("La bouche du Mordor", characType::Boss, stats);
  boss1->color = QColor("red");
  // m_BossesList.push_back(boss1);
  boss1->m_Forms.push_back(STANDARD_FORM);

  stats.m_AllStatsTable[STATS_HP].InitValues(4000, 4000, 4000, 0);
  stats.m_AllStatsTable[STATS_MANA].InitValues(9999, 9999, 9999, 9999);
  stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_VIGOR].InitValues(9999, 9999, 9999, 9999);
  stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(500, 500, 500, 0);
  stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(500, 500, 500, 0);
  stats.m_AllStatsTable[STATS_POW_PHY].InitValues(50, 50, 50, 0);
  stats.m_AllStatsTable[STATS_POW_MAG].InitValues(50, 50, 50, 0);
  stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_SPEED].InitValues(0, 0, 1000, 10);
  stats.m_AllStatsTable[STATS_CRIT].InitValues(10, 10, 10, 0);
  stats.m_AllStatsTable[STATS_DODGE].InitValues(10, 10, 10, 0);
  stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(25, 25, 25, 0);
  stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(0, 0, 0, 0);

  for (int i = 0; i < 5; i++) {
    const auto boss2 =
        new Character(QString("Nazgul-%1").arg(i), characType::Boss, stats);
    boss2->m_Forms.push_back(STANDARD_FORM);
    boss2->color = QColor("red");
    // m_BossesList.push_back(boss2);
  }

  stats.m_AllStatsTable[STATS_HP].InitValues(7000, 7000, 7000, 0);
  stats.m_AllStatsTable[STATS_MANA].InitValues(9999, 9999, 9999, 9999);
  stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_VIGOR].InitValues(9999, 9999, 9999, 9999);
  stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(500, 500, 500, 0);
  stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(700, 700, 700, 0);
  stats.m_AllStatsTable[STATS_POW_PHY].InitValues(140, 140, 140, 0);
  stats.m_AllStatsTable[STATS_POW_MAG].InitValues(200, 200, 200, 0);
  stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_SPEED].InitValues(0, 0, 1000, 20);
  stats.m_AllStatsTable[STATS_CRIT].InitValues(15, 15, 15, 0);
  stats.m_AllStatsTable[STATS_DODGE].InitValues(15, 15, 15, 0);
  stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(25, 25, 25, 0);
  stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(0, 0, 0, 0);
  const auto boss3 = new Character("Angmar", characType::Boss, stats);
  boss3->color = QColor("red");
  // m_BossesList.push_back(boss3);
  boss3->m_Forms.push_back(STANDARD_FORM);

  stats.m_AllStatsTable[STATS_HP].InitValues(20000, 20000, 20000, 0);
  stats.m_AllStatsTable[STATS_MANA].InitValues(9999, 9999, 9999, 9999);
  stats.m_AllStatsTable[STATS_BERSECK].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_RATE_BERSECK].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_VIGOR].InitValues(9999, 9999, 9999, 9999);
  stats.m_AllStatsTable[STATS_ARM_PHY].InitValues(800, 800, 800, 0);
  stats.m_AllStatsTable[STATS_ARM_MAG].InitValues(600, 600, 600, 0);
  stats.m_AllStatsTable[STATS_POW_PHY].InitValues(250, 250, 250, 0);
  stats.m_AllStatsTable[STATS_POW_MAG].InitValues(120, 120, 120, 0);
  stats.m_AllStatsTable[STATS_AGGRO].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_SPEED].InitValues(0, 0, 1000, 20);
  stats.m_AllStatsTable[STATS_CRIT].InitValues(20, 20, 20, 0);
  stats.m_AllStatsTable[STATS_DODGE].InitValues(20, 20, 20, 0);
  stats.m_AllStatsTable[STATS_REGEN_HP].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_REGEN_MANA].InitValues(25, 25, 25, 0);
  stats.m_AllStatsTable[STATS_REGEN_VIGOR].InitValues(0, 0, 0, 0);
  stats.m_AllStatsTable[STATS_RATE_AGGRO].InitValues(0, 0, 0, 0);
  const auto boss4 = new Character("Angmar le retour", characType::Boss, stats);
  boss4->color = QColor("red");
  m_BossesList.push_back(boss4);
  boss4->m_Forms.push_back(STANDARD_FORM);

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

        for (const auto &stats : ALL_STATS) {
          if (stuff.m_Stats.m_AllStatsTable.count(stats) == 0) {
            continue;
          }
          // init
          auto &stuffStat = stuff.m_Stats.m_AllStatsTable[stats];
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
        // effect is applicable at launch of one character and then at the next
        // turn of the target
        continue;
      }
      auto *launcherPl = GetCharacterByName(gae.launcher);
      if (launcherPl != nullptr) {
        const auto [output, _] = launcherPl->ApplyOneEffect(
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

    auto &hp = pl->m_Stats.m_AllStatsTable[STATS_HP];
    const auto &regenHp = pl->m_Stats.m_AllStatsTable[STATS_REGEN_HP];
    auto &mana = pl->m_Stats.m_AllStatsTable[STATS_MANA];
    const auto &regenMana = pl->m_Stats.m_AllStatsTable[STATS_REGEN_MANA];
    auto &berseck = pl->m_Stats.m_AllStatsTable[STATS_BERSECK];
    auto &vigor = pl->m_Stats.m_AllStatsTable[STATS_VIGOR];
    const auto &regenVigor = pl->m_Stats.m_AllStatsTable[STATS_REGEN_VIGOR];
    auto &speed = pl->m_Stats.m_AllStatsTable[STATS_SPEED];

    hp.m_CurrentValue = std::min(
        hp.m_MaxValue,
        hp.m_CurrentValue + hp.m_MaxValue * regenHp.m_CurrentValue / 100);
    mana.m_CurrentValue = std::min(
        mana.m_MaxValue,
        mana.m_CurrentValue + mana.m_MaxValue * regenMana.m_CurrentValue / 100);
    berseck.m_CurrentValue = std::min(
        berseck.m_MaxValue, berseck.m_CurrentValue + berseck.m_RegenOnTurn);
    vigor.m_CurrentValue =
        std::min(vigor.m_MaxValue,
                 vigor.m_CurrentValue +
                     vigor.m_MaxValue * regenVigor.m_CurrentValue / 100);
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
  std::vector<Character *> *playerList = nullptr;

  if (launcherType == characType::Hero) {
    playerList = &m_HeroesList;
  } else if (launcherType == characType::Boss) {
    playerList = &m_BossesList;
  }
  if (playerList == nullptr) {
    return QStringList();
  }

  if (launcherType == characType::Hero) {
    return QStringList();
  }

  QStringList output;

  const auto newEnd = std::remove_if(
      playerList->begin(), playerList->end(), [&output](const Character *pl) {
        if (pl == nullptr) {
          return false;
        }
        const auto &hp = pl->m_Stats.m_AllStatsTable.at(STATS_HP);
        QString name = pl->m_Name;
        const bool isDead = hp.m_CurrentValue <= 0;
        if (isDead) {
          output.append(pl->m_Name);
        }
        return isDead; // remove elements where this is true
      });

  // Delete only boss player
  if (launcherType == characType::Boss) {
    // TODO check if bosses has another phase
    // if yes init that phase
    // if not erase the boss of the list
    playerList->erase(newEnd, playerList->end());
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
        pl1->m_Stats.m_AllStatsTable.at(STATS_SPEED).m_CurrentValue;
    for (const auto &pl2 : playerList2) {
      const auto &speedpl2 =
          pl2->m_Stats.m_AllStatsTable.at(STATS_SPEED).m_CurrentValue;
      if (speedPl1 - speedpl2 >= speedThreshold) {
        speedPl1 -= speedThreshold;
        playerOrderTable.push_back(pl1->m_Name);
        break;
      }
    }
  }
}

std::tuple<bool, QString, QStringList>
PlayersManager::IsDodging(const std::vector<TargetInfo *> &targetList) {
  QString plName;
  QStringList output;
  const bool isDodging =
      std::any_of(targetList.begin(), targetList.end(),
                  [this, &plName, &output](const TargetInfo *ti) {
                    if (ti == nullptr) {
                      return false;
                    }
                    if (ti->get_is_targeted()) {
                      const auto *targetChara =
                          this->GetCharacterByName(ti->get_name().data());
                      plName = ti->get_name().data();
                      const auto [charIsDodging, randNbStr] =
                          targetChara->IsDodging();
                      output.append(randNbStr);
                      return charIsDodging;
                    }
                    return false;
                  });

  return std::make_tuple(isDodging, plName, output);
}

void PlayersManager::AddExpForHeroes(const int exp) {
  for (auto &pl : m_HeroesList) {
    pl->AddExp(exp);
  }
}

/**
 * @brief PlayersManager::GetAllDeadliestAllies
 * @param launcherType : enable the check on list : boss or hero
 * @return optional<std::vector<QString>> : if any, the list of the names of
 * characters with minimal HP
 */
std::optional<std::vector<QString>>
PlayersManager::GetAllDeadliestAllies(const characType &launcherType) const {
  const std::vector<Character *> *playerList = nullptr;

  if (launcherType == characType::Hero) {
    playerList = &m_HeroesList;
  } else if (launcherType == characType::Boss) {
    playerList = &m_BossesList;
  }
  if (playerList == nullptr) {
    return std::nullopt;
  }

  std::vector<QString> output;

  const auto minElement = std::min_element(
      playerList->begin(), playerList->end(),
      [](const Character *char1, const Character *char2) {
        if (char1 == nullptr || char2 == nullptr) {
          return false;
        }
        const auto &stat1 = char1->m_Stats.m_AllStatsTable.at(STATS_HP);
        const auto &stat2 = char2->m_Stats.m_AllStatsTable.at(STATS_HP);

        // ratio
        const double ratio1 = (stat1.m_MaxValue > 0)
                                  ? static_cast<double>(stat1.m_CurrentValue) /
                                        static_cast<double>(stat1.m_MaxValue)
                                  : 1;
        const double ratio2 = (stat2.m_MaxValue > 0)
                                  ? static_cast<double>(stat2.m_CurrentValue) /
                                        static_cast<double>(stat2.m_MaxValue)
                                  : 1;

        return ratio1 < ratio2;
      });

  const auto &chp =
      (*minElement)->m_Stats.m_AllStatsTable.at(STATS_HP).m_CurrentValue;

  std::for_each(
      playerList->begin(), playerList->end(), [&](const Character *c) {
        const auto &hp = c->m_Stats.m_AllStatsTable.at(STATS_HP).m_CurrentValue;
        if (hp == chp) {
          output.push_back(c->m_Name);
        }
      });

  return !output.empty() ? std::optional(output) : std::nullopt;
}

/**
 * @brief PlayersManager::ProcessDamageTXHealNeedyAlly
 * Assess all the allies with the minimal hp ratio in a std::vector
 * Choose randomly one in this list and increase its hp current value by 25% of
 * damageTX
 * @param launcherType
 * @param damageTX
 * @return QString: not used at the moment, should be used for the output on
 * channel log
 */
QString
PlayersManager::ProcessDamageTXHealNeedyAlly(const characType &launcherType,
                                             const int damageTX) {

  if (damageTX == 0) {
    return "";
  }

  const auto alliesStr =
      GetAllDeadliestAllies(launcherType).value_or(std::vector<QString>{});
  if (alliesStr.empty()) {
    return "";
  }
  const auto randNb = get_random_nb(0, alliesStr.size() - 1);
  auto *c = GetCharacterByName(alliesStr[randNb]);
  if (c == nullptr) {
    return "";
  }
  auto &hp = c->m_Stats.m_AllStatsTable.at(STATS_HP);
  hp.m_CurrentValue = std::min(
      hp.m_MaxValue, hp.m_CurrentValue + static_cast<int>(0.25 * damageTX));

  return "";
}

/**
 * @brief PlayersManager::GetPlayerListByType
 * There is a number n of defined characType. => n list of characters
 * That method returns a pointer to the list according the characType.
 * nullptr if nothing has been found
 * @return a pointer std::vector<Character *> *
 */
std::vector<Character *> *
PlayersManager::GetPlayerListByType(const characType &launcherType) {
  std::vector<Character *> *playerList = nullptr;

  if (launcherType == characType::Hero) {
    playerList = &m_HeroesList;
  } else if (launcherType == characType::Boss) {
    playerList = &m_BossesList;
  }

  return playerList;
}

/**
 * @brief PlayersManager::ProcessIsRandomTarget
 * For each character in the game, the boolean is_random_target is set to false.
 * In case of atk with effect containing REACH_RAND_INDIVIDUAL,
 * we have to process the one of should be chosen.
 */
void PlayersManager::ProcessIsRandomTarget() const {
  // Reset
  std::for_each(m_HeroesList.begin(), m_HeroesList.end(),
                [&](const Character *c) {
                  if (c != nullptr && c->m_ExtCharacter != nullptr) {
                    c->m_ExtCharacter->set_is_random_target(false);
                  }
                });
  std::for_each(m_BossesList.begin(), m_BossesList.end(),
                [&](const Character *c) {
                  if (c != nullptr && c->m_ExtCharacter != nullptr) {
                    c->m_ExtCharacter->set_is_random_target(false);
                  }
                });

  // get random numbers
  const int64_t randHeroNb = get_random_nb(0, m_HeroesList.size() - 1);
  const int64_t randBossNb = get_random_nb(0, m_BossesList.size() - 1);

  // set who is the random reach
  auto *hero = m_HeroesList[randHeroNb];
  auto *boss = m_BossesList[randBossNb];
  if (hero != nullptr && hero->m_ExtCharacter != nullptr) {
    hero->m_ExtCharacter->set_is_random_target(true);
  }
  if (boss != nullptr && boss->m_ExtCharacter != nullptr) {
    boss->m_ExtCharacter->set_is_random_target(true);
  }
}
