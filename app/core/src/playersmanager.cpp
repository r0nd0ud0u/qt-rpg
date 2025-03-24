#include "playersmanager.h"

#include "Application.h"

// decode json
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "bossclass.h"
#include "gamemanager.h"
#include "utils.h"

#include "rust-rpg-bridge/attaque.h"
#include "rust-rpg-bridge/utils.h"

#include <fstream>

const QString STATS_NAME_DELIMITER = ",";

void PlayersManager::InitHeroes(std::vector<Character *> &heroList) {
  std::for_each(heroList.begin(), heroList.end(), [&](Character *h) {
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
  for (const auto &h : heroList) {
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

void PlayersManager::InitBosses(std::vector<Character *> &bossList) {
  for (const auto &boss : bossList) {
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

bool PlayersManager::UpdateStartingPlayers(const bool isLoadingGame) {
  if (!isLoadingGame) {
    ClearHeroBossList();
    std::for_each(m_AllHeroesList.begin(), m_AllHeroesList.end(),
                  [&](const Character *c) {
                    if (c != nullptr && c->m_StatsInGame.m_IsPlaying) {
                      auto *newC = new Character();
                      *newC = *c;
                      m_HeroesList.push_back(newC);
                    }
                  });
    std::for_each(m_AllBossesList.begin(), m_AllBossesList.end(),
                  [&](const Character *c) {
                    if (c != nullptr && c->m_StatsInGame.m_IsPlaying) {
                      auto *newC = new Character();
                      *newC = *c;
                      m_BossesList.push_back(newC);
                    }
                  });
  }

  return (m_HeroesList.size() > 0 && m_BossesList.size() > 0);
}

void PlayersManager::LoadEquipmentsJson(const QString &dirpath) {
  // List all equipment
  QDir directory(dirpath);
  if (!directory.exists()) {
    Application::GetInstance().log(
        QString("Directory does not exist: %1").arg(dirpath));
    return;
  }
  // Detect all directories, each directory is a part of the body
  QStringList subdirectories =
      directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

  for (const auto &subdirPath : subdirectories) {
    QDir subdir(dirpath + subdirPath);
    if (!subdir.exists()) {
      Application::GetInstance().log(
          QString("Directory does not exist: %1").arg(subdirPath));
    }
    QStringList fileList = subdir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    for (const QString &file : fileList) {
      const auto [jsonObj, err] =
          Utils::LoadJsonFile(dirpath + subdirPath + "/" + file);
      if (!err.isEmpty()) {
        Application::GetInstance().log(err);
      }
      // decode json
      Stuff stuff;
      stuff.m_Name = jsonObj[EQUIP_NAME].toString();
      stuff.m_UniqueName = jsonObj[EQUIP_UNIQUE_NAME].toString();

      for (const auto &stats : ALL_STATS) {
        if (stuff.m_Stats.m_AllStatsTable.count(stats) == 0) {
          continue;
        }
        // init
        auto &stuffStat = stuff.m_Stats.m_AllStatsTable[stats];
        stuffStat.InitValues(0, 0);
        QJsonArray jsonArray = jsonObj[stats].toArray();
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
                  stuffStat.m_BufEquipValue = static_cast<int>(val.toDouble());
                }
              }
            }
          }
        }
      }
      const auto name =
          (stuff.m_UniqueName.isEmpty()) ? stuff.m_Name : stuff.m_UniqueName;
      m_Equipments[jsonObj[EQUIP_CATEGORY].toString()][name] = stuff;
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
      // One-turn effect are not logged out
      if (it->allAtkEffects.nbTurns > 1) {
        sl.push_back(QString("L'effet %1 sur %2 est terminé.(%3)")
                         .arg(effectName)
                         .arg(curPlayerName)
                         .arg(it->atk.name));
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
        const auto effectOutcome = launcherPl->ApplyOneEffect(
            targetPl, gae.allAtkEffects, fromLaunch, gae.atk);
        if (!effectOutcome.logDisplay.isEmpty()) {
          localLog.append(effectOutcome.logDisplay);
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
    hp.m_CurrentRawValue = Utils::MultiplyIntByDouble(
        hp.m_RawMaxValue, Utils::CalcRatio(hp.m_CurrentValue, hp.m_MaxValue));
    // mana
    mana.m_CurrentValue = std::min(
        mana.m_MaxValue, mana.m_CurrentValue + regenMana.m_CurrentValue);
    mana.m_CurrentRawValue = Utils::MultiplyIntByDouble(
        mana.m_RawMaxValue,
        Utils::CalcRatio(mana.m_CurrentValue, mana.m_MaxValue));
    // vigor
    vigor.m_CurrentValue = std::min(
        vigor.m_MaxValue, vigor.m_CurrentValue + regenVigor.m_CurrentValue);
    vigor.m_CurrentRawValue = Utils::MultiplyIntByDouble(
        vigor.m_RawMaxValue,
        Utils::CalcRatio(vigor.m_CurrentValue, vigor.m_MaxValue));
    // berseck
    berseck.m_CurrentValue =
        std::min(berseck.m_MaxValue,
                 berseck.m_CurrentValue + regenBerseck.m_CurrentValue);
    berseck.m_RawMaxValue = Utils::MultiplyIntByDouble(
        berseck.m_CurrentRawValue,
        Utils::CalcRatio(berseck.m_CurrentValue, berseck.m_MaxValue));
    // speed
    speed.m_CurrentValue += regenSpeed.m_CurrentValue;
    speed.m_MaxValue += regenSpeed.m_CurrentValue;
    speed.m_RawMaxValue += regenSpeed.m_CurrentValue;
    speed.m_CurrentRawValue = Utils::MultiplyIntByDouble(
        speed.m_RawMaxValue,
        Utils::CalcRatio(speed.m_CurrentValue, speed.m_MaxValue));
  }
}

QString PlayersManager::FormatAtkOnEnnemy(const int damage) {
  return QString("fait %1 de dégâts!").arg(damage);
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
    auto &spd1 = pl1->m_Stats.m_AllStatsTable.at(STATS_SPEED);
    auto &curSpeedPl1 = spd1.m_CurrentValue;
    for (const auto &pl2 : playerList2) {
      const auto &spd2 = pl2->m_Stats.m_AllStatsTable.at(STATS_SPEED);
      const auto &speedpl2 = spd2.m_CurrentValue;
      if (curSpeedPl1 - speedpl2 >= speedThreshold) {
        // Update of current value and max value
        curSpeedPl1 -= speedThreshold;
        spd1.m_MaxValue -= speedThreshold;
        spd1.m_RawMaxValue -= speedThreshold;
        spd1.m_CurrentRawValue = Utils::MultiplyIntByDouble(
            spd1.m_RawMaxValue,
            Utils::CalcRatio(spd1.m_CurrentValue, spd1.m_MaxValue));

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
        const double ratio1 =
            Utils::CalcRatio(stat1.m_CurrentValue, stat1.m_MaxValue);
        const double ratio2 =
            Utils::CalcRatio(stat2.m_CurrentValue, stat2.m_MaxValue);

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
    stuff.m_IsLoot = true;
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

  for (const auto &s : newStuffs) {
    m_Equipments[s.m_BodyPart][s.m_UniqueName] = s;
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
void PlayersManager::OutputCharactersInJson(const std::vector<Character *> &l,
                                            const QString &outputPath) const {
  for (const auto *h : l) {
    if (h == nullptr) {
      continue;
    }
    QDir logDir;
    logDir.mkpath(outputPath);
    // init json doc
    QJsonObject obj;

    obj.insert("Name", h->m_Name);
    QString shortName = h->m_ShortName;
    if(shortName.isEmpty()){
        shortName = h->m_Name;
    }
    obj.insert("Short name", shortName);
    obj.insert("Photo", h->m_PhotoName);
    const auto type =
        (h->m_Type == characType::Boss) ? CH_TYPE_BOSS : CH_TYPE_HERO;
    obj.insert("Type", type);
    obj.insert("Level", h->m_Level);
    obj.insert("Experience", h->m_Exp);
    obj.insert("Color", h->m_ColorStr);
    obj.insert("Rank", h->m_BossClass.m_Rank);
    obj.insert("Shape", h->m_SelectedForm);
    const auto classCh =
        (h->m_Class == CharacterClass::Tank) ? TANK_CLASS : STANDARD_CLASS;
    obj.insert("Class", classCh);
    obj.insert("nb-actions-in-round", h->m_ActionsDoneInRound);

    QJsonObject allStats;
    for (const auto &stats : ALL_STATS) {
      if (h->m_Stats.m_AllStatsTable.count(stats) == 0) {
        continue;
      }
      QJsonObject item;
      const auto &st = h->m_Stats.m_AllStatsTable.at(stats);
      const double ratio = Utils::CalcRatio(st.m_CurrentValue, st.m_MaxValue);
      item["Current"] = ratio * st.m_RawMaxValue;
      item["Max"] = st.m_RawMaxValue;
      allStats[conv.at(stats)] = item;
    }
    obj.insert("Stats", allStats);
    // buf - debuf
    QJsonObject bufObj;
    QJsonArray bufJa;
    for (int i = 0; i < static_cast<int>(BufTypes::enumSize); i++) {
      if (i >= h->m_AllBufs.size()) {
        break;
      }
      const auto b = h->m_AllBufs[i];
      const auto stats = b->get_all_stat_name();
      QString strStats;
      std::for_each(stats.begin(), stats.end(), [&](const ::rust::String &str) {
        strStats += str.data() + STATS_NAME_DELIMITER;
      });
      bufObj[CH_BUF_ALL_STATS] = strStats;
      bufObj[CH_BUF_IS_PERCENT] = b->get_is_percent();
      bufObj[CH_BUF_PASSIVE_ENABLED] = b->get_is_passive_enabled();
      bufObj[CH_BUF_VALUE] = b->get_value();
      bufObj[CH_BUF_TYPE] = i;
      bufJa.append(bufObj);
    }
    if (!bufJa.empty()) {
      obj[CH_BUF_DEBUF] = bufJa;
    }

    // m_LastTxRx
    QJsonObject lastTxRxObj;
    QJsonArray lastTxRxJa;
    for (int i = 0; i < static_cast<int>(BufTypes::enumSize); i++) {
      if (i >= h->m_LastTxRx.size()) {
        break;
      }
      const auto val = h->m_LastTxRx[i];
      QJsonObject valByTurnObj;
      QJsonArray valByTurnJa;
      lastTxRxObj[CH_TXRX_TYPE] = i;
      lastTxRxObj[CH_TXRX_SIZE] = static_cast<int>(val.size());
      for (const auto &[k, v] : val) {
        lastTxRxObj[QString::number(k)] = static_cast<int>(v);
      }
      lastTxRxJa.append(lastTxRxObj);
    }
    if (!lastTxRxJa.empty()) {
      obj[CH_TXRX] = lastTxRxJa;
    }

    // powers
    QJsonObject powersJson;
    powersJson[CH_POWERS_CRIT_AFTER_HEAL] = h->m_Power.is_crit_heal_after_crit;
    powersJson[CH_POWERS_DMG_TX_ALLY] = h->m_Power.is_damage_tx_heal_needy_ally;
    obj["Powers"] = powersJson;
    // extended character
    QJsonObject extJson;
    extJson[CH_EXT_HEAL_ATK_BLOCKED] = h->m_ExtCharacter->get_is_heal_atk_blocked();
    extJson[CH_EXT_RAND_TARGET] = h->m_ExtCharacter->get_is_random_target();
    extJson[CH_EXT_FIRST_ROUND] = h->m_ExtCharacter->get_is_first_round();
    obj["ExtendedCharacter"] = extJson;
    // blocking atk
    obj[CH_BLOCKING_ATK] = h->m_IsBlockingAtk;
    // max actions in round
    obj[CH_MAX_NB_ACTIONS_ROUND] = h->m_MaxNbActionsInRound;

    // output json
    QJsonDocument doc(obj);
    const QString logFilePath =
        logDir.filePath(outputPath + h->m_Name + ".json");
    QFile file;
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
    file.close();
  }
}

void PlayersManager::LoadAllCharactersJson(const bool isLoadingGame,
                                           const QString &pathForLoadingGame) {
  // List all characters
  QString directoryPath = OFFLINE_CHARACTERS; // Replace with the actual path
  if (isLoadingGame) {
    directoryPath = pathForLoadingGame;
  }
  QDir directory(directoryPath);
  if (directoryPath.isEmpty() || !directory.exists()) {
    Application::GetInstance().log(
        QString("Directory does not exist: %1").arg(directoryPath));
    return;
  }
  QStringList fileList =
      directory.entryList(QDir::Files | QDir::NoDotAndDotDot);
  for (const QString &file : fileList) {
    const auto [jsonObj, err] = Utils::LoadJsonFile(directoryPath + file);
    if (!err.isEmpty()) {
      Application::GetInstance().log(err);
    }
    // decode json
    auto *c = new Character("", characType::Hero, {});
    c->m_Name = jsonObj[CH_NAME].toString();
    c->m_ShortName = jsonObj[CH_SHORT_NAME].toString();
    if(c->m_ShortName.isEmpty()){
        c->m_ShortName = c->m_Name;
    }
    c->m_SelectedForm = jsonObj[CH_FORM].toString();
    if (const auto &classCh = jsonObj[CH_CLASS].toString();
        classCh == TANK_CLASS) {
      c->m_Class = CharacterClass::Tank;
    } else {
      c->m_Class = CharacterClass::Standard;
    }
    c->m_PhotoName = jsonObj[CH_PHOTO_NAME].toString();
    c->m_Type = (jsonObj[CH_TYPE].toString() == CH_TYPE_BOSS)
                    ? characType::Boss
                    : characType::Hero;
    c->m_Level = static_cast<int>(jsonObj[CH_LEVEL].toDouble());
    c->m_Exp = static_cast<int>(jsonObj[CH_EXP].toDouble());
    c->m_ActionsDoneInRound =
        static_cast<int>(jsonObj[CH_ACTIONS_IN_ROUND].toDouble());
    c->color = QColor(jsonObj[CH_COLOR].toString());
    c->m_ColorStr = jsonObj[CH_COLOR].toString();
    c->m_BossClass.m_Rank = static_cast<int>(jsonObj[CH_RANK].toDouble());

    // load buf - debuf
    const QJsonArray bufDebufArray = jsonObj[CH_BUF_DEBUF].toArray();
    for (const auto &elem : bufDebufArray) {
      if (elem.isObject()) {
        const QJsonObject item = elem.toObject();
        const auto idx = item[CH_BUF_TYPE].toInt();
        const auto allstats = item[CH_BUF_ALL_STATS].toString();
        const auto isPercent = item[CH_BUF_IS_PERCENT].toBool();
        const auto isPassive = item[CH_BUF_PASSIVE_ENABLED].toBool();
        const auto value = item[CH_BUF_VALUE].toInt();
        if (idx < c->m_AllBufs.size()) {
          c->m_AllBufs[idx]->set_buffers(value, isPercent);
          c->m_AllBufs[idx]->set_is_passive_enabled(isPassive);
          const auto splits = allstats.split(STATS_NAME_DELIMITER);
          for (const auto &s : splits) {
            c->m_AllBufs[idx]->add_stat_name(s.toStdString());
          }
        }
      }
    }

    // tx rx
    const QJsonArray txRxArray = jsonObj[CH_TXRX].toArray();
    for (const auto &elem : txRxArray) {
      if (elem.isObject()) {
        const QJsonObject item = elem.toObject();
        const auto idx = item[CH_TXRX_TYPE].toInt();
        const auto size = item[CH_TXRX_SIZE].toInt();
        std::unordered_map<uint64_t, uint64_t> tmp;
        for (int i = 0; i < size; i++) {
          if (i < item.size()) {
            tmp[i] = item[QString::number(i)].toInt();
          }
        }
        if (idx < c->m_LastTxRx.size()) {
          c->m_LastTxRx[idx] = tmp;
        }
      }
    }

    // powers
    c->m_Power.is_crit_heal_after_crit =
        jsonObj[CH_POWERS_CRIT_AFTER_HEAL].toBool();
    c->m_Power.is_damage_tx_heal_needy_ally =
        jsonObj[CH_POWERS_DMG_TX_ALLY].toBool();
    // extended character
    c->m_ExtCharacter->set_is_heal_atk_blocked(
        jsonObj[CH_EXT_HEAL_ATK_BLOCKED].toBool());
    c->m_ExtCharacter->set_is_random_target(
        jsonObj[CH_EXT_RAND_TARGET].toBool());
    c->m_ExtCharacter->set_is_first_round(jsonObj[CH_EXT_FIRST_ROUND].toBool());
    // blocking atk
    c->m_IsBlockingAtk = jsonObj[CH_BLOCKING_ATK].toBool();
    // max actions in round
    c->m_MaxNbActionsInRound = jsonObj[CH_MAX_NB_ACTIONS_ROUND].toInt();
    // TOD0 to remove someday hehe
    if (c->m_MaxNbActionsInRound == 0) {
      c->m_MaxNbActionsInRound = 1;
    }

    for (const auto &stats : ALL_STATS) {
      if (c->m_Stats.m_AllStatsTable.count(stats) == 0) {
        continue;
      }

      // init
      const QJsonArray jsonArray = jsonObj[stats].toArray();
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
    if (isLoadingGame) {
      c->m_StatsInGame.m_IsPlaying = true;
        if (c->m_Type == characType::Hero) {
        m_HeroesList.push_back(c);
      } else {
        m_BossesList.push_back(c);
      }
    } else {
        if (c->m_Type == characType::Hero) {
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

void PlayersManager::SetSelectedPlayer(const QString &name) {
  for (auto *hero : m_HeroesList) {
    if (hero == nullptr) {
      continue;
    }
    if (hero->m_Name == name) {
      m_SelectedPlayer = hero;
      break;
    }
  }
  for (auto *boss : m_BossesList) {
    if (boss == nullptr) {
      continue;
    }
    if (boss->m_Name == name) {
      m_SelectedPlayer = boss;
      break;
    }
  }
}

/**
 * @brief PlayersManager::ProcessNewDefaultName
 * format `default` string name for a character
 */
QString PlayersManager::ProcessNewDefaultName() const {
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
    if (auto idx = static_cast<int>(hero->m_Name.indexOf(defaultName));
        idx >= 0 && hero->m_Name.size() > idx) {
      const int tmp = hero->m_Name.sliced(defaultName.size()).toInt();
      if (max < tmp) {
        max = tmp;
      }
    }
  }
  for (const auto *boss : m_AllBossesList) {
    if (auto idx = static_cast<int>(boss->m_Name.indexOf(defaultName));
        idx >= 0 && boss->m_Name.size() > idx) {
      const int tmp = boss->m_Name.sliced(idx).toInt();
      if (max < tmp) {
        max = tmp;
      }
    }
  }
  return max;
}

void PlayersManager::OutputAllOnGoingEffectToJson(
    const QString &filepath) const {
  QJsonArray ja;
  for (const auto &[pl, gaeTable] : m_AllEffectsOnGame) {
    for (const auto &gae : gaeTable) {
      if (gae.allAtkEffects.passiveTalent) {
        // passive effect is not saved, it is not a new effect from the game
        continue;
      }
      auto item = gae.allAtkEffects.EffectToJsonObject();
      item[EFFECT_TARGET] = pl;
      item[EFFECT_LAUNCHER] = gae.launcher;
      item[ATK_NAME] = gae.atk.name;
      item[EFFECT_INDEX_TURN] = gae.launchingTurn;
      ja.append(item);
    }
  }
  QJsonObject obj;
  if (!ja.empty()) {
    obj[EFFECT_ARRAY] = ja;
  }

  // output ongoing effects json
  QJsonDocument doc(obj);
  QFile file;
  QDir logDir;
  file.setFileName(logDir.filePath(filepath));
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    // add log
    return;
  }
  QTextStream out(&file);
#if QT_VERSION_MAJOR == 6
  out.setEncoding(QStringConverter::Encoding::Utf8);
#else
  out.setCodec("UTF-8");
#endif
  out << doc.toJson() << "\n";
  file.close();
}

void PlayersManager::Reset() {
  for (auto *c : m_HeroesList) {
    delete c;
  }
  m_HeroesList.clear();
  for (auto *c : m_BossesList) {
    delete c;
  }
  m_BossesList.clear();
  m_SelectedPlayer = nullptr;
  m_ActivePlayer = nullptr;
  m_AllEffectsOnGame.clear();
  m_Equipments.clear();
}

void PlayersManager::LoadAllEffects(const QString &filepath) {
  const auto [jsonObj, err] = Utils::LoadJsonFile(filepath);
  if (!err.isEmpty()) {
    Application::GetInstance().log(err);
  }
  // decode json
  const QJsonArray effectArray = jsonObj[EFFECT_ARRAY].toArray();
#if QT_VERSION_MAJOR == 6
  for (const auto &effect : effectArray) {
    if (effect[EFFECT_STAT].toString().isEmpty() ||
        effect[EFFECT_TYPE].toString().isEmpty()) {
      continue;
    }
    GameAtkEffects gae;
    gae.launcher = effect[EFFECT_LAUNCHER].toString();
    gae.atk.name = effect[ATK_NAME].toString();
    gae.target = effect[EFFECT_TARGET].toString();
    // effect param
    gae.allAtkEffects.target = gae.target;
    gae.allAtkEffects.effect = effect[EFFECT_TYPE].toString();
    gae.allAtkEffects.value = effect[EFFECT_VALUE].toInt();
    gae.allAtkEffects.nbTurns = effect[EFFECT_ACTIVE_TURNS].toInt();
    gae.allAtkEffects.reach = effect[EFFECT_REACH].toString();
    gae.allAtkEffects.statsName = effect[EFFECT_STAT].toString();
    gae.allAtkEffects.subValueEffect = effect[EFFECT_SUB_VALUE].toInt();
    gae.allAtkEffects.counterTurn = effect[EFFECT_COUNTER_TURN].toInt();
    // processed
    gae.allAtkEffects.isMagicAtk = effect[EFFECT_IS_MAGIC].toBool();

    m_AllEffectsOnGame[gae.target].push_back(gae);
  }
#endif
}

bool PlayersManager::CheckGameOver() const{
  for (const auto *h : m_HeroesList) {
    if (h != nullptr && !h->IsDead()) {
      return false;
    }
  }
  return true;
}
