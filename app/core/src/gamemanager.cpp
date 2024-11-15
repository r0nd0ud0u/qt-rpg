#include "gamemanager.h"

#include "utils.h"

#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>

void GameManager::InitPlayers() {
  // init the game
  m_GameState = new GameState();
  // init the players
  m_PlayersManager = new PlayersManager();
  // order is important
  // first equipment
  // then heroes or bosses
  // finally selected hero is init
  m_PlayersManager->LoadAllEquipmentsJson();
  m_PlayersManager->InitRandomEquip();
  m_PlayersManager->LoadAllCharactersJson();
  m_PlayersManager->InitHeroes();
  m_PlayersManager->InitBosses();

  // make directories
  QDir directory(OFFLINE_SAVES);
  if (!directory.exists()) {
    directory.mkdir(OFFLINE_SAVES);
  }
}

Character *GameManager::GetSelectedHero() {
  if (m_PlayersManager == nullptr ||
      m_PlayersManager->m_SelectedHero == nullptr) {
    return nullptr;
  }

  return m_PlayersManager->m_SelectedHero;
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

void GameState::OutputGameStateOnJson(const QString& filepath){
    QJsonObject obj;
    // died bosses by turn
    QJsonObject diedBoss;
    for(const auto& [nbTurn, diedBossPl] : m_DiedEnnemies){
        diedBoss[QString::number(nbTurn)] = diedBossPl;
    }
    if (!diedBoss.empty()) {
        obj[GAME_STATE_DIED_ENNEMIES] = diedBoss;
    }
    // current round
    obj[GAME_STATE_CURRENT_ROUND] = QString::number(m_CurrentRound);
    // current turn
    obj[GAME_STATE_CURRENT_TURN] = QString::number(m_CurrentTurnNb);
    // order players
    QJsonObject orderPlayers;
    int i = 0;
    for(const auto& pl: m_OrderToPlay){
        orderPlayers[QString::number(i)] = pl;
        i++;
    }
    if (!diedBoss.empty()) {
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

  // output stats in game

  // output loot equipment
}

void GameManager::StartGame() {
  // create name of exercise
  const auto timeStr = Utils::getCurrentTimeAsString();
  m_GameState->m_GameName = QString("Game_%1").arg(timeStr);
  // update paths
  m_Paths.characterPath =
      QString("%1%2/%3/")
          .arg(GAMES_DIR, m_GameState->m_GameName, GAMES_CHARACTERS);
  m_Paths.equipmentPath =
      QString("%1%2/%3/")
          .arg(GAMES_DIR, m_GameState->m_GameName, GAMES_EQUIPMENT);
  m_Paths.ongoingEffectsPath =
      QString("%1%2/%3.json")
          .arg(GAMES_DIR, m_GameState->m_GameName, GAMES_EFFECTS);
  m_Paths.gameState =
      QString("%1%2/%3.json")
          .arg(GAMES_DIR, m_GameState->m_GameName, GAMES_STATE);
}
