#include "gamemanager.h"

#include "Application.h"
#include "statsingame.h"
#include "utils.h"

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

void GameManager::InitGamemanager() {
  // init the game
  m_GameState = new GameState();
  // init the players
  m_PlayersManager = new PlayersManager();
  // make directories
  QDir directory(GAMES_DIR);
  if (!directory.exists()) {
    directory.mkdir(GAMES_DIR);
  }
}
void GameManager::InitPlayers() {

  // order is important
  // first equipment
  // then heroes or bosses
  // finally selected hero is init
  m_PlayersManager->LoadEquipmentsJson(OFFLINE_ROOT_EQUIPMENT);
  m_PlayersManager->InitRandomEquip();
  m_PlayersManager->LoadAllCharactersJson(false, "");
  m_PlayersManager->InitHeroes(m_PlayersManager->m_AllHeroesList);
  m_PlayersManager->InitBosses(m_PlayersManager->m_AllBossesList);
  const QString characterPath =
      QString("./offlines/new_characters/");
  m_PlayersManager->OutputCharactersInJson(m_PlayersManager->m_AllHeroesList,
                                           characterPath);
  m_PlayersManager->OutputCharactersInJson(m_PlayersManager->m_AllBossesList,
                                           characterPath);
}

void GameManager::Reset() {
  if (m_PlayersManager != nullptr) {
    m_PlayersManager->Reset();
  }
  if (m_GameState != nullptr) {
    m_GameState->Reset();
  }
}

Character *GameManager::GetSelectedHero() {
  if (m_PlayersManager == nullptr ||
      m_PlayersManager->m_SelectedPlayer == nullptr) {
    return nullptr;
  }

  return m_PlayersManager->m_SelectedPlayer;
}

/**
 * @brief GameManager::ProcessOrderToPlay
 * Each turn the order of playing for the characters is processed.
 * First the hero are playing one turn. Then the bosses, one turn.
 * The order of round between heroes and separately bosses is determined by the
 * highest speed. If one hero is dead, he will be last on the list of playing
 * heroes (maybe he can be revived). Then there is a process to assess a
 * supplementary turn for heroes and for bosses.
 */
void GameManager::ProcessOrderToPlay(std::vector<QString> &orderToPlay) const {
  // to be improved with stats
  // one player can play several times as well in different order
  orderToPlay.clear();

  // sort by speed
  std::sort(m_PlayersManager->m_HeroesList.begin(),
            m_PlayersManager->m_HeroesList.end(), Utils::CompareBySpeed);
  std::vector<QString> deadHeroes;
  for (const auto &hero : m_PlayersManager->m_HeroesList) {
    if (!hero->IsDead()) {
      orderToPlay.push_back(hero->m_Name);
    } else {
      deadHeroes.push_back(hero->m_Name);
    }
  }
  // add dead heroes
  std::for_each(deadHeroes.begin(), deadHeroes.end(),
                [&](const QString &name) { orderToPlay.push_back(name); });
  // add bosses
  // sort by speed
  std::sort(m_PlayersManager->m_BossesList.begin(),
            m_PlayersManager->m_BossesList.end(), Utils::CompareBySpeed);
  for (const auto &boss : m_PlayersManager->m_BossesList) {
    orderToPlay.push_back(boss->m_Name);
  }
  // supplementariy atks to push
  m_PlayersManager->AddSupAtkTurn(characType::Hero, orderToPlay);
  m_PlayersManager->AddSupAtkTurn(characType::Boss, orderToPlay);
}

QString GameManager::ProcessLogOrderToPlay() const {
  QString logs;
  if (m_GameState == nullptr) {
    return "";
  }
  if (!m_GameState->m_OrderToPlay.empty()) {
    logs.append("Ordre des joueurs:\n");
  }
  int counter = 1;
  std::for_each(m_GameState->m_OrderToPlay.begin(),
                m_GameState->m_OrderToPlay.end(),
                [&logs, &counter](const QString &plName) {
                  logs += QString("%1 %2\n").arg(counter).arg(plName);
                  counter++;
                });

  return logs;
}

