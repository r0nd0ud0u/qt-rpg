#include "playersmanager.h"

#include "Application.h"

// decode json
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "bossclass.h"
#include "utils.h"

#include "rust-rpg-bridge/attaque.h"
#include "rust-rpg-bridge/utils.h"

#include <fstream>

void PlayersManager::InitHeroes() {
  std::for_each(m_AllHeroesList.begin(), m_AllHeroesList.end(),
                [&](Character *h) {
                  h->LoadAtkJson();
                  h->SortAtkByLevel();
                  h->LoadStuffJson();
                  std::unordered_map<QString, QString> table;
                  for (auto &[bodypart, stuff] : h->m_WearingEquipment) {
                    if (!bodypart.isEmpty()) {
                      table[bodypart] = stuff.m_UniqueName;
                    }
                  }
                  h->SetEquipment(table);
                  h->ApplyEquipOnStats(m_AllEffectsOnGame[h->m_Name]);
                });

  // Hard-coded passive talents
  for (const auto &h : m_AllHeroesList) {
    if (h->m_Name == "Thalia") {
      const auto epParamTalent1 = h->LoadThaliaTalent();
      AddGameEffectOnAtk(h->m_Name, AttaqueType(), h->m_Name, epParamTalent1,
                         0);
      h->ApplyEffeftOnStats(true, m_AllEffectsOnGame[h->m_Name]);
    }
    if (h->m_Name == "Azrak Ombresang") {
      const auto epParamTalent2 = h->LoadAzrakTalent();
      AddGameEffectOnAtk(h->m_Name, AttaqueType(), h->m_Name, epParamTalent2,
                         0);
      h->ApplyEffeftOnStats(true, m_AllEffectsOnGame[h->m_Name]);
    }
    if (h->m_Name == "Thraïn") {
      const auto epParamTalent3 = h->LoadThrainTalent();
      AddGameEffectOnAtk(h->m_Name, AttaqueType(), h->m_Name, epParamTalent3,
                         0);
      h->ApplyEffeftOnStats(true, m_AllEffectsOnGame[h->m_Name]);
    }
    if (h->m_Name == "Elara la guerisseuse de la Lorien") {
      const auto epParamTalent4 = h->LoadElaraTalent();
      // add passive powers
      h->m_Power.is_crit_heal_after_crit = true;
      h->m_Power.is_damage_tx_heal_needy_ally = true;
    }
  }
}

void PlayersManager::InitBosses() {
  for (const auto &boss : m_AllBossesList) {
    boss->m_ColorStr = "red";
    boss->LoadAtkJson();
    boss->SortAtkByLevel();
    boss->LoadStuffJson();
    boss->ApplyEquipOnStats(m_AllEffectsOnGame[boss->m_Name]);
  }
}

void PlayersManager::ClearHeroBossList() {
  m_HeroesList.clear();
  m_BossesList.clear();
}

bool PlayersManager::UpdateActivePlayers() {
  ClearHeroBossList();
  std::for_each(m_AllHeroesList.begin(), m_AllHeroesList.end(),
                [&](Character *c) {
                  if (c != nullptr && c->m_StatsInGame.m_IsPlaying) {
                    m_HeroesList.push_back(c);
                  }
                });
  if (!m_HeroesList.empty()) {
    m_SelectedHero = m_HeroesList.front();
  }

  std::for_each(m_AllBossesList.begin(), m_AllBossesList.end(),
                [&](Character *c) {
                  if (c != nullptr && c->m_StatsInGame.m_IsPlaying) {
                    m_BossesList.push_back(c);
                  }
                });
  return (m_HeroesList.size() > 0 && m_BossesList.size() > 0);
}