Character *GameManager::GetCurrentPlayer() {
  if (m_GameState == nullptr) {
    return nullptr;
  }
  const auto nameChara = m_GameState->GetCurrentPlayerName();
  return m_PlayersManager->GetActiveCharacterByName(nameChara);
}

/////////////////////////////////////////
/// \brief GameState::GetCurrentPlayerName
///
QString GameState::GetCurrentPlayerName() {
  return (m_CurrentRound > 0) ? m_OrderToPlay.at(m_CurrentRound - 1) : "";
}

void GameState::RemoveDeadPlayerInTurn(const QString &name) {
  const auto newEnd = std::remove_if(
      m_OrderToPlay.begin(), m_OrderToPlay.end(),
      [&name](const QString &localName) {
        return localName == name; // remove elements where this is true
      });
  m_OrderToPlay.erase(newEnd, m_OrderToPlay.end());
}

void GameState::OutputGameStateOnJson(const QString &filepath) const {
  QJsonObject obj;
  // died bosses by turn
  QJsonObject diedBoss;
  for (const auto &[nbTurn, diedBossPl] : m_DiedEnnemies) {
    diedBoss[QString::number(nbTurn)] = diedBossPl;
  }
  if (!diedBoss.empty()) {
    obj[GAME_STATE_DIED_ENNEMIES] = diedBoss;
  }
  // current round
  obj[GAME_STATE_CURRENT_ROUND] = static_cast<int>(m_CurrentRound);
  // current turn
  obj[GAME_STATE_CURRENT_TURN] = static_cast<int>(m_CurrentTurnNb);
  // order players
  QJsonObject orderPlayers;
  int i = 0;
  for (const auto &pl : m_OrderToPlay) {
    orderPlayers[QString::number(i)] = pl;
    i++;
  }
  if (!orderPlayers.empty()) {
    obj[GAME_STATE_ORDER_PLAYERS] = orderPlayers;
  }
  // game name
  obj[GAME_STATE_GAME_NAME] = m_GameName;

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

void GameState::Reset() {
  m_CurrentTurnNb = 0;
  m_DiedEnnemies.clear();
  m_OrderToPlay.clear();
  m_CurrentRound = 0; // max value = size of m_OrderToPlay
  m_GameName = "";
}

void GameState::LoadGameState(const QString &filepath) {
  const auto [jsonObj, err] = Utils::LoadJsonFile(filepath);
  if (!err.isEmpty()) {
    Application::GetInstance().log(err);
  }
  // decode json
  m_CurrentRound =
      static_cast<uint64_t>(jsonObj[GAME_STATE_CURRENT_ROUND].toInt());
  m_CurrentTurnNb = jsonObj[GAME_STATE_CURRENT_TURN].toInt();
  m_GameName = jsonObj[GAME_STATE_GAME_NAME].toString();
  const auto diedEnnemiesObj = jsonObj[GAME_STATE_DIED_ENNEMIES].toObject();
  const auto orderToPlObj = jsonObj[GAME_STATE_ORDER_PLAYERS].toObject();
#if QT_VERSION_MAJOR == 6
  for (const auto &key : orderToPlObj.keys()) {
    m_OrderToPlay.push_back(orderToPlObj[key].toString());
  }
  for (const auto &key : diedEnnemiesObj.keys()) {
    m_DiedEnnemies[key.toInt()].push_back(diedEnnemiesObj[key].toString());
  }
#endif
}

void GameManager::SaveGame() {
  if (m_PlayersManager == nullptr) {
    return;
  }
  // output characters
  m_PlayersManager->OutputCharactersInJson(m_PlayersManager->m_HeroesList,
                                           m_Paths.characterPath);
  m_PlayersManager->OutputCharactersInJson(m_PlayersManager->m_BossesList,
                                           m_Paths.characterPath);
  // output equipments
  for (const auto *c : m_PlayersManager->m_HeroesList) {
    if (c != nullptr) {
      c->UpdateEquipmentOnJson(m_Paths.equipmentPath);
    }
  }
  // output ongoing effects
  m_PlayersManager->OutputAllOnGoingEffectToJson(m_Paths.ongoingEffectsPath);

  // output game state
  m_GameState->OutputGameStateOnJson(m_Paths.gameState);

  // save outputs stats in game
  StatsInGame::GenerateStatsEndGame(m_Paths.statsInGame);
}

void GameManager::StartGame() {
  // create name of exercise
  const auto timeStr = Utils::getCurrentTimeAsString();
  m_GameState->m_GameName = QString("Game_%1").arg(timeStr);
  // update paths
  BuildPaths(m_GameState->m_GameName);
}

QString GameManager::GetEquipmentPath(const bool isLoot) const {
  QString filepath = OFFLINE_ROOT_EQUIPMENT;
  if (isLoot) {
    filepath = m_Paths.lootEquipment;
  }
  return filepath;
}

QStringList GameManager::GetListOfGames() const {
  QDir dir(GAMES_DIR);
  return dir.entryList(QDir::AllDirs | QDir::NoDotDot | QDir::NoDot);
}

void GameManager::BuildPaths(const QString &gameName) {
  m_GameState->m_GameName = gameName;
  const auto fullPathGameDir =
      QString("%1%2/").arg(GAMES_DIR, m_GameState->m_GameName);
  m_Paths.characterPath =
      QString("%1%2/%3/")
          .arg(GAMES_DIR, m_GameState->m_GameName, GAMES_CHARACTERS);
  m_Paths.equipmentPath =
      QString("%1%2/%3/")
          .arg(GAMES_DIR, m_GameState->m_GameName, GAMES_EQUIPMENT);
  m_Paths.ongoingEffectsPath =
      QString("%1%2/%3.json")
          .arg(GAMES_DIR, m_GameState->m_GameName, GAMES_EFFECTS);
  m_Paths.gameState = QString("%1%2/%3.json")
                          .arg(GAMES_DIR, m_GameState->m_GameName, GAMES_STATE);
  m_Paths.statsInGame =
      QString("%1%2").arg(GAMES_DIR, m_GameState->m_GameName) +
      GAME_STATE_STATS_IN_GAME;
  m_Paths.lootEquipment =
      QString("%1%2/%3/")
          .arg(GAMES_DIR, m_GameState->m_GameName, GAMES_LOOT_EQUIPMENT);
}

bool GameManager::LoadGame(const QString &gameName) {
  if (m_PlayersManager == nullptr || m_GameState == nullptr) {
    return false;
  }
  // reset
  Reset();
  m_PlayersManager->Reset();
  // update gamemanager
  BuildPaths(gameName);
  m_GameState->LoadGameState(m_Paths.gameState);
  m_PlayersManager->LoadEquipmentsJson(OFFLINE_ROOT_EQUIPMENT);
  // load loot equipments for characters
  m_PlayersManager->LoadEquipmentsJson(GAMES_LOOT_EQUIPMENT);
  m_PlayersManager->LoadAllCharactersJson(true, m_Paths.characterPath);
  // update effects for player manager
  m_PlayersManager->LoadAllEffects(m_Paths.ongoingEffectsPath);
  m_PlayersManager->InitBosses(m_PlayersManager->m_BossesList);
  m_PlayersManager->InitHeroes(m_PlayersManager->m_HeroesList);

  QString curPlName;
  if (m_GameState->m_CurrentRound <= m_GameState->m_OrderToPlay.size()) {
    auto rnd = m_GameState->m_CurrentRound;
    if (rnd == 0) {
      rnd++;
    }
    if (rnd - 1 < m_GameState->m_OrderToPlay.size()) {
      curPlName = m_GameState->m_OrderToPlay[rnd - 1];
    }
  }
  m_PlayersManager->m_SelectedPlayer =
      m_PlayersManager->GetActiveCharacterByName(curPlName);

  return m_PlayersManager->UpdateStartingPlayers(true);
}