void PlayersManager::LoadAllEquipmentsJson() {
  // List all equipment
  QString directoryPath =
      OFFLINE_ROOT_EQUIPMENT; // Replace with the actual path
  QDir directory(directoryPath);
  if (!directory.exists()) {
    Application::GetInstance().log(
        QString("Directory does not exist: %1").arg(directoryPath));
    return;
  }
  // Detect all directories, each directory is a part of the body
  QStringList subdirectories =
      directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

  for (const auto &subdirPath : subdirectories) {
    QDir subdir(directoryPath + subdirPath);
    if (!subdir.exists()) {
      Application::GetInstance().log(
          QString("Directory does not exist: %1").arg(subdirPath));
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
        stuff.m_UniqueName = jsonDoc[EQUIP_UNIQUE_NAME].toString();

        for (const auto &stats : ALL_STATS) {
          if (stuff.m_Stats.m_AllStatsTable.count(stats) == 0) {
            continue;
          }
          // init
          auto &stuffStat = stuff.m_Stats.m_AllStatsTable[stats];
          stuffStat.InitValues(0, 0);
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
        const auto name =
            (stuff.m_UniqueName.isEmpty()) ? stuff.m_Name : stuff.m_UniqueName;
        m_Equipments[jsonDoc[EQUIP_CATEGORY].toString()][name] = stuff;
      }
    }
  }
}

/**
 * @brief PlayersManager::GetActiveCharacterByName
 * Find the pointer of one character by name
 * in the list of all `PLAYING` heros and bosses available
 * Otherwise return nullpointer
 */
Character *PlayersManager::GetActiveCharacterByName(const QString &name) {
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

/**
 * @brief PlayersManager::GetCharacterByName
 * Find the pointer of one character by name
 *  in the `RAW` list of heros and bosses
 * Otherwise return nullpointer
 */
Character *PlayersManager::GetCharacterByName(const QString &name) {
  for (auto *boss : m_AllBossesList) {
    if (name == boss->m_Name) {
      return boss;
    }
  }
  for (auto *hero : m_AllHeroesList) {
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
      const auto effectName =
          build_effect_name(it->allAtkEffects.effect.toStdString(),
                            it->allAtkEffects.statsName.toStdString(), true)
              .data();
      const auto terminated = QString("L'effet %1 sur %2 est terminé.(%3)")
                                  .arg(effectName)
                                  .arg(curPlayerName)
                                  .arg(it->atk.name);
      // One-turn effect are not logged out
      if (it->allAtkEffects.nbTurns > 1) {
        sl.push_back(terminated);
      }
      // remove malus effect from player
      auto *player = GetActiveCharacterByName(curPlayerName);
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
  auto *targetPl = GetActiveCharacterByName(curPlayerName);

  QStringList localLog;

  if (targetPl != nullptr) {
    int hotAndDot = 0;
    for (auto &gae : gaeTable) {
      if (gae.launchingTurn == currentTurn) {
        // effect is applicable at launch of one character and then at the next
        // turn of the target
        continue;
      }
      if (gae.allAtkEffects.statsName == STATS_HP &&
          EFFECTS_HOT_OR_DOT.count(gae.allAtkEffects.effect) > 0) {
        // sum of hot and dot
        hotAndDot += gae.allAtkEffects.value;
        QString type = "DOT->";
        if (gae.allAtkEffects.value > 0) {
          type = "HOT->";
        }
        localLog.append(QString("%1 valeur: %2, atk: %3")
                            .arg(type)
                            .arg(gae.allAtkEffects.value)
                            .arg(gae.atk.name));
      } else if (auto *launcherPl = GetActiveCharacterByName(gae.launcher);
                 launcherPl != nullptr) {
        const auto [output, _] = launcherPl->ApplyOneEffect(
            targetPl, gae.allAtkEffects, fromLaunch, gae.atk);
        if (!output.isEmpty()) {
          localLog.append(output);
        }
      }
    }
    auto &localStat = targetPl->m_Stats.m_AllStatsTable[STATS_HP];
    localStat.m_CurrentValue += hotAndDot;
    if (hotAndDot != 0) {
      localLog.append(QString("HOT et DOT totaux: %1").arg(hotAndDot));
      // update buf overheal
      const int deltaOverHeal = localStat.m_CurrentValue - localStat.m_MaxValue;
      if (deltaOverHeal > 0) {
        const auto *gs = Application::GetInstance().m_GameManager->m_GameState;
        targetPl->m_LastTxRx[static_cast<int>(amountType::overHealRx)]
                            [gs->m_CurrentTurnNb] += deltaOverHeal;
        localLog.append(QString("Ajout Overheal tour%1: %2")
                            .arg(gs->m_CurrentTurnNb)
                            .arg(deltaOverHeal));
      }
      // current value must be included between 0 and max value
      localStat.m_CurrentValue =
          std::min(localStat.m_CurrentValue, localStat.m_MaxValue);
      localStat.m_CurrentValue = std::max(localStat.m_CurrentValue, 0);
    }
  }
  if (!localLog.isEmpty()) {
    logs.append(localLog.join("\n"));
  }

  return logs;
}

/**
 * @brief PlayersManager::ApplyRegenStats
 * Hp, mana, berseck, vigor and speed are updated through their respective regen
 * stat. Regen on {hp, mana and vigor} are by % Regen on {berseck and speed} is
 * by value
 */
void PlayersManager::ApplyRegenStats(const characType &type) {
  std::vector<Character *> playerList = m_HeroesList;
  if (type == characType::Hero) {
    playerList = m_HeroesList;
  } else if (type == characType::Boss) {
    playerList = m_BossesList;
  }
  for (const auto &pl : playerList) {
    if (pl == nullptr || pl->IsDead()) {
      continue;
    }

    auto &hp = pl->m_Stats.m_AllStatsTable[STATS_HP];
    auto &mana = pl->m_Stats.m_AllStatsTable[STATS_MANA];
    auto &berseck = pl->m_Stats.m_AllStatsTable[STATS_BERSECK];
    auto &vigor = pl->m_Stats.m_AllStatsTable[STATS_VIGOR];
    auto &speed = pl->m_Stats.m_AllStatsTable[STATS_SPEED];

    const auto &regenHp = pl->m_Stats.m_AllStatsTable[STATS_REGEN_HP];
    const auto &regenMana = pl->m_Stats.m_AllStatsTable[STATS_REGEN_MANA];
    const auto &regenBerseck = pl->m_Stats.m_AllStatsTable[STATS_RATE_BERSECK];
    const auto &regenVigor = pl->m_Stats.m_AllStatsTable[STATS_REGEN_VIGOR];
    const auto &regenSpeed = pl->m_Stats.m_AllStatsTable[STATS_REGEN_SPEED];

    // hp
    hp.m_CurrentValue =
        std::min(hp.m_MaxValue, hp.m_CurrentValue + regenHp.m_CurrentValue);
    // mana
    mana.m_CurrentValue = std::min(
        mana.m_MaxValue, mana.m_CurrentValue + regenMana.m_CurrentValue);
    // vigor
    vigor.m_CurrentValue = std::min(
        vigor.m_MaxValue, vigor.m_CurrentValue + regenVigor.m_CurrentValue);
    // berseck
    berseck.m_CurrentValue =
        std::min(berseck.m_MaxValue,
                 berseck.m_CurrentValue + regenBerseck.m_CurrentValue);
    // speed
    speed.m_CurrentValue += regenSpeed.m_CurrentValue;
    speed.m_MaxValue += regenSpeed.m_CurrentValue;
    speed.m_RawMaxValue += regenSpeed.m_CurrentValue;
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

/**
 * @brief PlayersManager::GetNbOfActiveHotsOnHeroes
 * @return the number of hots on all heroes
 */
int PlayersManager::GetNbOfActiveHotsOnHeroes() const {
  int counter = 0;
  for (const auto &[name, allE] : m_AllEffectsOnGame) {
    for (const auto &e : allE) {
      if (e.allAtkEffects.statsName == STATS_HP && e.allAtkEffects.value > 0 &&
          e.allAtkEffects.nbTurns > 1 &&
          EFFECTS_HOT_OR_DOT.count(e.allAtkEffects.effect) > 0) {
        counter++;
      }
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
          ALLIES_TARGETS.count(e.allAtkEffects.target) > 0 &&
          EFFECTS_HOT_OR_DOT.count(e.allAtkEffects.effect) > 0 &&
          e.allAtkEffects.value > 0 && e.allAtkEffects.nbTurns > 1) {
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
/// Hero has his life staying at 0. All effects and bufs are reset.
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

  QStringList output;

  // Delete only boss player
  if (launcherType == characType::Boss) {
    const auto newEnd = std::remove_if(
        playerList->begin(), playerList->end(), [&](Character *pl) {
          if (pl == nullptr) {
            return false;
          }
          const bool isDead = pl->IsDead();
          pl->m_StatsInGame.m_IsAlive = !isDead;
          if (isDead) {
            output.append(pl->m_Name);
          }
          return isDead; // remove elements where this is true
        });
    // TODO check if bosses has another phase
    // if yes init that phase
    // if not erase the boss of the list
    playerList->erase(newEnd, playerList->end());
  }

  if (launcherType == characType::Hero) {
    std::for_each(playerList->begin(), playerList->end(), [&](Character *pl) {
      if (pl == nullptr) {
        return;
      }
      if (pl->IsDead()) {
        ResetAllEffectsOnPlayer(pl);
        // process actions on dead hero
        pl->ProcessDeath();
        output.append(pl->m_Name);
      }
    });
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
    if (pl1->IsDead()) {
      continue;
    }
    auto &speedPl1 =
        pl1->m_Stats.m_AllStatsTable.at(STATS_SPEED).m_CurrentValue;
    for (const auto &pl2 : playerList2) {
      const auto &speedpl2 =
          pl2->m_Stats.m_AllStatsTable.at(STATS_SPEED).m_CurrentValue;
      if (speedPl1 - speedpl2 >= speedThreshold) {
        // Update of current value and max value
        speedPl1 -= speedThreshold;
        pl1->m_Stats.m_AllStatsTable.at(STATS_SPEED).m_MaxValue -=
            speedThreshold;
        pl1->m_Stats.m_AllStatsTable.at(STATS_SPEED).m_RawMaxValue -=
            speedThreshold;
        playerOrderTable.push_back(pl1->m_Name);
        break;
      }
    }
  }
}

std::tuple<bool, QString, QStringList>
PlayersManager::IsDodging(const std::vector<TargetInfo *> &targetList,
                          const AttaqueType &atk) {
  QString plName;
  QStringList output;
  const bool isDodging =
      std::any_of(targetList.begin(), targetList.end(),
                  [this, &plName, &output, &atk](const TargetInfo *ti) {
                    if (ti == nullptr) {
                      return false;
                    }
                    if (ti->get_is_targeted()) {
                      auto *targetChara =
                          this->GetActiveCharacterByName(ti->get_name().data());
                      plName = ti->get_name().data();
                      const auto [charIsDodging, randNbStr] =
                          targetChara->IsDodging(atk);
                      output.append(randNbStr);
                      targetChara->ProcessBlock(charIsDodging);
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
  const auto randNb = Utils::GetRandomNb(0, alliesStr.size() - 1);
  auto *c = GetActiveCharacterByName(alliesStr[randNb]);
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
  const int64_t randHeroNb = Utils::GetRandomNb(0, m_HeroesList.size() - 1);
  const int64_t randBossNb = Utils::GetRandomNb(0, m_BossesList.size() - 1);

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

/**
 * @brief PlayersManager::ResetIsFirstRound
 * The boolean is_first_round is reset for all the characters of the game.
 */
void PlayersManager::ResetIsFirstRound() const {
  // Reset
  std::for_each(m_HeroesList.begin(), m_HeroesList.end(),
                [&](const Character *c) {
                  if (c != nullptr && c->m_ExtCharacter != nullptr) {
                    c->m_ExtCharacter->set_is_first_round(true);
                  }
                });
  std::for_each(m_BossesList.begin(), m_BossesList.end(),
                [&](const Character *c) {
                  if (c != nullptr && c->m_ExtCharacter != nullptr) {
                    c->m_ExtCharacter->set_is_first_round(true);
                  }
                });
}

std::vector<Stuff> PlayersManager::LootNewEquipments(const QString &name) {
  uint64_t rank = 0;
  int stuffClass = 0;
  std::vector<Stuff> newStuffs;
  for (const auto *boss : m_AllBossesList) {
    if (boss != nullptr && name == boss->m_Name) {
      rank = boss->m_BossClass.m_Rank;
      break;
    }
  }
  const auto nbOfLoots = rank;
  const auto probaLoot =
      (rank < static_cast<uint64_t>(BossClass::PROBA_LOOTS.size()))
          ? BossClass::PROBA_LOOTS.at(rank)
          : std::vector<uint64_t>{};

  // No loot
  if (nbOfLoots == 0) {
    return newStuffs;
  }
  for (int i = 0; i < nbOfLoots; i++) {
    const auto randProba = static_cast<uint64_t>(Utils::GetRandomNb(0, 100));
    // Assess the rank of the loot
    for (int j = 0; j < probaLoot.size() - 1; j++) {
      if (randProba >= probaLoot[j] && randProba < probaLoot[j + 1]) {
        stuffClass = j + 1;
        break;
      }
    }
    // no loot found
    if (stuffClass == 0) {
      continue;
    }

    // Create stuff
    Stuff stuff;
    // add name
    const auto randEquipType =
        Utils::GetRandomNb(0, RAND_EQUIP_ON_BODY.size() - 1);
    const auto &equipType = RAND_EQUIP_ON_BODY.at(randEquipType);
    const auto indexEquipName =
        Utils::GetRandomNb(0, m_RandomEquipName[equipType].size() - 1);
    stuff.m_UniqueName =
        QString("%1-%2-%3")
            .arg(m_RandomEquipName[equipType].at(indexEquipName))
            .arg(stuffClass)
            .arg(Utils::getCurrentTimeAsString());
    stuff.m_Name = m_RandomEquipName[equipType].at(indexEquipName);
    // add body part
    stuff.m_BodyPart = equipType;
    // add class
    stuff.m_Rank = stuffClass;
    // stuffClass == nb of effects of the loot
    const auto nbOfEffets = stuffClass;

    // Init BONUS_STAT_STR bool used stats
    // one stat must be improved just once
    std::set<int> bonusUsed;
    for (int bonusIdx = 0; bonusIdx < BossClass::BONUS_STAT_STR.size();
         bonusIdx++) {
      bonusUsed.insert(bonusIdx);
    }

    // Update 'nbOfEffets' effects for one stuff
    // one stat can be updated only once
    for (int k = 0; k < nbOfEffets; k++) {
      // get a random bonus in index in bonusUsed
      const auto index = Utils::GetRandomNb(0, bonusUsed.size() - 1);
      auto localBonusIt = bonusUsed.begin();
      std::advance(localBonusIt, index);

      const auto &stat = BossClass::BONUS_STAT_STR.at(*localBonusIt);
      const auto *bonus = BossClass::BONUS_LIST.at(stat).at(stuffClass - 1);
      // store the name of the updated stats
      stuff.m_StatsUpByLoot.push_back(stat);
      // Update is on percent or value
      if (bonus->get_is_percent()) {
        stuff.m_Stats.m_AllStatsTable[stat].m_BufEquipPercent =
            bonus->get_value();
      } else {
        stuff.m_Stats.m_AllStatsTable[stat].m_BufEquipValue =
            bonus->get_value();
      }
      // Erase index of bonusUsed for the next effect to be updated
      if (localBonusIt != bonusUsed.end()) {
        bonusUsed.erase(localBonusIt);
      }
    }
    // Armur bonus does not contain 'no loot'(0) whereas stuffClass is starting
    // at 'no loot'(0) This bonus is for all body parts with some exceptions
    if (const std::set<QString> armurBonusExceptions{
            EQUIP_NECKLACE, EQUIP_LEFT_WEAPON, EQUIP_RIGHT_WEAPON,
            EQUIP_RING_RIGHT, EQUIP_RING_LEFT};
        armurBonusExceptions.count(stuff.m_BodyPart) == 0) {
      const auto armurBonus =
          (stuffClass - 1 < BossClass::ARMOR.size())
              ? BossClass::ARMOR[stuffClass - 1]
              : BossClass::ARMOR[BossClass::ARMOR.size() - 1];
      stuff.m_Stats.m_AllStatsTable[STATS_ARM_MAG].m_BufEquipValue +=
          armurBonus;
      stuff.m_Stats.m_AllStatsTable[STATS_ARM_PHY].m_BufEquipValue +=
          armurBonus;
    }

    newStuffs.push_back(stuff);
  }

  return newStuffs;
}

void PlayersManager::InitRandomEquip() {
  for (const auto &equip : RAND_EQUIP_ON_BODY) {
    // Create an object of ifstream (input file stream) class
    std::ifstream inputFile;
    // Open a file named "example.txt" for reading
    const QString path = OFFLINE_RAND_NAME_STUFF + equip + ".txt";
    inputFile.open(path.toStdString());

    // Check if the file is opened successfully
    if (!inputFile.is_open()) {
      return; // Return an error code
    }

    std::string line;
    while (std::getline(inputFile, line)) {
      line = line.substr(2);
      m_RandomEquipName[equip].push_back(QString::fromStdString(line));
    }
    // Close the file when done
    inputFile.close();
  }
}

/**
 * @brief PlayersManager::GetHeroMostAggro
 * Returns the name of the hero character with the most aggro and the value of
 * the aggro In case of empty list, returns nullopt In case of equal aggro, the
 * output hero is randomly picked among the aggro-equal ones.
 */
std::optional<std::pair<QString, int>>
PlayersManager::GetHeroMostAggro() const {
  std::vector<const Character *> mostAggroCh;
  if (m_HeroesList.empty()) {
    return std::nullopt;
  }
  std::for_each(
      m_HeroesList.begin(), m_HeroesList.end(), [&](const Character *c) {
        if (!mostAggroCh.empty()) {
          const auto &aggroStat1 = mostAggroCh.front()
                                       ->m_Stats.m_AllStatsTable.at(STATS_AGGRO)
                                       .m_CurrentValue;
          const auto &aggroStat2 =
              c->m_Stats.m_AllStatsTable.at(STATS_AGGRO).m_CurrentValue;
          if (aggroStat1 < aggroStat2) {
            mostAggroCh.clear();
            mostAggroCh.push_back(c);
          } else if (aggroStat1 == aggroStat2) {
            mostAggroCh.push_back(c);
          }
        } else {
          mostAggroCh.push_back(c);
        }
      });
  const auto randNb = Utils::GetRandomNb(0, mostAggroCh.size() - 1);

  return std::make_pair(mostAggroCh[randNb]->m_Name,
                        mostAggroCh[randNb]
                            ->m_Stats.m_AllStatsTable.at(STATS_AGGRO)
                            .m_CurrentValue);
}

/**
 * @brief PlayersManager::OutputCharactersInJson
 * output in json all the characters from a list of Character object
 */
void PlayersManager::OutputCharactersInJson(
    const std::vector<Character *> &l) const {
  for (const auto *h : l) {
    if (h == nullptr) {
      continue;
    }
    QFile file;
    QDir logDir;
    QString path = OFFLINE_CHARACTERS;
    logDir.mkpath(path);
    // init json doc
    QJsonObject obj;

    obj.insert(CH_NAME, h->m_Name);
    obj.insert(CH_PHOTO_NAME, h->m_PhotoName);
    const auto type =
        (h->m_type == characType::Boss) ? CH_TYPE_BOSS : CH_TYPE_HERO;
    obj.insert(CH_TYPE, type);
    obj.insert(CH_LEVEL, h->m_Level);
    obj.insert(CH_COLOR, h->m_ColorStr);
    obj.insert(CH_RANK, h->m_BossClass.m_Rank);
    obj.insert(CH_FORM, h->m_SelectedForm);
    auto classCh = STANDARD_CLASS;
    if (h->m_Class == CharacterClass::Tank) {
      classCh = TANK_CLASS;
    }
    obj.insert(CH_CLASS, classCh);

    for (const auto &stats : ALL_STATS) {
      if (h->m_Stats.m_AllStatsTable.count(stats) == 0) {
        continue;
      }
      const auto &st = h->m_Stats.m_AllStatsTable.at(stats);
      QJsonObject item;
      QJsonArray jsonArray;
      item[CH_CURRENT_VALUE] = st.m_CurrentValue;
      item[CH_MAX_VALUE] = st.m_MaxValue;
      jsonArray.append(item);
      if (!jsonArray.empty()) {
        obj[stats] = jsonArray;
      }
    }

    // output json
    QJsonDocument doc(obj);
    QString logFilePath = logDir.filePath(path + h->m_Name + ".json");
    file.setFileName(logFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      Application::GetInstance().log(" Could not open the file for writing " +
                                     logFilePath);
    }
    QTextStream out(&file);
#if QT_VERSION_MAJOR == 6
    out.setEncoding(QStringConverter::Encoding::Utf8);
#else
    out.setCodec("UTF-8");
#endif
    out << doc.toJson() << "\n";
  }
}

void PlayersManager::LoadAllCharactersJson() {
  // List all characters
  QString directoryPath = OFFLINE_CHARACTERS; // Replace with the actual path
  QDir directory(directoryPath);
  if (!directory.exists()) {
    Application::GetInstance().log(
        QString("Directory does not exist: %1").arg(directoryPath));
    return;
  }
  QStringList fileList =
      directory.entryList(QDir::Files | QDir::NoDotAndDotDot);
  for (const QString &file : fileList) {
    QFile json(directoryPath + file);
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

      auto *c = new Character("", characType::Hero, {});
      c->m_Name = jsonDoc[CH_NAME].toString();
      c->m_SelectedForm = jsonDoc[CH_FORM].toString();
      if (const auto &classCh = jsonDoc[CH_CLASS].toString();
          classCh == TANK_CLASS) {
        c->m_Class = CharacterClass::Tank;
      } else {
        c->m_Class = CharacterClass::Standard;
      }
      c->m_PhotoName = jsonDoc[CH_PHOTO_NAME].toString();
      c->m_type = (jsonDoc[CH_TYPE].toString() == CH_TYPE_BOSS)
                      ? characType::Boss
                      : characType::Hero;
      c->m_Level = static_cast<int>(jsonDoc[CH_LEVEL].toDouble());
      c->color = QColor(jsonDoc[CH_COLOR].toString());
      c->m_ColorStr = jsonDoc[CH_COLOR].toString();
      c->m_BossClass.m_Rank = static_cast<int>(jsonDoc[CH_RANK].toDouble());

      for (const auto &stats : ALL_STATS) {
        if (c->m_Stats.m_AllStatsTable.count(stats) == 0) {
          continue;
        }

        // init
        QJsonArray jsonArray = jsonDoc[stats].toArray();
        for (const auto &elem : jsonArray) {
          if (elem.isObject()) {
            const QJsonObject item = elem.toObject();
            const auto current =
                static_cast<int>(item[CH_CURRENT_VALUE].toDouble());
            const auto max = static_cast<int>(item[CH_MAX_VALUE].toDouble());
            c->m_Stats.m_AllStatsTable[stats].InitValues(current, max);
          }
        }
      }
      if (c->m_type == characType::Hero) {
        m_AllHeroesList.push_back(c);
      } else {
        m_AllBossesList.push_back(c);
      }
    }
  }
}

/**
 * @brief PlayersManager::ResetAllEffectsOnPlayer
 * All effects on one player are removed
 * The condition is met by equaling all the counter turn to nbTurns of effect
 */
void PlayersManager::ResetAllEffectsOnPlayer(const Character *chara) {
  if (chara == nullptr || m_AllEffectsOnGame.count(chara->m_Name) == 0) {
    return;
  }
  for (auto &e : m_AllEffectsOnGame[chara->m_Name]) {
    if (!e.allAtkEffects.passiveTalent) {
      e.allAtkEffects.counterTurn = e.allAtkEffects.nbTurns;
    }
  }
  RemoveTerminatedEffectsOnPlayer(chara->m_Name);
}

void PlayersManager::SetSelectedHero(const QString &name) {
  for (auto *hero : m_HeroesList) {
    if (hero->m_Name == name) {
      m_SelectedHero = hero;
      break;
    }
  }
  for (auto *boss : m_BossesList) {
    if (boss->m_Name == name) {
      m_SelectedHero = boss;
      break;
    }
  }
}

/**
 * @brief PlayersManager::ProcessNewDefaultName
 * format `default` string name for a character
 */
QString PlayersManager::ProcessNewDefaultName() {
  return QString("default_%1").arg(GetMaxIndexDefaultName() + 1);
}

/**
 * @brief PlayersManager::GetMaxIndexDefaultName
 * Find the max integer contained in default names among both heroes and bosses
 */
int PlayersManager::GetMaxIndexDefaultName() const {
  const QString defaultName = "default_";
  int max = 0;
  for (const auto *hero : m_AllHeroesList) {
    if (int idx = hero->m_Name.indexOf(defaultName);
        idx >= 0 && hero->m_Name.size() > idx) {
      const int tmp = hero->m_Name.sliced(defaultName.size()).toInt();
      if (max < tmp) {
        max = tmp;
      }
    }
  }
  for (const auto *boss : m_AllBossesList) {
    if (int idx = boss->m_Name.indexOf(defaultName);
        idx >= 0 && boss->m_Name.size() > idx) {
      const int tmp = boss->m_Name.sliced(idx).toInt();
      if (max < tmp) {
        max = tmp;
      }
    }
  }
  return max;
}
